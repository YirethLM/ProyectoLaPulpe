#include "ServicioVenta.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDateTime>
#include <QUuid>
#include <QString>
#include <QByteArray>

#include <cmath>

// ==========================================
// CONSTRUCTOR
// ==========================================

ServicioVenta::ServicioVenta(
    const QString& rutaProductos)
    : ruta(rutaProductos)
{
    QString error;
    cargarProductos(error);
}

// ==========================================
// GUARDAR ARCHIVOS JSON
// ==========================================

bool ServicioVenta::guardarJson(
    const QString& archivo,
    const QJsonDocument& documento,
    QString& error)
{
    QSaveFile salida(archivo);

    // Permitir escritura directa si falla
    // el reemplazo seguro del archivo.
    salida.setDirectWriteFallback(true);

    if (!salida.open(QIODevice::WriteOnly)) {
        error = "No se puede abrir el archivo:\n"
            + archivo
            + "\n\nMotivo: "
            + salida.errorString();
        return false;
    }

    QByteArray datos =
        documento.toJson(QJsonDocument::Indented);

    if (salida.write(datos) != datos.size()) {
        error = "No se pudo escribir el archivo:\n"
            + archivo
            + "\n\nMotivo: "
            + salida.errorString();

        salida.cancelWriting();
        return false;
    }

    if (!salida.commit()) {
        error = "No se pudo guardar el archivo:\n"
            + archivo
            + "\n\nMotivo: "
            + salida.errorString();
        return false;
    }

    return true;
}

// ==========================================
// CARGAR PRODUCTOS
// ==========================================

bool ServicioVenta::cargarProductos(
    QString& error)
{
    QFile archivo(ruta);

    if (!archivo.open(QIODevice::ReadOnly)) {
        error = "No se pudo abrir el inventario:\n"
            + ruta;
        return false;
    }

    QJsonParseError fallo;

    QJsonDocument doc =
        QJsonDocument::fromJson(
            archivo.readAll(),
            &fallo
        );

    archivo.close();

    if (fallo.error != QJsonParseError::NoError
        || !doc.isArray()) {

        error = "El archivo de productos "
            "no contiene una lista JSON valida";
        return false;
    }

    inventario = doc.array();
    return true;
}

// ==========================================
// CONSULTAR PRODUCTOS
// ==========================================

QJsonArray ServicioVenta::productos() const
{
    return inventario;
}

// ==========================================
// CONSULTAR CARRITO
// ==========================================

QJsonArray ServicioVenta::carrito() const
{
    return lineas;
}

// ==========================================
// CALCULAR TOTAL
// ==========================================

double ServicioVenta::total() const
{
    double resultado = 0;

    for (const QJsonValue& v : lineas) {

        QJsonObject item = v.toObject();

        double precio =
            item.value("precio").toDouble();

        int cantidad =
            item.value("cantidad").toInt();

        resultado += precio * cantidad;
    }

    return resultado;
}

// ==========================================
// AGREGAR PRODUCTO AL CARRITO
// ==========================================

bool ServicioVenta::agregar(
    const QString& codigo,
    int cantidad,
    QString& error)
{
    if (cantidad <= 0) {
        error = "La cantidad debe ser mayor que cero";
        return false;
    }

    for (const QJsonValue& v : inventario) {

        QJsonObject p = v.toObject();

        if (p.value("codigo").toString() != codigo)
            continue;

        if (!p.value("activo").toBool(true)) {
            error = "Producto desactivado";
            return false;
        }

        double precio =
            p.value("precio").toDouble(-1);

        if (!std::isfinite(precio) || precio < 0) {
            error = "Precio invalido";
            return false;
        }

        int cantidadActual = 0;

        for (const QJsonValue& l : lineas) {

            QJsonObject item = l.toObject();

            if (item.value("codigo").toString()
                == codigo) {

                cantidadActual +=
                    item.value("cantidad").toInt();
            }
        }

        int existencia =
            p.value("existencia").toInt();

        if (cantidad + cantidadActual > existencia) {
            error = "No hay suficientes existencias";
            return false;
        }

        // Si ya existe, aumentar cantidad
        for (int i = 0; i < lineas.size(); ++i) {

            QJsonObject item =
                lineas[i].toObject();

            if (item.value("codigo").toString()
                == codigo) {

                item["cantidad"] =
                    cantidadActual + cantidad;

                lineas.replace(i, item);
                return true;
            }
        }

        // Agregar nuevo producto
        QJsonObject item;

        item["codigo"] = codigo;

        item["nombre"] =
            p.value("nombre").toString();

        item["precio"] = precio;

        item["cantidad"] = cantidad;

        lineas.append(item);

        return true;
    }

    error = "No se encontro el producto";
    return false;
}

