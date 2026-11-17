#ifndef NAVEGADORPANTALLAS_H
#define NAVEGADORPANTALLAS_H

#include <QString>
#include <QWidget>
#include <QStackedWidget>
#include <QMap>

enum class Rol {
    Administrador,
    Cajero,
    Ninguno
};

class NavegadorPantallas {
private:
    QStackedWidget* contenedor;
    QMap<QString, QWidget*> vistas;
    QMap<QString, Rol> permisos;
    Rol rolActualUsuario;

public:
    NavegadorPantallas(QStackedWidget* contenedor);
    void registrarVista(const QString& nombre, QWidget* vista, Rol rolMinimo);
    void setRolActual(Rol rol);
    void mostrarVista(const QString& nombre);
};

#endif // NAVEGADORPANTALLAS_H
