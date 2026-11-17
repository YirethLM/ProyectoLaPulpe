
#include <QApplication>
#include "ModuloUsuarios/AplicacionUsuarios.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setOrganizationName("LaPulpe");
    app.setApplicationName("LaPulpePruebaUsuarios");
    app.setQuitOnLastWindowClosed(false);

    return ejecutarLaPulpe({});
}
