#include "AlmacenProductos.h"
#include <QFile>
#include <QSaveFile>
#include <cmath>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

bool AlmacenProductos::guardar(const std::string& ruta, const std::vector<Producto>& productos) const {
	QJsonArray lista;
	for (const Producto& p : productos) {
		QJsonObject obj;
		obj["codigo"] = QString::fromStdString(p.getCodigo());
		obj["nombre"] = QString::fromStdString(p.getNombre());
		obj["precio"] = p.getPrecio();
		obj["existencia"] = p.getExistencia();
		obj["activo"] = p.getActivo();
		lista.append(obj);
	}
	QSaveFile archivo(QString::fromStdString(ruta));
	if (!archivo.open(QIODevice::WriteOnly))
		return false;
	QByteArray datos = QJsonDocument(lista).toJson();
	if (archivo.write(datos) != datos.size()) {
		archivo.cancelWriting();
		return false;
	}
	return archivo.commit();
}

bool AlmacenProductos::cargar(const std::string& ruta, std::vector<Producto>& productos) const {
	QFile archivo(QString::fromStdString(ruta));
	if (!archivo.open(QIODevice::ReadOnly)) return false;

	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(archivo.readAll(), &error);
	if (error.error != QJsonParseError::NoError || !doc.isArray()) return false;

	std::vector<Producto> lista;
	for (const QJsonValue& valor : doc.array()) {
		if (!valor.isObject()) return false;
		QJsonObject obj = valor.toObject();
		if (!obj["codigo"].isString() || !obj["nombre"].isString() ||
			!obj["precio"].isDouble() || !obj["existencia"].isDouble() ||
			!obj["activo"].isBool()) return false;

		double precio = obj["precio"].toDouble();
		double unidades = obj["existencia"].toDouble();
		if (!std::isfinite(precio) || precio <= 0 ||
			unidades < 0 || unidades > 2147483647.0 || std::floor(unidades) != unidades)
			return false;
		Producto p(obj["codigo"].toString().toStdString(),
			obj["nombre"].toString().toStdString(),
			precio, static_cast<int>(unidades));
		if (!obj["activo"].toBool()) p.desactivar();
		lista.push_back(p);
	}
	productos = lista;
	return true;
}
