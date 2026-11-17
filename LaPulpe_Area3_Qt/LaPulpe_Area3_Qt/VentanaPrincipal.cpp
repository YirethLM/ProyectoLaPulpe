#include "VentanaPrincipal.h"
#include "Estilos.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>

VentanaPrincipal::VentanaPrincipal(NavegadorPantallas* nav, QWidget* parent)
    : QMainWindow(parent), navegador(nav) {
    
    setMinimumSize(1200, 800);
    setWindowTitle("La Pulpe - Administración de pulpería");
    setStyleSheet(Estilos::QSS_MAIN);
    configurarMenu();
}

void VentanaPrincipal::configurarMenu() {
    QMenuBar* barraMenu = menuBar();

    // Menú Archivo
    QMenu* menuArchivo = barraMenu->addMenu("Archivo");
    QAction* accionSalir = menuArchivo->addAction("Salir");
    connect(accionSalir, &QAction::triggered, this, &QWidget::close);

    // Menú Navegación (Ejemplo)
    QMenu* menuIr = barraMenu->addMenu("Navegación");
    
    QAction* accionInicio = menuIr->addAction("Inicio");
    connect(accionInicio, &QAction::triggered, [this]() {
        navegador->mostrarVista("Inicio");
    });

    QAction* accionVentas = menuIr->addAction("Ventas");
    connect(accionVentas, &QAction::triggered, [this]() {
        navegador->mostrarVista("Ventas");
    });

    QAction* accionInventario = menuIr->addAction("Inventario");
    connect(accionInventario, &QAction::triggered, [this]() {
        navegador->mostrarVista("Inventario");
    });
}
