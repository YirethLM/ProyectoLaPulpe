
#include "GestorUsuarios.h"
#include "Clave.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <limits>
#include <cmath>

namespace {
    QString q(const std::string& s) {
        return QString::fromStdString(s);
    }

    QString normalizar(const QString& s) {
        return s.trimmed().toCaseFolded();
    }

    bool rolValido(Rol r) {
        return r == Rol::Administrador ||
            r == Rol::Cajero;
    }
}

GestorUsuarios::GestorUsuarios(const QString& r)
    : ruta(r) {
}

QString GestorUsuarios::ultimoError() const {
    return error;
}

bool GestorUsuarios::estaVacio() const {
    return usuarios.empty();
}

const std::vector<std::unique_ptr<Usuario>>&
GestorUsuarios::listarUsuarios() const {
    return usuarios;
}

const Usuario* GestorUsuarios::buscarUsuarioPorNombre(
    const QString& nombre) const {

    for (const auto& u : usuarios) {
        if (normalizar(q(u->nombreUsuario)) ==
            normalizar(nombre)) {
            return u.get();
        }
    }
    return nullptr;
}

const Usuario* GestorUsuarios::buscarUsuarioPorId(
    int id) const {

    for (const auto& u : usuarios) {
        if (u->id == id) {
            return u.get();
        }
    }
    return nullptr;
}

int GestorUsuarios::administradoresActivos() const {
    int total = 0;

    for (const auto& u : usuarios) {
        if (u->activo &&
            u->rol == Rol::Administrador) {
            ++total;
        }
    }
    return total;
}

bool GestorUsuarios::cargar() {
    error.clear();
    listo = false;

    if (!usuarios.empty()) {
        error = "La carga solo se permite al iniciar el gestor.";
        return false;
    }

    if (!QFileInfo::exists(ruta)) {
        listo = true;
        return true;
    }

    QFile archivo(ruta);

    if (!archivo.open(QIODevice::ReadOnly)) {
        error = "No se pudo abrir usuarios: " +
            archivo.errorString();
        return false;
    }

    QJsonParseError parse;
    const auto doc = QJsonDocument::fromJson(
        archivo.readAll(), &parse
    );

    if (parse.error != QJsonParseError::NoError ||
        !doc.isObject() ||
        doc.object().value("version").toInt(-1) != 1 ||
        !doc.object().value("usuarios").isArray()) {

        error = "El archivo de usuarios no tiene "
            "un formato valido. No se sobrescribio.";
        return false;
    }

    std::vector<std::unique_ptr<Usuario>> temporal;
    QSet<int> ids;
    QSet<QString> nombres;

    int maxId = 0;
    int admins = 0;

    for (const auto& v :
        doc.object().value("usuarios").toArray()) {

        const auto o = v.toObject();
        const double numeroId =
            o.value("id").toDouble(-1);

        const auto n =
            o.value("nombre").toString().trimmed();

        const auto login =
            normalizar(o.value("usuario").toString());

        const auto hash =
            o.value("clave").toString();

        const auto rolTexto =
            o.value("rol").toString();

        if (!v.isObject() ||
            numeroId < 1 ||
            numeroId >= std::numeric_limits<int>::max() ||
            std::floor(numeroId) != numeroId ||
            n.isEmpty() ||
            login.isEmpty() ||
            !Clave::formatoValido(hash) ||
            !o.value("activo").isBool() ||
            (rolTexto != "Administrador" &&
                rolTexto != "Cajero")) {

            error = "Hay un registro de usuario "
                "invalido. No se sobrescribio el archivo.";
            return false;
        }

        const int id = static_cast<int>(numeroId);

        if (ids.contains(id) ||
            nombres.contains(login)) {
            error = "Hay usuarios o IDs duplicados.";
            return false;
        }

        ids.insert(id);
        nombres.insert(login);
        maxId = qMax(maxId, id);

        const Rol rol =
            rolTexto == "Administrador"
            ? Rol::Administrador
            : Rol::Cajero;

        bool activo = o.value("activo").toBool();

        if (activo && rol == Rol::Administrador) {
            ++admins;
        }

        temporal.push_back(
            std::make_unique<Usuario>(
                id,
                n.toStdString(),
                login.toStdString(),
                hash.toStdString(),
                rol,
                activo
            )
        );
    }

    if (temporal.empty() || admins == 0) {
        error = "El archivo existente debe tener "
            "un administrador activo.";
        return false;
    }

    usuarios.swap(temporal);
    siguienteId = maxId + 1;
    listo = true;
    return true;
}

bool GestorUsuarios::guardar() {
    QJsonArray registros;

    for (const auto& u : usuarios) {
        registros.append(QJsonObject{
            { "id", u->id },
            { "nombre", q(u->nombre) },
            { "usuario", q(u->nombreUsuario) },
            { "clave", q(u->hashContrasena) },
            { "rol", u->rol == Rol::Administrador
                ? "Administrador" : "Cajero" },
            { "activo", u->activo }
            });
    }

    if (!QDir().mkpath(
        QFileInfo(ruta).absolutePath())) {
        error = "No se pudo crear la carpeta de datos.";
        return false;
    }

    QSaveFile archivo(ruta);

    if (!archivo.open(QIODevice::WriteOnly)) {
        error = "No se pudo guardar: " +
            archivo.errorString();
        return false;
    }

    const auto datos = QJsonDocument(
        QJsonObject{
            { "version", 1 },
            { "usuarios", registros }
        }
    ).toJson();

    if (archivo.write(datos) != datos.size() ||
        !archivo.commit()) {

        error = "No se pudo confirmar el guardado: " +
            archivo.errorString();
        return false;
    }

    return true;
}

