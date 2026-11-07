#pragma once
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

// Area 2: logica de ventas. NO contiene main ni depende de las ventanas.
class ServicioVenta {
public:
    explicit ServicioVenta(const QString& rutaProductos);
    bool cargarProductos(QString& error);
    QJsonArray productos() const;
    QJsonArray carrito() const;
    bool agregar(const QString& codigo, int cantidad, QString& error);
    bool quitar(const QString& codigo);
    void vaciar();
    double total() const;
    bool cobrar(double efectivo, double& cambio, QJsonObject& registro, QString& error);

private:
    QString ruta;
    QJsonArray inventario;
    QJsonArray lineas;
    static bool guardarJson(const QString& archivo, const QJsonDocument& documento, QString& error);
};
