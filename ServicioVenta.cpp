#include "ServicioVenta.h"
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QSaveFile>
#include <QUuid>
#include <cmath>

ServicioVenta::ServicioVenta(const QString& rutaProductos) : ruta(rutaProductos) {}

bool ServicioVenta::guardarJson(const QString& archivo, const QJsonDocument& documento, QString& error) {
    QSaveFile salida(archivo);
    if (!salida.open(QIODevice::WriteOnly)) { error = "No se puede escribir " + archivo; return false; }
    QByteArray datos = documento.toJson(QJsonDocument::Indented);
    if (salida.write(datos) != datos.size() || !salida.commit()) {
        error = "No se pudieron guardar los cambios en " + archivo;
        return false;
    }
    return true;
}

bool ServicioVenta::cargarProductos(QString& error) {
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly)) { error = "No se pudo abrir " + ruta; return false; }
    QJsonParseError fallo;
    QJsonDocument doc = QJsonDocument::fromJson(archivo.readAll(), &fallo);
    if (fallo.error != QJsonParseError::NoError || !doc.isArray()) {
        error = "El archivo de productos no contiene una lista JSON valida";
        return false;
    }
    inventario = doc.array();
    return true;
}

QJsonArray ServicioVenta::productos() const { return inventario; }
QJsonArray ServicioVenta::carrito() const { return lineas; }

double ServicioVenta::total() const {
    double resultado = 0;
    for (const QJsonValue& v : lineas) {
        QJsonObject item = v.toObject();
        resultado += item.value("precio").toDouble() * item.value("cantidad").toInt();
    }
    return resultado;
}

bool ServicioVenta::agregar(const QString& codigo, int cantidad, QString& error) {
    if (cantidad <= 0) { error = "La cantidad debe ser mayor que cero"; return false; }
    for (const QJsonValue& v : inventario) {
        QJsonObject p = v.toObject();
        if (p.value("codigo").toString() != codigo) continue;
        if (!p.value("activo").toBool(true)) { error = "Producto desactivado"; return false; }
        const double precio = p.value("precio").toDouble(-1);
        if (!std::isfinite(precio) || precio < 0) { error = "Precio invalido"; return false; }
        int cantidadActual = 0;
        for (const QJsonValue& l : lineas) {
            QJsonObject item = l.toObject();
            if (item.value("codigo").toString() == codigo) cantidadActual += item.value("cantidad").toInt();
        }
        if (cantidad + cantidadActual > p.value("existencia").toInt()) {
            error = "No hay suficientes existencias"; return false;
        }
        for (int i = 0; i < lineas.size(); ++i) {
            QJsonObject item = lineas[i].toObject();
            if (item.value("codigo").toString() == codigo) {
                item["cantidad"] = cantidadActual + cantidad;
                lineas.replace(i, item);
                return true;
            }
        }
        QJsonObject item;
        item["codigo"] = codigo;
        item["nombre"] = p.value("nombre").toString();
        item["precio"] = precio;
        item["cantidad"] = cantidad;
        lineas.append(item);
        return true;
    }
    error = "No se encontro el producto";
    return false;
}

bool ServicioVenta::quitar(const QString& codigo) {
    for (int i = 0; i < lineas.size(); ++i) {
        if (lineas[i].toObject().value("codigo").toString() == codigo) {
            lineas.removeAt(i);
            return true;
        }
    }
    return false;
}

void ServicioVenta::vaciar() { lineas = QJsonArray(); }

bool ServicioVenta::cobrar(double efectivo, double& cambio, QJsonObject& registro, QString& error) {
    if (lineas.isEmpty()) { error = "El carrito esta vacio"; return false; }
    if (!std::isfinite(efectivo) || efectivo < total()) { error = "Pago insuficiente"; return false; }
    // Releer el archivo para comprobar las existencias actuales.
    if (!cargarProductos(error)) return false;
    QJsonArray inventarioNuevo = inventario;
    for (const QJsonValue& v : lineas) {
        QJsonObject linea = v.toObject();
        bool encontrado = false;
        for (int i = 0; i < inventarioNuevo.size(); ++i) {
            QJsonObject p = inventarioNuevo[i].toObject();
            if (p.value("codigo").toString() != linea.value("codigo").toString()) continue;
            encontrado = true;
            int disponible = p.value("existencia").toInt();
            int compra = linea.value("cantidad").toInt();
            if (!p.value("activo").toBool(true) || disponible < compra) {
                error = "Sin existencias suficientes para: " + linea.value("nombre").toString();
                return false;
            }
            p["existencia"] = disponible - compra;
            inventarioNuevo.replace(i, p);
            break;
        }
        if (!encontrado) { error = "El producto ya no existe: " + linea.value("nombre").toString(); return false; }
    }
    cambio = efectivo - total();
    registro["id"] = QUuid::createUuid().toString(QUuid::WithoutBraces);
    registro["fecha"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    registro["lineas"] = lineas;
    registro["total"] = total();
    registro["efectivo"] = efectivo;
    registro["cambio"] = cambio;
    // Registro provisional separado del historial definitivo del area 3.
    QString rutaVentas = QFileInfo(ruta).dir().filePath("ventas_area2.json");
    QJsonArray ventas;
    QFile archivoVentas(rutaVentas);
    if (archivoVentas.exists()) {
        if (!archivoVentas.open(QIODevice::ReadOnly)) { error = "No se puede leer el registro de ventas"; return false; }
        QJsonDocument previo = QJsonDocument::fromJson(archivoVentas.readAll());
        if (!previo.isArray()) { error = "El archivo de ventas no es una lista JSON"; return false; }
        ventas = previo.array();
    }
    ventas.append(registro);
    if (!guardarJson(rutaVentas, QJsonDocument(ventas), error)) return false;
    if (!guardarJson(ruta, QJsonDocument(inventarioNuevo), error)) {
        error += ". ATENCION: se guardo el registro de venta, pero no se descuento inventario; revisar antes de repetir";
        return false;
    }
    inventario = inventarioNuevo;
    vaciar();
    return true;
}
