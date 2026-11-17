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

    // Acceso total para Administrador
    if (rolActualUsuario == Rol::Administrador) {
        contenedor->setCurrentWidget(vistas[nombre]);
        return;
    }
    
    // Acceso para Cajero
    if (rolActualUsuario == Rol::Cajero) {
        if (nombre == "Ventas" || nombre == "Historial" || nombre == "Inicio") {
            contenedor->setCurrentWidget(vistas[nombre]);
            return;
        }
    }

    // Acceso público (Inicio)
    if (rolRequerido == Rol::Ninguno) {
        contenedor->setCurrentWidget(vistas[nombre]);
        return;
    }

    QMessageBox::warning(contenedor, "Acceso Denegado", "No tiene permisos para acceder a esta pantalla.");
}
