#include "VentanaHistorial.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VentanaHistorial window;
    window.show();
    return app.exec();
}
