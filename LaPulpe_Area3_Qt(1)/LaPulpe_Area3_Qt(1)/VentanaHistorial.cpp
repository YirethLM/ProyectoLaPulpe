
#include "VentanaHistorial.h"
#include "ui_VentanaHistorial.h"
#include "../../TicketHtml.h"
#include "../../TicketTxt.h"

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

// Constructor de la ventana de historial :joz1303
VentanaHistorial::VentanaHistorial(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    // Carga el diseño creado con Qt Designer :joz1303
    ui->setupUi(this);
    setWindowTitle("La Pulpe - Historial de ventas");

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
    connect(ui->botonTicketTxt, &QPushButton::clicked,
        this, [this]() { generarComprobante(false); });
    connect(ui->botonTicketHtml, &QPushButton::clicked,
        this, [this]() { generarComprobante(true); });

    // Carga las ventas persistidas y muestra la tabla inicial :joz1303
    cargarHistorial();
}

// Libera la interfaz al cerrar la ventana :joz1303
VentanaHistorial::~VentanaHistorial()
{
    delete ui;
}

// Recarga el JSON y comunica errores sin bloquear la ventana :joz1303
void VentanaHistorial::cargarHistorial()
{
    // Si la lectura falla, el gestor conserva en memoria el último historial válido :joz1303
    if (!gestor.cargarVentas())
    {
        actualizarTabla(ui->campoBusqueda->text());
        // Distingue un archivo ausente de un JSON presente pero inválido :joz1303
        if (!QFileInfo::exists(QDir::current().filePath("ventas.json")))
        {
            statusBar()->showMessage(
                "No existe ventas.json en el directorio de trabajo.", 10000
            );
        }
        else
        {
            statusBar()->showMessage(
                "No se pudo leer ventas.json. Se conserva el historial cargado.",
                10000
            );
        }
        return;
    }

    actualizarTabla(ui->campoBusqueda->text());
}

// Muestra las ventas y permite buscar por ID :joz1303
void VentanaHistorial::actualizarTabla(const QString& filtro)
{
    // Limpia las filas de resultados anteriores antes de volver a consultar :joz1303
    ui->tablaHistorial->setRowCount(0);
    const QString idBuscado = filtro.trimmed();

    // Compara el filtro con el ID exacto de cada venta del historial :joz1303
    for (const auto& venta : gestor.consultarVentas())
    {
        const QString id = QString::number(venta.getId());
        // Aplica el filtro solamente cuando el campo contiene un ID :joz1303
        if (!idBuscado.isEmpty() && id != idBuscado)
        {
            continue; // Omite las ventas cuyo ID no coincide con la búsqueda :joz1303
        }

        // Agrega una fila por cada venta encontrada :joz1303
        const int fila = ui->tablaHistorial->rowCount();
        ui->tablaHistorial->insertRow(fila);

        // Muestra el identificador de la venta :joz1303
        ui->tablaHistorial->setItem(fila, 0, new QTableWidgetItem(id));

        // Muestra la fecha y los importes de la venta :joz1303
        ui->tablaHistorial->setItem(
            fila, 1, new QTableWidgetItem(QString::fromStdString(venta.getFecha()))
        );
        // El total se calcula con los subtotales guardados de sus líneas :joz1303
        ui->tablaHistorial->setItem(
            fila, 2, new QTableWidgetItem(QString::number(venta.getTotal(), 'f', 2))
        );
        // Presenta el monto entregado por el cliente :joz1303
        ui->tablaHistorial->setItem(
            fila, 3, new QTableWidgetItem(QString::number(venta.getPagado(), 'f', 2))
        );
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
    // formatoHtml elige el generador HTML; false selecciona el generador TXT :joz1303
    const int fila = ui->tablaHistorial->currentRow();
    // Requiere una venta seleccionada para evitar emitir un ticket incorrecto :joz1303
    if (fila < 0 || !ui->tablaHistorial->item(fila, 0))
    {
        statusBar()->showMessage(
            "Selecciona una venta para generar su comprobante.", 8000
        );
        return;
    }

    // Convierte el ID de la fila y recupera la venta original del gestor :joz1303
    bool idValido = false;
    const int id = ui->tablaHistorial->item(fila, 0)->text().toInt(&idValido);
    const Venta* venta = idValido ? gestor.buscarPorId(id) : nullptr;
    if (!venta)
    {
        statusBar()->showMessage(
            "No se pudo recuperar la venta seleccionada.", 8000
        );
        return;
    }

    // Usa la interfaz común del generador para reutilizar TXT y HTML :joz1303
    std::unique_ptr<GeneradorTicket> generador;
    if (formatoHtml)
    {
        generador = std::make_unique<TicketHtml>();
    }
    else
    {
        generador = std::make_unique<TicketTxt>();
    }

    // Guarda los comprobantes en una carpeta del usuario, separada del JSON :joz1303
    const QString documentos = QStandardPaths::writableLocation(
        QStandardPaths::DocumentsLocation
    );
    if (documentos.isEmpty())
    {
        statusBar()->showMessage(
            "No se encontró una carpeta de documentos para guardar el comprobante.",
            10000
        );
        return;
    }

    // Crea la carpeta de comprobantes si todavía no existe :joz1303
    const QString directorio = QDir(documentos).filePath("La Pulpe/Comprobantes");
    if (!QDir().mkpath(directorio))
    {
        statusBar()->showMessage(
            "No se pudo crear la carpeta de comprobantes.", 10000
        );
        return;
    }

    const QString extension = QString::fromStdString(generador->extension());
    const QString base = QString("venta_%1_%2")
        .arg(id)
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_zzz"));
    const QByteArray contenido = QByteArray::fromStdString(generador->generar(*venta));

    // Incluye fecha y hora para evitar colisiones con comprobantes anteriores :joz1303
    QString ruta;
    QFile archivo;
    for (int intento = 0; ; ++intento)
    {
        const QString sufijo = intento == 0 ? QString() : QString("_%1").arg(intento);
        ruta = QDir(directorio).filePath(base + sufijo + extension);
        archivo.setFileName(ruta);
        // NewOnly prohíbe reemplazar archivos; un sufijo resuelve colisiones :joz1303
        if (archivo.open(QIODevice::WriteOnly | QIODevice::NewOnly))
        {
            break;
        }
        if (!QFileInfo::exists(ruta))
        {
            statusBar()->showMessage(
                QString("No se pudo guardar el comprobante: %1")
                    .arg(archivo.errorString()),
                10000
            );
            return;
        }
    }

    // Verifica la escritura completa antes de dar el archivo por generado :joz1303
    if (archivo.write(contenido) != contenido.size())
    {
        const QString error = archivo.errorString();
        archivo.close();
        QFile::remove(ruta);
        statusBar()->showMessage(
            QString("No se pudo completar el comprobante: %1").arg(error),
            10000
        );
        return;
    }
    archivo.close();

    // Abre el comprobante con la aplicación predeterminada del sistema :joz1303
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(ruta)))
    {
        statusBar()->showMessage(
            QString("Comprobante guardado en %1, pero no se pudo abrir.").arg(ruta),
            10000
        );
        return;
    }

    statusBar()->showMessage(QString("Comprobante guardado en %1").arg(ruta), 10000);
}
