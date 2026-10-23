#include "VentanaProductos.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QString>

VentanaProductos::VentanaProductos(const std::string& ruta) : rutaArchivo(ruta) {
	crearPantalla();
	cargarDatos();
	mostrarTabla();
}

void VentanaProductos::crearPantalla() {
	setWindowTitle("La Pulpe - Productos e inventario");
	resize(750, 560);

	txtBuscar = new QLineEdit;
	txtBuscar->setPlaceholderText("Buscar por codigo o nombre");

	tabla = new QTableWidget;
	tabla->setColumnCount(5);
	tabla->setHorizontalHeaderLabels({ "Codigo", "Nombre", "Precio", "Existencia", "Estado" });
	tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
	tabla->setSelectionMode(QAbstractItemView::SingleSelection);
	tabla->horizontalHeader()->setStretchLastSection(true);
	tabla->setColumnWidth(1, 250);

	txtCodigo = new QLineEdit;
	txtNombre = new QLineEdit;
	txtPrecio = new QLineEdit;
	txtExistencia = new QLineEdit;
	txtCantidad = new QLineEdit;

	QFormLayout* formulario = new QFormLayout;
	formulario->addRow("Codigo:", txtCodigo);
	formulario->addRow("Nombre:", txtNombre);
	formulario->addRow("Precio:", txtPrecio);
	formulario->addRow("Existencia inicial:", txtExistencia);
	formulario->addRow("Cantidad a ingresar:", txtCantidad);

	QPushButton* btnAgregar = new QPushButton("Agregar");
	QPushButton* btnModificar = new QPushButton("Modificar");
	QPushButton* btnIngresar = new QPushButton("Ingresar mercaderia");
	QPushButton* btnDesactivar = new QPushButton("Desactivar");
	QPushButton* btnLimpiar = new QPushButton("Limpiar");

	QHBoxLayout* botones = new QHBoxLayout;
	botones->addWidget(btnAgregar);
	botones->addWidget(btnModificar);
	botones->addWidget(btnIngresar);
	botones->addWidget(btnDesactivar);
	botones->addWidget(btnLimpiar);

	lblMensaje = new QLabel;

	QVBoxLayout* principal = new QVBoxLayout(this);
	principal->addWidget(txtBuscar);
	principal->addWidget(tabla);
	principal->addLayout(formulario);
	principal->addLayout(botones);
	principal->addWidget(lblMensaje);

	connect(txtBuscar, &QLineEdit::textChanged, this, [this]() { mostrarTabla(); });
	connect(tabla, &QTableWidget::cellClicked, this, [this](int fila, int) { seleccionarFila(fila); });
	connect(btnAgregar, &QPushButton::clicked, this, [this]() { agregarProducto(); });
	connect(btnModificar, &QPushButton::clicked, this, [this]() { modificarProducto(); });
	connect(btnIngresar, &QPushButton::clicked, this, [this]() { ingresarMercaderia(); });
	connect(btnDesactivar, &QPushButton::clicked, this, [this]() { desactivarProducto(); });
	connect(btnLimpiar, &QPushButton::clicked, this, [this]() { limpiarFormulario(); });
}

void VentanaProductos::cargarDatos() {
	if (!QFile::exists(QString::fromStdString(rutaArchivo))) {
		mostrarMensaje("No hay archivo de productos, se inicia con la lista vacia.");
		return;
	}
	std::vector<Producto> lista;
	if (!almacen.cargar(rutaArchivo, lista)) {
		mostrarError("No se pudo leer el archivo de productos.");
		return;
	}
	gestor.setProductos(lista);
}

void VentanaProductos::guardarDatos() {
	if (!almacen.guardar(rutaArchivo, gestor.getProductos())) {
		mostrarError("No se pudo guardar el archivo de productos.");
	}
}

void VentanaProductos::mostrarTabla() {
	QString filtro = txtBuscar->text().trimmed().toLower();
	tabla->setRowCount(0);

	for (const Producto& p : gestor.getProductos()) {
		QString codigo = QString::fromStdString(p.getCodigo());
		QString nombre = QString::fromStdString(p.getNombre());
		if (!filtro.isEmpty() && !codigo.toLower().contains(filtro) && !nombre.toLower().contains(filtro))
			continue;

		int fila = tabla->rowCount();
		tabla->insertRow(fila);
		tabla->setItem(fila, 0, new QTableWidgetItem(codigo));
		tabla->setItem(fila, 1, new QTableWidgetItem(nombre));
		tabla->setItem(fila, 2, new QTableWidgetItem(QString::number(p.getPrecio(), 'f', 2)));
		tabla->setItem(fila, 3, new QTableWidgetItem(QString::number(p.getExistencia())));
		tabla->setItem(fila, 4, new QTableWidgetItem(p.getActivo() ? "Activo" : "Inactivo"));
	}
}

