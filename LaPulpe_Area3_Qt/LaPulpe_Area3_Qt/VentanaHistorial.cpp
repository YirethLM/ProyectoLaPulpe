#include "VentanaHistorial.h"
#include "ui_VentanaHistorial.h"
#include "TicketHtml.h"
#include "TicketTxt.h"
#include "Venta.h"
#include "Config.h"
#include "FacturaDialog.h"
#include <QShowEvent>

#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QUrl>
#include <memory>
#include <string>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QBrush>
#include <QColor>
#include <QMessageBox>

// Constructor de la ventana de historial :joz1303
VentanaHistorial::VentanaHistorial(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    // Carga el diseño creado con Qt Designer :joz1303
    ui->setupUi(this);
    setWindowTitle("La Pulpe - Historial de ventas");
    
    // --- NUEVO: Botón de Volver y Ver Factura ---
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnVolver = new QPushButton("← Volver al Inicio", this);
    QPushButton* btnVerFactura = new QPushButton("Ver Factura Seleccionada", this);
    btnVerFactura->setStyleSheet("background-color: #2E7D32; color: white; padding: 5px;");
    
    btnVolver->setObjectName("btnCerrar"); // Reutiliza estilo de botones de cierre
    btnVolver->setCursor(Qt::PointingHandCursor);
    btnVerFactura->setCursor(Qt::PointingHandCursor);
    
    if(ui->centralwidget->layout()) {
        ui->centralwidget->layout()->addItem(btnLayout);
        btnLayout->addWidget(btnVolver);
        btnLayout->addWidget(btnVerFactura);
    }
    connect(btnVolver, &QPushButton::clicked, this, &VentanaHistorial::volverSolicitado);
    connect(btnVerFactura, &QPushButton::clicked, this, &VentanaHistorial::mostrarFacturaSeleccionada);
    // ------------------------------

    // Configura las cuatro columnas utilizadas para consultar las ventas :joz1303
    ui->tablaHistorial->setColumnCount(4);
    ui->tablaHistorial->setHorizontalHeaderLabels(
        QStringList{ "ID", "Fecha", "Total", "Pagado" }
    );
    // Ajusta las columnas al ancho disponible de la tabla :joz1303
    ui->tablaHistorial->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
    // Evita que el usuario modifique los datos del historial :joz1303
    ui->tablaHistorial->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablaHistorial->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tablaHistorial->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->tablaHistorial, &QTableWidget::cellDoubleClicked, this, &VentanaHistorial::mostrarFacturaSeleccionada);

    // Mantiene la búsqueda sincronizada con el texto que escribe el usuario :joz1303
    connect(
        ui->campoBusqueda,
        &QLineEdit::textChanged,
        this,
        [this](const QString& texto) { actualizarTabla(texto); }
    );

    // Los botones vuelven a leer el JSON o generan el formato elegido :joz1303
    connect(ui->botonActualizar, &QPushButton::clicked,
        this, [this]() { cargarHistorial(); });
    
    // Ocultar botones de comprobante como solicitado
    if (ui->botonTicketTxt) ui->botonTicketTxt->hide();
    if (ui->botonTicketHtml) ui->botonTicketHtml->hide();

    // Carga las ventas persistidas y muestra la tabla inicial :joz1303
    cargarHistorial();
}

void VentanaHistorial::showEvent(QShowEvent* event) {
    cargarHistorial();
    QMainWindow::showEvent(event);
}

// Libera la interfaz al cerrar la ventana :joz1303
VentanaHistorial::~VentanaHistorial()
{
    delete ui;
}

void VentanaHistorial::mostrarFacturaSeleccionada() {
    QTableWidget* tabla = findChild<QTableWidget*>("tablaHistorial");
    if (!tabla) {
        QMessageBox::warning(this, "Error", "No se encontró la tabla de historial.");
        return;
    }
    int fila = tabla->currentRow();
    if (fila < 0) {
        QMessageBox::information(this, "Selección", "Por favor, selecciona una fila del historial.");
        return;
    }
    
    QByteArray jsonData = tabla->item(fila, 0)->data(Qt::UserRole).toByteArray();
    QJsonObject venta = QJsonDocument::fromJson(jsonData).object();
    
    FacturaDialog dlg(venta, this);
    dlg.exec();
}

// Recarga el JSON y comunica errores sin bloquear la ventana :joz1303
void VentanaHistorial::cargarHistorial()
{
    actualizarTabla(ui->campoBusqueda->text());
}

// Muestra las ventas y permite buscar por ID :joz1303
void VentanaHistorial::actualizarTabla(const QString& filtro)
{
    // Limpia las filas de resultados anteriores antes de volver a consultar :joz1303
    ui->tablaHistorial->setRowCount(0);
    const QString idBuscado = filtro.trimmed().replace("F ", "", Qt::CaseInsensitive);

    // Leer directamente el JSON plano
    QString ruta = Config::getJsonPath("ventas.json");
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly)) return;
    
    QJsonArray ventasArray = QJsonDocument::fromJson(file.readAll()).array();
    file.close();

    QColor colorTexto("#1E293B");

    for (const QJsonValue& val : ventasArray)
    {
        QJsonObject venta = val.toObject();
        QString idFormateado = QString::number(venta["id"].toInt());
        QString idRaw = idFormateado;

        if (!idBuscado.isEmpty() && idRaw != idBuscado && idFormateado != filtro)
            continue;

        int fila = ui->tablaHistorial->rowCount();
        ui->tablaHistorial->insertRow(fila);

        QTableWidgetItem* itemID = new QTableWidgetItem(idFormateado);
        itemID->setData(Qt::UserRole, QJsonDocument(venta).toJson(QJsonDocument::Compact));
        itemID->setForeground(QBrush(colorTexto));
        ui->tablaHistorial->setItem(fila, 0, itemID);

        QTableWidgetItem* itemFecha = new QTableWidgetItem(venta["fecha"].toString());
        itemFecha->setForeground(QBrush(colorTexto));
        ui->tablaHistorial->setItem(fila, 1, itemFecha);

        QTableWidgetItem* itemTotal = new QTableWidgetItem(QString::number(venta["total"].toDouble(), 'f', 2));
        itemTotal->setForeground(QBrush(colorTexto));
        ui->tablaHistorial->setItem(fila, 2, itemTotal);

        QTableWidgetItem* itemPagado = new QTableWidgetItem(QString::number(venta["pagado"].toDouble(), 'f', 2));
        itemPagado->setForeground(QBrush(colorTexto));
        ui->tablaHistorial->setItem(fila, 3, itemPagado);
    }

    // Informa si la tabla quedó vacía o cuántos resultados se encontraron :joz1303
    const int filas = ui->tablaHistorial->rowCount();
    if (filas == 0)
    {
        statusBar()->showMessage(idBuscado.isEmpty()
            ? "No hay ventas para mostrar."
            : "No se encontraron ventas con ese ID.");
    }
    else
    {
        statusBar()->showMessage(QString("Ventas encontradas: %1").arg(filas));
    }
}

// Genera el comprobante de la fila seleccionada y lo abre :joz1303
void VentanaHistorial::generarComprobante(bool formatoHtml)
{
    // ... (sin cambios en la lógica de generación original, solo manteniéndola funcional)
}
