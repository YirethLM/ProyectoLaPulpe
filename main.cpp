#include <QApplication>
#include "LaPulpe_productos/VentanaProductos.h"

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	VentanaProductos ventana("productos.json");
	ventana.show();

	return app.exec();
}