void VentanaProductos::limpiarFormulario() {
	txtCodigo->clear();
	txtNombre->clear();
	txtPrecio->clear();
	txtExistencia->clear();
	txtCantidad->clear();
	lblMensaje->clear();
	tabla->clearSelection();
}

void VentanaProductos::seleccionarFila(int fila) {
	txtCodigo->setText(tabla->item(fila, 0)->text());
	txtNombre->setText(tabla->item(fila, 1)->text());
	txtPrecio->setText(tabla->item(fila, 2)->text());
	txtExistencia->clear();
	txtCantidad->clear();
	lblMensaje->clear();
}

void VentanaProductos::mostrarError(const QString& texto) {
	lblMensaje->setStyleSheet("color: red;");
	lblMensaje->setText(texto);
	QMessageBox::warning(this, "Error", texto);
}

void VentanaProductos::mostrarMensaje(const QString& texto) {
	lblMensaje->setStyleSheet("color: green;");
	lblMensaje->setText(texto);
}

Producto* VentanaProductos::productoActivoDelFormulario() {
	std::string codigo = txtCodigo->text().trimmed().toStdString();
	if (codigo.empty()) {
		mostrarError("Escriba o seleccione el codigo del producto.");
		return nullptr;
	}
	Producto* p = gestor.buscarPorCod(codigo);
	if (p == nullptr) {
		mostrarError("No existe un producto con ese codigo.");
		return nullptr;
	}
	if (!p->getActivo()) {
		mostrarError("El producto esta inactivo.");
		return nullptr;
	}
	return p;
}

void VentanaProductos::agregarProducto() {
	std::string codigo = txtCodigo->text().trimmed().toStdString();
	std::string nombre = txtNombre->text().trimmed().toStdString();
	if (codigo.empty() || nombre.empty()) {
		mostrarError("El codigo y el nombre son obligatorios.");
		return;
	}

	bool precioOk = false;
	bool existenciaOk = false;
	double precio = txtPrecio->text().toDouble(&precioOk);
	int existencia = txtExistencia->text().toInt(&existenciaOk);
	if (!precioOk || precio <= 0) {
		mostrarError("El precio debe ser un numero mayor que cero.");
		return;
	}
	if (!existenciaOk || existencia < 0) {
		mostrarError("La existencia debe ser un numero entero, cero o mayor.");
		return;
	}
	if (gestor.buscarPorCod(codigo) != nullptr) {
		mostrarError("Ya existe un producto con ese codigo.");
		return;
	}

	if (!gestor.agregar(Producto(codigo, nombre, precio, existencia))) {
		mostrarError("No se pudo agregar el producto.");
		return;
	}
	guardarDatos();
	mostrarTabla();
	mostrarMensaje("Producto agregado.");
}

void VentanaProductos::modificarProducto() {
	Producto* p = productoActivoDelFormulario();
	if (p == nullptr) return;

	std::string nombre = txtNombre->text().trimmed().toStdString();
	bool precioOk = false;
	double precio = txtPrecio->text().toDouble(&precioOk);
	if (nombre.empty()) {
		mostrarError("El nombre no puede estar vacio.");
		return;
	}
	if (!precioOk || precio <= 0) {
		mostrarError("El precio debe ser un numero mayor que cero.");
		return;
	}

	std::string codigo = p->getCodigo();
	gestor.modificarNombre(codigo, nombre);
	gestor.modificarPrecio(codigo, precio);
	guardarDatos();
	mostrarTabla();
	mostrarMensaje("Producto modificado.");
}

void VentanaProductos::ingresarMercaderia() {
	Producto* p = productoActivoDelFormulario();
	if (p == nullptr) return;

	bool cantidadOk = false;
	int cantidad = txtCantidad->text().toInt(&cantidadOk);
	if (!cantidadOk || cantidad <= 0) {
		mostrarError("La cantidad debe ser un numero entero mayor que cero.");
		return;
	}

	gestor.ingresaMerca(p->getCodigo(), cantidad);
	guardarDatos();
	mostrarTabla();
	txtCantidad->clear();
	mostrarMensaje("Mercaderia ingresada.");
}

void VentanaProductos::desactivarProducto() {
	Producto* p = productoActivoDelFormulario();
	if (p == nullptr) return;

	QMessageBox::StandardButton respuesta = QMessageBox::question(
		this, "Desactivar", "Desea desactivar el producto " + QString::fromStdString(p->getNombre()) + "?");
	if (respuesta != QMessageBox::Yes) return;

	gestor.desactivar(p->getCodigo());
	guardarDatos();
	mostrarTabla();
	mostrarMensaje("Producto desactivado.");
}
