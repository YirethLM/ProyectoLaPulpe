
#include <QApplication>
#include "LaPulpe_productos/VentanaProductos.h"
#include "ModuloUsuarios/AplicacionUsuarios.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setOrganizationName("LaPulpe");
    app.setApplicationName("LaPulpe");
    app.setQuitOnLastWindowClosed(false);

    return ejecutarLaPulpe([]() -> QWidget* {
        return new VentanaProductos("productos.json");
        });
}
