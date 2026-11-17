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

inline QString rolToString(Rol rol) {
    switch(rol) {
        case Rol::Administrador: return "Administrador";
        case Rol::Cajero: return "Cajero";
        default: return "Ninguno";
    }
}

inline Rol stringToRol(const QString& rol) {
    QString r = rol.trimmed().toLower();
    if (r == "administrador" || r == "admin") return Rol::Administrador;
    if (r == "cajero") return Rol::Cajero;
    return Rol::Ninguno;
}

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
