#pragma once
#include <string>
#include <QWidget>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "GestorProductos.h"
#include "AlmacenProductos.h"

// Pantalla de productos e inventario. Solo maneja la interfaz:
// las reglas estan en GestorProductos y los archivos en AlmacenProductos.
class VentanaProductos : public QWidget {
private:
	GestorProductos gestor;
	AlmacenProductos almacen;
	std::string rutaArchivo;

	QLineEdit* txtBuscar;
	QLineEdit* txtCodigo;
	QLineEdit* txtNombre;
	QLineEdit* txtPrecio;
	QLineEdit* txtExistencia;
	QLineEdit* txtCantidad;
	QTableWidget* tabla;
	QLabel* lblMensaje;

	void crearPantalla();
	void cargarDatos();
	void guardarDatos();
	void mostrarTabla();
	void limpiarFormulario();
	void seleccionarFila(int fila);
	void mostrarError(const QString& texto);
	void mostrarMensaje(const QString& texto);
	Producto* productoActivoDelFormulario();

	void agregarProducto();
	void modificarProducto();
	void ingresarMercaderia();
	void desactivarProducto();

public:
	VentanaProductos(const std::string& ruta);
};