// ==========================================
// QUITAR PRODUCTO DEL CARRITO
// ==========================================

bool ServicioVenta::quitar(
    const QString& codigo)
{
    for (int i = 0; i < lineas.size(); ++i) {

        QJsonObject item =
            lineas[i].toObject();

        if (item.value("codigo").toString()
            == codigo) {

            lineas.removeAt(i);
            return true;
        }
    }

    return false;
}

// ==========================================
// VACIAR CARRITO
// ==========================================

void ServicioVenta::vaciar()
{
    lineas = QJsonArray();
}

// ==========================================
// REALIZAR COBRO
// ==========================================

bool ServicioVenta::cobrar(
    double efectivo,
    double& cambio,
    QJsonObject& registro,
    QString& error)
{
    // Comprobar que haya productos
    if (lineas.isEmpty()) {
        error = "El carrito esta vacio";
        return false;
    }

    // Comprobar dinero recibido
    if (!std::isfinite(efectivo)
        || efectivo < total()) {

        error = "Pago insuficiente";
        return false;
    }

    // Recargar inventario actualizado
    if (!cargarProductos(error))
        return false;

    QJsonArray inventarioNuevo = inventario;

    // ======================================
    // VALIDAR Y DESCONTAR EXISTENCIAS
    // ======================================

    for (const QJsonValue& v : lineas) {

        QJsonObject linea = v.toObject();

        bool encontrado = false;

        for (int i = 0;
            i < inventarioNuevo.size();
            ++i) {

            QJsonObject p =
                inventarioNuevo[i].toObject();

            if (p.value("codigo").toString()
                != linea.value("codigo").toString()) {

                continue;
            }

            encontrado = true;

            int disponible =
                p.value("existencia").toInt();

            int compra =
                linea.value("cantidad").toInt();

            if (!p.value("activo").toBool(true)
                || disponible < compra) {

                error =
                    "Sin existencias suficientes para: "
                    + linea.value("nombre").toString();

                return false;
            }

            p["existencia"] =
                disponible - compra;

            inventarioNuevo.replace(i, p);

            break;
        }

        if (!encontrado) {

            error = "El producto ya no existe: "
                + linea.value("nombre").toString();

            return false;
        }
    }

    // ======================================
    // CALCULAR CAMBIO
    // ======================================

    cambio = efectivo - total();

    // ======================================
    // CREAR REGISTRO DE VENTA
    // ======================================

    registro = QJsonObject();

    registro["id"] =
        QUuid::createUuid().toString(
            QUuid::WithoutBraces
        );

    registro["fecha"] =
        QDateTime::currentDateTime().toString(
            Qt::ISODate
        );

    registro["lineas"] = lineas;
    registro["total"] = total();
    registro["efectivo"] = efectivo;
    registro["cambio"] = cambio;

    // ======================================
    // ARCHIVO DEL HISTORIAL DE VENTAS
    // ======================================

    QString rutaVentas =
        QFileInfo(ruta)
        .dir()
        .filePath("ventas_area2.json");

    QJsonArray ventas;

    QFile archivoVentas(rutaVentas);

    // Cargar las ventas anteriores
    if (archivoVentas.exists()) {

        if (!archivoVentas.open(
            QIODevice::ReadOnly)) {

            error =
                "No se puede leer el registro de ventas";
            return false;
        }

        QJsonParseError falloVentas;

        QJsonDocument previo =
            QJsonDocument::fromJson(
                archivoVentas.readAll(),
                &falloVentas
            );

        archivoVentas.close();

        if (falloVentas.error !=
            QJsonParseError::NoError
            || !previo.isArray()) {

            error =
                "El archivo de ventas "
                "no es una lista JSON valida";

            return false;
        }

        ventas = previo.array();
    }

    // Agregar la nueva venta
    ventas.append(registro);

    // ======================================
    // GUARDAR REGISTRO DE VENTAS
    // ======================================

    if (!guardarJson(
        rutaVentas,
        QJsonDocument(ventas),
        error)) {

        return false;
    }

    // ======================================
    // GUARDAR INVENTARIO ACTUALIZADO
    // ======================================

    if (!guardarJson(
        ruta,
        QJsonDocument(inventarioNuevo),
        error)) {

        error +=
            "\n\nATENCION: Se guardo el registro "
            "de venta, pero no se pudo actualizar "
            "el inventario. Revise ambos archivos "
            "antes de repetir el cobro.";

        return false;
    }

    // ======================================
    // ACTUALIZAR SISTEMA
    // ======================================

    inventario = inventarioNuevo;

    // Vaciar carrito despues del cobro exitoso
    vaciar();

    return true;
}
