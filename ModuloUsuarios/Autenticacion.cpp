
#include "Autenticacion.h"
#include "Clave.h"

Autenticacion::Autenticacion(GestorUsuarios& g)
    : gestor(g) {
}

const Usuario* Autenticacion::iniciarSesion(
    const QString& nombre,
    const QString& clave) const {

    if (clave.size() > 128) {
        return nullptr;
    }

    const auto u =
        gestor.buscarUsuarioPorNombre(nombre);

    if (!u ||
        !u->estaActivo() ||
        !Clave::verificar(
            clave,
            QString::fromStdString(
                u->obtenerHashContrasena()
            ))) {
        return nullptr;
    }

    return u;
}

bool Autenticacion::tienePermiso(
    const Usuario& usuario,
    Rol requerido) const {

    const auto u =
        gestor.buscarUsuarioPorId(usuario.obtenerId());

    return u &&
        u->estaActivo() &&
        (u->obtenerRol() == Rol::Administrador ||
            u->obtenerRol() == requerido);
}
