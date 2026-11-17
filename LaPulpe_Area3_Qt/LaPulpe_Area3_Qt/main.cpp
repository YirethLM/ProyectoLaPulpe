#include <QApplication>
#include <QStackedWidget>
#include <QMessageBox>
#include "VentanaPrincipal.h"
#include "NavegadorPantallas.h"
#include "VistaInicio.h"
#include "VistaSeccion.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QStackedWidget* contenedorPrincipal = new QStackedWidget();
    NavegadorPantallas* navegador = new NavegadorPantallas(contenedorPrincipal);

    // Instanciación de vistas
    VistaInicio* vistaInicio = new VistaInicio();
    VistaSeccion* vistaVentas = new VistaSeccion("Módulo de Ventas");
    VistaSeccion* vistaInventario = new VistaSeccion("Gestión de Inventario");
    VistaSeccion* vistaHistorial = new VistaSeccion("Historial de Transacciones");
    VistaSeccion* vistaUsuarios = new VistaSeccion("Control de Usuarios");

    // Registrar vistas
    navegador->registrarVista("Inicio", vistaInicio, Rol::Ninguno);
    navegador->registrarVista("Ventas", vistaVentas, Rol::Cajero);
    navegador->registrarVista("Inventario", vistaInventario, Rol::Administrador);
    navegador->registrarVista("Historial", vistaHistorial, Rol::Administrador);
    navegador->registrarVista("Usuarios", vistaUsuarios, Rol::Administrador);

    // Conexiones de Navegación
    QObject::connect(vistaInicio, &VistaInicio::navegarASolicitado, [navegador](const QString& destino) {
        navegador->mostrarVista(destino);
    });

    QObject::connect(vistaInicio, &VistaInicio::cerrarSesionSolicitado, []() {
        QMessageBox::information(nullptr, "Cerrar sesión", "Simulando cierre de sesión...");
    });

    auto volverAlInicio = [navegador]() { navegador->mostrarVista("Inicio"); };
    QObject::connect(vistaVentas, &VistaSeccion::volverSolicitado, volverAlInicio);
    QObject::connect(vistaInventario, &VistaSeccion::volverSolicitado, volverAlInicio);
    QObject::connect(vistaHistorial, &VistaSeccion::volverSolicitado, volverAlInicio);
    QObject::connect(vistaUsuarios, &VistaSeccion::volverSolicitado, volverAlInicio);

    // Inicio
    navegador->setRolActual(Rol::Administrador);
    navegador->mostrarVista("Inicio");

    VentanaPrincipal ventana(navegador);
    ventana.setCentralWidget(contenedorPrincipal);
    ventana.show();

    return app.exec();
}
