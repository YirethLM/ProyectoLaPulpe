#include "VistaVentas.h"
#include "Estilos.h"
#include "Config.h"
#include "GestorHistorial.h"
#include "Venta.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDateTime>
#include <QDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QFrame>
#include <QSpinBox>

VistaVentas::VistaVentas(QWidget *parent) 
    : QWidget(parent) {
    setupUi();
}

void VistaVentas::showEvent(QShowEvent* event) {
    gestorProductos.cargar(Config::getJsonPath("productos.json").toStdString());
    cargarComboProductos();
    QWidget::showEvent(event);
}

void VistaVentas::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);
    setStyleSheet("background-color: #FAF8F5;");

    // Header
    QHBoxLayout* topLayout = new QHBoxLayout();
    QPushButton* btnVolver = new QPushButton(QString::fromUtf8("← Volver"));
    btnVolver->setStyleSheet("background-color: #FFFFFF; color: #93179E; border: 1px solid #CBD5E1; border-radius: 8px; padding: 6px 14px; font-weight: bold;");
    btnVolver->setCursor(Qt::PointingHandCursor);
    connect(btnVolver, &QPushButton::clicked, this, &VistaVentas::volverSolicitado);
    topLayout->addWidget(btnVolver);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // Banner
    QFrame* banner = new QFrame();
    banner->setStyleSheet("background-color: #2E0836; border-radius: 12px; padding: 18px 24px;");
    QHBoxLayout* bannerLayout = new QHBoxLayout(banner);
    QVBoxLayout* titleLayout = new QVBoxLayout();
    QLabel* lblTitulo = new QLabel(QString::fromUtf8("LA PULPE | Ventas y Cobro"));
    lblTitulo->setStyleSheet("color: #FFFFFF; font-size: 20px; font-weight: bold;");
    QLabel* lblSub = new QLabel(QString::fromUtf8("Seleccione productos, calcule el cambio y registre la factura."));
    lblSub->setStyleSheet("color: #E9D5FF; font-size: 13px;");
    titleLayout->addWidget(lblTitulo);
    titleLayout->addWidget(lblSub);
    QLabel* lblStatus = new QLabel("Caja Activa");
    lblStatus->setStyleSheet("background-color: #93179E; color: #FFFFFF; font-weight: bold; padding: 6px 14px; border-radius: 12px;");
    bannerLayout->addLayout(titleLayout);
    bannerLayout->addStretch();
    bannerLayout->addWidget(lblStatus);
    mainLayout->addWidget(banner);

    // Tarjeta 1: Selección
    QFrame* cardSelect = new QFrame();
    cardSelect->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px;");
    QVBoxLayout* selectLayout = new QVBoxLayout(cardSelect);
    selectLayout->addWidget(new QLabel("<b>Agregar Artículo</b>"));
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    comboProductos = new QComboBox();
    comboProductos->setStyleSheet("color: #1E293B; padding: 8px; border: 1px solid #CBD5E1; border-radius: 8px; background: white;");
    spinCantidad = new QSpinBox();
    spinCantidad->setMinimum(1);
    spinCantidad->setMinimumWidth(110);
    QPushButton* btnAdd = new QPushButton(QString::fromUtf8("+ Agregar"));
    btnAdd->setStyleSheet("background-color: #93179E; color: #FFFFFF; font-weight: bold; padding: 0 20px; height: 38px; border-radius: 8px;");
    connect(btnAdd, &QPushButton::clicked, this, &VistaVentas::agregarAlCarrito);
    controlsLayout->addWidget(comboProductos, 4);
    controlsLayout->addWidget(spinCantidad, 1);
    controlsLayout->addWidget(btnAdd);
    selectLayout->addLayout(controlsLayout);
    mainLayout->addWidget(cardSelect);

    // Tarjeta 2: Carrito
    QFrame* cardTabla = new QFrame();
    cardTabla->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px;");
    QVBoxLayout* tableLayout = new QVBoxLayout(cardTabla);
    tablaCarrito = new QTableWidget(0, 5);
    tablaCarrito->setHorizontalHeaderLabels({"Código", "Producto", "Precio U.", "Cant.", "Subtotal"});
    tablaCarrito->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaCarrito->verticalHeader()->setVisible(false);
    tablaCarrito->setAlternatingRowColors(true);
    tablaCarrito->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaCarrito->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaCarrito->setStyleSheet("QTableWidget { background-color: #FFFFFF; } QTableWidget::item { color: #1E293B; } QHeaderView::section { background-color: #F3E8FF; color: #2E0836; font-weight: bold; padding: 8px; border: none; }");
    tableLayout->addWidget(tablaCarrito);
    mainLayout->addWidget(cardTabla);

    // Tarjeta 3: Cobro
    QFrame* cardCobro = new QFrame();
    cardCobro->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px;");
    QHBoxLayout* cobroLayout = new QHBoxLayout(cardCobro);
    
    lblTotal = new QLabel("Total a pagar: ₡0");
    lblTotal->setStyleSheet("font-size: 24px; font-weight: bold; color: #2E0836;");
    
    QVBoxLayout* rightCobro = new QVBoxLayout();
    txtMonto = new QLineEdit();
    txtMonto->setPlaceholderText(QString::fromUtf8("Monto recibido (₡)..."));
    txtMonto->setStyleSheet("color: #1E293B; padding: 10px; border: 1px solid #CBD5E1; border-radius: 8px; background: white;");
    connect(txtMonto, &QLineEdit::textChanged, this, &VistaVentas::calcularTotales);
    lblCambio = new QLabel("Cambio: ₡0");
    lblCambio->setStyleSheet("font-size: 16px; font-weight: bold; color: #15803D;");
    
    QPushButton* btnCobrar = new QPushButton(QString::fromUtf8("Cobrar e Imprimir"));
    btnCobrar->setStyleSheet("background-color: #15803D; color: #FFFFFF; font-weight: bold; font-size: 14px; padding: 12px 24px; border-radius: 8px;");
    connect(btnCobrar, &QPushButton::clicked, this, &VistaVentas::procesarCobro);
    
    rightCobro->addWidget(txtMonto);
    rightCobro->addWidget(lblCambio);
    rightCobro->addWidget(btnCobrar);
    
    cobroLayout->addWidget(lblTotal);
    cobroLayout->addLayout(rightCobro);
    mainLayout->addWidget(cardCobro);

    connect(tablaCarrito, &QTableWidget::cellDoubleClicked, [this](int row, int){
        quitarDelCarrito(row);
    });
}

