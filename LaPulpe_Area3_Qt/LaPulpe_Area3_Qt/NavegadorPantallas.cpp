#include "NavegadorPantallas.h"
#include <QMessageBox>

NavegadorPantallas::NavegadorPantallas(QStackedWidget* contenedor) 
    : contenedor(contenedor), rolActualUsuario(Rol::Ninguno) {}

void NavegadorPantallas::registrarVista(const QString& nombre, QWidget* vista, Rol rolMinimo) {
    if (!vistas.contains(nombre)) {
        vistas[nombre] = vista;
        permisos[nombre] = rolMinimo;
        contenedor->addWidget(vista);
    }
}

void NavegadorPantallas::setRolActual(Rol rol) {
    rolActualUsuario = rol;
}

void NavegadorPantallas::mostrarVista(const QString& nombre) {
    if (!vistas.contains(nombre)) return;

    Rol rolRequerido = permisos[nombre];

    // Lógica básica de permisos
    if (rolRequerido == Rol::Ninguno || rolActualUsuario == Rol::Administrador || rolActualUsuario == rolRequerido) {
        contenedor->setCurrentWidget(vistas[nombre]);
    } else {
        QMessageBox::warning(contenedor, "Acceso Denegado", "No tiene permisos para acceder a esta pantalla.");
    }
}
