#include "VistaInventario.h"
#include "Estilos.h"
#include "Config.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QShowEvent>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QSignalBlocker>

VistaInventario::VistaInventario(QWidget *parent) : QWidget(parent) {
    setupUi();
    connect(txtBusqueda, &QLineEdit::textChanged, this, &VistaInventario::cargarProductos);
}

void VistaInventario::showEvent(QShowEvent* event) {
    gestor.cargar(Config::getJsonPath("productos.json").toStdString());
    cargarProductos();
    QWidget::showEvent(event);
}

void VistaInventario::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);
    setStyleSheet("background-color: #FAF8F5;");

    // Fila Superior
    QHBoxLayout* topLayout = new QHBoxLayout();
    QPushButton* btnVolver = new QPushButton(QString::fromUtf8("← Volver"));
    btnVolver->setStyleSheet("background-color: #FFFFFF; color: #93179E; border: 1px solid #CBD5E1; border-radius: 8px; padding: 8px 16px; font-weight: bold;");
    btnVolver->setCursor(Qt::PointingHandCursor);
    connect(btnVolver, &QPushButton::clicked, this, &VistaInventario::volverSolicitado);
    topLayout->addWidget(btnVolver);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // Banner
    QFrame* banner = new QFrame();
    banner->setStyleSheet("background-color: #2E0836; border-radius: 12px; padding: 18px 24px;");
    QHBoxLayout* bannerLayout = new QHBoxLayout(banner);
    QVBoxLayout* titleLayout = new QVBoxLayout();
    QLabel* lblTitulo = new QLabel(QString::fromUtf8("LA PULPE | Productos e Inventario"));
    lblTitulo->setStyleSheet("color: #FFFFFF; font-size: 20px; font-weight: bold;");
    QLabel* lblSub = new QLabel(QString::fromUtf8("Consulte existencias, precios y registre nuevos artículos."));
    lblSub->setStyleSheet("color: #E9D5FF; font-size: 13px;");
    titleLayout->addWidget(lblTitulo);
    titleLayout->addWidget(lblSub);
    lblTotal = new QLabel("Total de productos: 0");
    lblTotal->setStyleSheet("background-color: #93179E; color: #FFFFFF; font-weight: bold; padding: 6px 14px; border-radius: 12px;");
    bannerLayout->addLayout(titleLayout);
    bannerLayout->addStretch();
    bannerLayout->addWidget(lblTotal);
    mainLayout->addWidget(banner);

    // Tarjeta Formulario
    QFrame* cardForm = new QFrame();
    cardForm->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px;");
    QVBoxLayout* formLayout = new QVBoxLayout(cardForm);
    formLayout->setContentsMargins(24, 24, 24, 24);
    
    QLabel* lblCardTitle = new QLabel(QString::fromUtf8("Nuevo Producto / Datos del Artículo"));
    lblCardTitle->setStyleSheet("color: #1E293B; font-size: 15px; font-weight: bold;");
    formLayout->addWidget(lblCardTitle);

    QGridLayout* grid = new QGridLayout();
    QString commonStyle = "color: #1E293B; padding: 8px; border: 1px solid #CBD5E1; border-radius: 8px; background-color: #FFFFFF;";
    edtCod = new QLineEdit(); edtCod->setPlaceholderText("Ej. P006"); edtCod->setStyleSheet(commonStyle);
    edtNom = new QLineEdit(); edtNom->setPlaceholderText("Ej. Café Molido 500g"); edtNom->setStyleSheet(commonStyle);
    
    edtPrecio = new QDoubleSpinBox();
    edtPrecio->setRange(0.0, 9999999.99); edtPrecio->setDecimals(2); edtPrecio->setPrefix(QString::fromUtf8("₡ "));
    edtPrecio->setStyleSheet(commonStyle);
    
    edtStock = new QSpinBox();
    edtStock->setRange(0, 999999);
    edtStock->setStyleSheet(commonStyle);
    
    grid->addWidget(new QLabel("Código:"), 0, 0); grid->addWidget(edtCod, 0, 1);
    grid->addWidget(new QLabel("Nombre:"), 0, 2); grid->addWidget(edtNom, 0, 3);
    grid->addWidget(new QLabel("Precio (₡):"), 1, 0); grid->addWidget(edtPrecio, 1, 1);
    grid->addWidget(new QLabel("Stock:"), 1, 2); grid->addWidget(edtStock, 1, 3);
    formLayout->addLayout(grid);

    QPushButton* btnAdd = new QPushButton(QString::fromUtf8("+ Agregar Producto"));
    btnAdd->setStyleSheet("background-color: #93179E; color: #FFFFFF; font-weight: bold; padding: 10px 22px; border-radius: 8px;");
    connect(btnAdd, &QPushButton::clicked, [this](){
        if(gestor.agregar(Producto(edtCod->text().toStdString(), edtNom->text().toStdString(), edtPrecio->value(), edtStock->value()))){
            gestor.guardar(Config::getJsonPath("productos.json").toStdString());
            cargarProductos();
        }
    });
    formLayout->addWidget(btnAdd, 0, Qt::AlignRight);
    mainLayout->addWidget(cardForm);

    // Tarjeta Tabla
    QFrame* cardTabla = new QFrame();
    cardTabla->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px;");
    QVBoxLayout* tableLayout = new QVBoxLayout(cardTabla);
    
    txtBusqueda = new QLineEdit();
    txtBusqueda->setPlaceholderText("Buscar por nombre o código...");
    txtBusqueda->setStyleSheet("background-color: #FAF8F5; padding: 10px 14px; border: 1px solid #CBD5E1; border-radius: 8px; color: #1E293B;");
    connect(txtBusqueda, &QLineEdit::textChanged, this, &VistaInventario::cargarProductos);
    tableLayout->addWidget(txtBusqueda);

    tablaProductos = new QTableWidget(0, 4);
    tablaProductos->setHorizontalHeaderLabels({"Código", "Nombre", "Precio", "Existencias"});
    tablaProductos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaProductos->verticalHeader()->setVisible(false);
    tablaProductos->setAlternatingRowColors(true);
    tablaProductos->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaProductos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaProductos->setStyleSheet("QTableWidget { background-color: #FFFFFF; } QTableWidget::item { color: #1E293B; } QHeaderView::section { background-color: #F3E8FF; color: #2E0836; font-weight: bold; padding: 8px; border: none; }");
    tableLayout->addWidget(tablaProductos);
    mainLayout->addWidget(cardTabla);
    
    connect(tablaProductos, &QTableWidget::cellChanged, [this](int row, int col){
        if (!tablaProductos->item(row, 0) || !tablaProductos->item(row, 2) || !tablaProductos->item(row, 3)) {
            return;
        }
        if(col == 2 || col == 3) {
            QString cod = tablaProductos->item(row, 0)->text();
            double precio = tablaProductos->item(row, 2)->text().remove(QChar(0x20A1)).trimmed().toDouble();
            int stock = tablaProductos->item(row, 3)->text().toInt();
            Producto* p = gestor.buscarPorCod(cod.toStdString());
            if(p) { p->setPrecio(precio); p->agregarExistencia(stock - p->getExistencia()); }
            gestor.guardar(Config::getJsonPath("productos.json").toStdString());
        }
    });
}

