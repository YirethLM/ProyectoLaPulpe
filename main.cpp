
#include <QApplication>
#include "LaPulpe_productos/VentanaProductos.h"
#include "VentanaVentas.h"
#include "InicioLaPulpe.h"
#include "ModuloUsuarios/AplicacionUsuarios.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setOrganizationName("LaPulpe");
    app.setApplicationName("LaPulpe");
    app.setQuitOnLastWindowClosed(false);

    return ejecutarLaPulpe([]() -> QWidget* {
        return crearPantallaInicio(
            []() {
                auto* productos = new VentanaProductos("productos.json");
                productos->show();
            },
            []() {
                auto* ventas = new VentanaVentas("productos.json");
                ventas->show();
            }
        );
    });
}