bool GestorUsuarios::autorizado(int id) {
    error.clear();

    const auto u = buscarUsuarioPorId(id);

    if (!listo || !u || !u->activo ||
        u->rol != Rol::Administrador) {
        error = "No tiene permiso de administrador.";
        return false;
    }

    return true;
}

bool GestorUsuarios::datosValidos(
    const QString& n,
    const QString& u,
    const QString& clave,
    bool nueva) {

    if (n.trimmed().isEmpty() ||
        n.size() > 120 ||
        u.trimmed().isEmpty() ||
        u.size() > 60) {

        error = "Nombre (hasta 120 caracteres) "
            "y usuario (hasta 60) son obligatorios.";
        return false;
    }

    if ((nueva || !clave.isEmpty()) &&
        (clave.size() < 8 ||
            clave.size() > 128 ||
            clave.trimmed().isEmpty())) {

        error = "La contrasena debe tener de 8 a "
            "128 caracteres y no ser solo espacios.";
        return false;
    }

    return true;
}

bool GestorUsuarios::crearAdministradorInicial(
    const QString& n,
    const QString& u,
    const QString& clave) {

    error.clear();

    if (!listo || !usuarios.empty() ||
        QFileInfo::exists(ruta)) {

        error = "La configuracion inicial ya "
            "no esta disponible.";
        return false;
    }

    if (!datosValidos(n, u, clave, true)) {
        return false;
    }

    usuarios.push_back(
        std::make_unique<Usuario>(
            1,
            n.trimmed().toStdString(),
            normalizar(u).toStdString(),
            Clave::generar(clave).toStdString(),
            Rol::Administrador
        )
    );

    if (!guardar()) {
        usuarios.clear();
        return false;
    }

    siguienteId = 2;
    return true;
}

bool GestorUsuarios::crearUsuario(
    int actor,
    const QString& n,
    const QString& u,
    const QString& clave,
    Rol rol) {

    if (!autorizado(actor) ||
        !datosValidos(n, u, clave, true)) {
        return false;
    }

    if (!rolValido(rol)) {
        error = "Rol invalido.";
        return false;
    }

    if (buscarUsuarioPorNombre(u)) {
        error = "Ya existe ese nombre de usuario.";
        return false;
    }

    if (siguienteId >=
        std::numeric_limits<int>::max()) {
        error = "Se agotaron los IDs disponibles.";
        return false;
    }

    usuarios.push_back(
        std::make_unique<Usuario>(
            siguienteId,
            n.trimmed().toStdString(),
            normalizar(u).toStdString(),
            Clave::generar(clave).toStdString(),
            rol
        )
    );

    if (!guardar()) {
        usuarios.pop_back();
        return false;
    }

    ++siguienteId;
    return true;
}

bool GestorUsuarios::modificarUsuario(
    int actor,
    int id,
    const QString& n,
    Rol rol,
    const QString& clave) {

    if (!autorizado(actor)) {
        return false;
    }

    auto u = const_cast<Usuario*>(
        buscarUsuarioPorId(id)
        );

    if (!u) {
        error = "El usuario no existe.";
        return false;
    }

    if (!datosValidos(
        n, q(u->nombreUsuario), clave, false)) {
        return false;
    }

    if (!rolValido(rol)) {
        error = "Rol invalido.";
        return false;
    }

    if (id == actor && rol != u->rol) {
        error = "No puede cambiar su propio "
            "rol durante la sesion.";
        return false;
    }

    if (u->activo &&
        u->rol == Rol::Administrador &&
        rol != u->rol &&
        administradoresActivos() <= 1) {

        error = "Debe conservar al menos un "
            "administrador activo.";
        return false;
    }

    const Usuario anterior = *u;

    u->nombre = n.trimmed().toStdString();
    u->rol = rol;

    if (!clave.isEmpty()) {
        u->hashContrasena =
            Clave::generar(clave).toStdString();
    }

    if (!guardar()) {
        *u = anterior;
        return false;
    }

    return true;
}

bool GestorUsuarios::cambiarEstado(
    int actor,
    int id,
    bool activo) {

    if (!autorizado(actor)) {
        return false;
    }

    auto u = const_cast<Usuario*>(
        buscarUsuarioPorId(id)
        );

    if (!u) {
        error = "El usuario no existe.";
        return false;
    }

    if (id == actor && !activo) {
        error = "No puede desactivar su propia sesion.";
        return false;
    }

    if (!activo &&
        u->activo &&
        u->rol == Rol::Administrador &&
        administradoresActivos() <= 1) {

        error = "Debe conservar al menos un "
            "administrador activo.";
        return false;
    }

    const bool anterior = u->activo;
    u->activo = activo;

    if (!guardar()) {
        u->activo = anterior;
        return false;
    }

    return true;
}
