#include <QApplication>
#include <QStackedWidget>
#include <QMessageBox>
#include "VentanaPrincipal.h"
#include "NavegadorPantallas.h"
#include "VistaInicio.h"
#include "VentanaHistorial.h"
#include "VistaInventario.h"
#include "VistaVentas.h"
#include "VistaLogin.h"
#include "VistaUsuarios.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Contenedor principal para Login y VentanaPrincipal
    QStackedWidget* stackPrincipal = new QStackedWidget();

    // 1. Módulo de Navegación interno de la Ventana Principal
    QStackedWidget* contenedorInterno = new QStackedWidget();
    NavegadorPantallas* navegador = new NavegadorPantallas(contenedorInterno);

    // Instanciación de vistas
    VistaInicio* vistaInicio = new VistaInicio();
    VentanaHistorial* vistaHistorial = new VentanaHistorial();
    VistaInventario* vistaInventario = new VistaInventario();
    VistaVentas* vistaVentas = new VistaVentas();
    VistaUsuarios* vistaUsuarios = new VistaUsuarios();

    // Registrar vistas
    navegador->registrarVista("Inicio", vistaInicio, Rol::Ninguno);
    navegador->registrarVista("Ventas", vistaVentas, Rol::Cajero);
    navegador->registrarVista("Inventario", vistaInventario, Rol::Administrador);
    navegador->registrarVista("Historial", vistaHistorial, Rol::Cajero);
    navegador->registrarVista("Usuarios", vistaUsuarios, Rol::Administrador);

    // Conexiones de Navegación interna
    QObject::connect(vistaInicio, &VistaInicio::navegarASolicitado, [navegador](const QString& destino) {
        navegador->mostrarVista(destino);
    });

    auto volverAlInicio = [navegador]() { navegador->mostrarVista("Inicio"); };
    QObject::connect(vistaVentas, &VistaVentas::volverSolicitado, volverAlInicio);
    QObject::connect(vistaInventario, &VistaInventario::volverSolicitado, volverAlInicio);
    QObject::connect(vistaHistorial, &VentanaHistorial::volverSolicitado, volverAlInicio);
    QObject::connect(vistaUsuarios, &VistaUsuarios::volverSolicitado, volverAlInicio);

    // 2. Ventana Principal (se muestra tras login)
    VentanaPrincipal* ventanaPrincipal = new VentanaPrincipal(navegador);
    ventanaPrincipal->setCentralWidget(contenedorInterno);

    // 3. Login
    VistaLogin* login = new VistaLogin();
    
    QObject::connect(login, &VistaLogin::loginExitoso, [&](const QString& nombre, const QString& rol) {
        navegador->setRolActual(stringToRol(rol));
        vistaInicio->actualizarRol(rol, nombre);
        
        navegador->mostrarVista("Inicio");
        stackPrincipal->setCurrentWidget(ventanaPrincipal);
    });

    QObject::connect(vistaInicio, &VistaInicio::cerrarSesionSolicitado, [&]() {
        stackPrincipal->setCurrentWidget(login);
    });

    stackPrincipal->addWidget(login);
    stackPrincipal->addWidget(ventanaPrincipal);
    stackPrincipal->setCurrentWidget(login);
    stackPrincipal->show();

    return app.exec();
}