void VistaVentas::cargarComboProductos() {
    comboProductos->clear();
    const auto& productos = gestorProductos.getProductos();
    for (const auto& p : productos) {
        if (p.getActivo() && p.getExistencia() > 0) {
            QString texto = QString::fromStdString(p.getNombre()) + " - ₡" + QString::number(p.getPrecio(), 'f', 2) + " (Stock: " + QString::number(p.getExistencia()) + ")";
            comboProductos->addItem(texto, QString::fromStdString(p.getCodigo()));
        }
    }
}

void VistaVentas::agregarAlCarrito() {
    if (comboProductos->currentIndex() < 0) return;
    QString codigo = comboProductos->currentData().toString();
    Producto* p = gestorProductos.buscarPorCod(codigo.toStdString());
    if (!p) return;

    int cantidad = spinCantidad->value();
    if (cantidad > p->getExistencia()) {
        QMessageBox::warning(this, "Stock Insuficiente", "No hay suficiente existencia para este producto. Stock actual: " + QString::number(p->getExistencia()));
        return;
    }

    bool encontrado = false;
    for (auto& item : carrito) {
        if (item.producto.getCodigo() == p->getCodigo()) {
            if (item.cantidad + cantidad > p->getExistencia()) {
                QMessageBox::warning(this, "Stock Insuficiente", "La cantidad total en el carrito supera el stock disponible.");
                return;
            }
            item.cantidad += cantidad;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        carrito.push_back({*p, cantidad});
    }

    actualizarTablaCarrito();
    calcularTotales();
}

void VistaVentas::quitarDelCarrito(int row) {
    if (row >= 0 && row < (int)carrito.size()) {
        carrito.erase(carrito.begin() + row);
        actualizarTablaCarrito();
        calcularTotales();
    }
}

void VistaVentas::actualizarTablaCarrito() {
    tablaCarrito->setRowCount(0);
    int row = 0;
    for (const auto& item : carrito) {
        tablaCarrito->insertRow(row);
        tablaCarrito->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(item.producto.getCodigo())));
        tablaCarrito->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(item.producto.getNombre())));
        tablaCarrito->setItem(row, 2, new QTableWidgetItem(QString::number(item.producto.getPrecio(), 'f', 2)));
        tablaCarrito->setItem(row, 3, new QTableWidgetItem(QString::number(item.cantidad)));
        double subtotal = item.producto.getPrecio() * item.cantidad;
        tablaCarrito->setItem(row, 4, new QTableWidgetItem(QString::number(subtotal, 'f', 2)));
        row++;
    }
}

