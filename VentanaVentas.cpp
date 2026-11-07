#include "VentanaVentas.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFont>
#include <QStringList>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>

VentanaVentas::VentanaVentas(const QString& rutaProductos, QWidget* parent)
    : QWidget(parent), servicio(rutaProductos) {
    setWindowTitle("La Pulpe - Ventas y cobro (Area 2)");
    resize(750, 480);
    auto* principal = new QVBoxLayout(this);
    auto* titulo = new QLabel("VENTAS Y COBRO");
    QFont fuente = titulo->font(); fuente.setPointSize(16); fuente.setBold(true);
    titulo->setFont(fuente);
    principal->addWidget(titulo);

    auto* controles = new QHBoxLayout();
    productosCombo = new QComboBox(this);
    cantidad = new QSpinBox(this);
    cantidad->setRange(1, 100000);
    auto* botonAgregar = new QPushButton("Agregar al carrito", this);
    auto* botonActualizar = new QPushButton("Recargar productos", this);
    controles->addWidget(new QLabel("Producto:"));
    controles->addWidget(productosCombo, 1);
    controles->addWidget(new QLabel("Cantidad:"));
    controles->addWidget(cantidad);
    controles->addWidget(botonAgregar);
    controles->addWidget(botonActualizar);
    principal->addLayout(controles);

    tabla = new QTableWidget(0, 5, this);
    tabla->setHorizontalHeaderLabels({"Codigo", "Producto", "Precio", "Cantidad", "Subtotal"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    principal->addWidget(tabla);

    auto* botonQuitar = new QPushButton("Quitar producto seleccionado", this);
    principal->addWidget(botonQuitar);
    totalLabel = new QLabel("Total: ₡0.00", this);
    principal->addWidget(totalLabel);
    auto* cobro = new QHBoxLayout();
    efectivo = new QDoubleSpinBox(this);
    efectivo->setRange(0, 1000000000);
    efectivo->setDecimals(2);
    efectivo->setPrefix("₡");
    auto* botonCobrar = new QPushButton("Cobrar", this);
    cobro->addWidget(new QLabel("Efectivo recibido:"));
    cobro->addWidget(efectivo);
    cobro->addWidget(botonCobrar);
    principal->addLayout(cobro);

    connect(botonAgregar, &QPushButton::clicked, this, [this]() { agregarProducto(); });
    connect(botonActualizar, &QPushButton::clicked, this, [this]() { actualizarProductos(); });
    connect(botonQuitar, &QPushButton::clicked, this, [this]() { quitarProducto(); });
    connect(botonCobrar, &QPushButton::clicked, this, [this]() { cobrar(); });
    actualizarProductos();
}

void VentanaVentas::actualizarProductos() {
    QString error;
    if (!servicio.cargarProductos(error)) {
        QMessageBox::warning(this, "Error", error);
        return;
    }
    productosCombo->clear();
    for (const QJsonValue& v : servicio.productos()) {
        QJsonObject p = v.toObject();
        if (!p.value("activo").toBool(true)) continue;
        QString codigo = p.value("codigo").toString();
        QString texto = codigo + " - " + p.value("nombre").toString()
            + " (₡" + QString::number(p.value("precio").toDouble(), 'f', 2)
            + ", stock " + QString::number(p.value("existencia").toInt()) + ")";
        productosCombo->addItem(texto, codigo);
    }
}

void VentanaVentas::actualizarCarrito() {
    QJsonArray carrito = servicio.carrito();
    tabla->setRowCount(carrito.size());
    for (int fila = 0; fila < carrito.size(); ++fila) {
        QJsonObject item = carrito[fila].toObject();
        QStringList valores = {
            item.value("codigo").toString(), item.value("nombre").toString(),
            QString::number(item.value("precio").toDouble(), 'f', 2),
            QString::number(item.value("cantidad").toInt()),
            QString::number(item.value("precio").toDouble() * item.value("cantidad").toInt(), 'f', 2)
        };
        for (int col = 0; col < valores.size(); ++col)
            tabla->setItem(fila, col, new QTableWidgetItem(valores[col]));
    }
    totalLabel->setText("Total: ₡" + QString::number(servicio.total(), 'f', 2));
}

void VentanaVentas::agregarProducto() {
    QString error;
    if (!servicio.agregar(productosCombo->currentData().toString(), cantidad->value(), error))
        QMessageBox::warning(this, "No se pudo agregar", error);
    actualizarCarrito();
}

void VentanaVentas::quitarProducto() {
    int fila = tabla->currentRow();
    if (fila < 0) { QMessageBox::information(this, "Carrito", "Seleccione una fila"); return; }
    servicio.quitar(tabla->item(fila, 0)->text());
    actualizarCarrito();
}

void VentanaVentas::cobrar() {
    QString error;
    QJsonObject venta;
    double cambio = 0;
    if (!servicio.cobrar(efectivo->value(), cambio, venta, error)) {
        QMessageBox::warning(this, "No se pudo cobrar", error);
        return;
    }
    QMessageBox::information(this, "Venta realizada", "Venta guardada. Cambio: ₡" + QString::number(cambio, 'f', 2));
    efectivo->setValue(0);
    actualizarCarrito();
    actualizarProductos();
}
