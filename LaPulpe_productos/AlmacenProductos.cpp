#include "AlmacenProductos.h"
#include <QFile>
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
	QFile archivo(QString::fromStdString(ruta));
	if (!archivo.open(QIODevice::WriteOnly))
		return false;
	QByteArray datos = QJsonDocument(lista).toJson();
	bool escrito = archivo.write(datos) == datos.size();
	archivo.close();
	return escrito;
}

bool AlmacenProductos::cargar(const std::string& ruta, std::vector<Producto>& productos) const {
	QFile archivo(QString::fromStdString(ruta));
	if (!archivo.open(QIODevice::ReadOnly)) return false;

	QJsonDocument doc = QJsonDocument::fromJson(archivo.readAll());
	if (!doc.isArray()) return false;

	productos.clear();
	for (const QJsonValue& valor : doc.array()) {
		QJsonObject obj = valor.toObject();
		Producto p(obj["codigo"].toString().toStdString(),
			obj["nombre"].toString().toStdString(),
			obj["precio"].toDouble(),
			obj["existencia"].toInt());
		if (!obj["activo"].toBool(true)) p.desactivar();
		productos.push_back(p);
	}
	return true;
}