double VistaVentas::calcularTotalGeneral() const {
    double total = 0.0;
    for (const auto& item : carrito) {
        total += item.producto.getPrecio() * item.cantidad;
    }
    return total;
}

void VistaVentas::calcularTotales() {
    double total = calcularTotalGeneral();
    lblTotal->setText("Total a pagar: ₡" + QString::number(total, 'f', 2));

    bool ok = false;
    double monto = txtMonto->text().toDouble(&ok);
    if (ok) {
        double cambio = monto - total;
        if (cambio >= 0) {
            lblCambio->setText("Cambio: ₡" + QString::number(cambio, 'f', 2));
            lblCambio->setStyleSheet("font-size: 16px; font-weight: bold; color: #15803D;");
        } else {
            lblCambio->setText("Faltan: ₡" + QString::number(-cambio, 'f', 2));
            lblCambio->setStyleSheet("font-size: 16px; font-weight: bold; color: #DC2626;");
        }
    } else {
        lblCambio->setText("Cambio: ₡0");
        lblCambio->setStyleSheet("font-size: 16px; font-weight: bold; color: #15803D;");
    }
}

void VistaVentas::procesarCobro() {
    if (carrito.empty()) {
        QMessageBox::warning(this, "Carrito Vacío", "Agregue productos al carrito antes de cobrar.");
        return;
    }

    bool ok = false;
    double monto = txtMonto->text().toDouble(&ok);
    double total = calcularTotalGeneral();

    if (!ok || monto < total) {
        QMessageBox::warning(this, "Monto Insuficiente", "El monto recibido es menor al total a pagar.");
        return;
    }

    GestorHistorial gestorHistorial(Config::getJsonPath("ventas.json").toStdString());
    gestorHistorial.cargarVentas();
    int nuevoId = 1;
    const auto& ventasAnt = gestorHistorial.consultarVentas();
    if (!ventasAnt.empty()) {
        for (const auto& v : ventasAnt) {
            if (v.getId() >= nuevoId) nuevoId = v.getId() + 1;
        }
    }

    QDateTime ahora = QDateTime::currentDateTime();
    QString fechaStr = ahora.toString("yyyy-MM-dd HH:mm:ss");

    Venta nuevaVenta(nuevoId, fechaStr.toStdString(), monto);

    for (const auto& item : carrito) {
        LineaVenta linea(item.producto.getCodigo(), item.producto.getNombre(), item.producto.getPrecio(), item.cantidad);
        nuevaVenta.agregarLinea(linea);

        Producto* p = gestorProductos.buscarPorCod(item.producto.getCodigo());
        if (p) {
            p->agregarExistencia(-item.cantidad);
        }
    }

    gestorProductos.guardar(Config::getJsonPath("productos.json").toStdString());
    gestorHistorial.agregarVenta(nuevaVenta);
    gestorHistorial.guardarVentas();

    QMessageBox::information(this, "Cobro Exitoso", QString("Venta #%1 registrada con éxito.\nCambio a devolver: ₡%2").arg(nuevoId).arg(monto - total, 0, 'f', 2));

    carrito.clear();
    actualizarTablaCarrito();
    txtMonto->clear();
    cargarComboProductos();
    calcularTotales();
}