void VistaInventario::cargarProductos() {
    QSignalBlocker blocker(tablaProductos);
    const QString filtro = txtBusqueda->text().trimmed();
    const auto& productos = gestor.getProductos(); 
    tablaProductos->setRowCount(0);
    
    int row = 0;
    for (const auto& prod : productos) {
        QString cod = QString::fromStdString(prod.getCodigo());
        QString nom = QString::fromStdString(prod.getNombre());
        
        if (!filtro.isEmpty()) {
            if (cod.compare(filtro, Qt::CaseInsensitive) != 0 &&
                !cod.contains(filtro, Qt::CaseInsensitive) &&
                !nom.contains(filtro, Qt::CaseInsensitive)) {
                continue;
            }
        }
        
        tablaProductos->insertRow(row);
        tablaProductos->setItem(row, 0, new QTableWidgetItem(cod));
        tablaProductos->setItem(row, 1, new QTableWidgetItem(nom));
        tablaProductos->setItem(row, 2, new QTableWidgetItem(QString(QChar(0x20A1)) + " " + QString::number(prod.getPrecio(), 'f', 2)));
        tablaProductos->setItem(row, 3, new QTableWidgetItem(QString::number(prod.getExistencia())));
        row++;
    }
    lblTotal->setText(QString::number(tablaProductos->rowCount()) + " productos en catálogo");
}
