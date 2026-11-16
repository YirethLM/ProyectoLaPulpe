
#include "VentanaHistorial.h"
#include "ui_VentanaHistorial.h"

#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

// Constructor de la ventana de historial :joz1303
VentanaHistorial::VentanaHistorial(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    // Carga el diseño creado en Qt Designer :joz1303
    ui->setupUi(this);

    setWindowTitle("La Pulpe - Historial de ventas");

    // Configura la tabla de ventas :joz1303
    ui->tablaHistorial->setColumnCount(4);
    ui->tablaHistorial->setHorizontalHeaderLabels(
        QStringList{ "ID", "Fecha", "Total", "Pagado" }
    );

    // Ajusta las columnas al ancho de la tabla :joz1303
    ui->tablaHistorial->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    // Evita que se modifiquen los datos desde la tabla :joz1303
    ui->tablaHistorial->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    // Carga las ventas guardadas en JSON :joz1303
    gestor.cargarVentas();

    // Muestra inicialmente todas las ventas :joz1303
    actualizarTabla("");

    // Actualiza la tabla cuando se escribe un ID :joz1303
    connect(
        ui->campoBusqueda,
        &QLineEdit::textChanged,
        this,
        [this](const QString& texto)
        {
            actualizarTabla(texto);
        }
    );
}

// Libera la interfaz al cerrar la ventana :joz1303
VentanaHistorial::~VentanaHistorial()
{
    delete ui;
}

// Muestra las ventas y permite buscar por ID :joz1303
void VentanaHistorial::actualizarTabla(const QString& filtro)
{
    // Limpia los resultados anteriores :joz1303
    ui->tablaHistorial->setRowCount(0);

    const auto& ventas = gestor.consultarVentas();

    for (const auto& venta : ventas)
    {
        QString id = QString::number(venta.getId());

        // Filtra por identificador cuando se escribe uno :joz1303
        if (!filtro.trimmed().isEmpty() &&
            id != filtro.trimmed())
        {
            continue;
        }

        // Agrega una fila por cada venta encontrada :joz1303
        int fila = ui->tablaHistorial->rowCount();
        ui->tablaHistorial->insertRow(fila);

        // Columna 1: identificador :joz1303
        ui->tablaHistorial->setItem(
            fila, 0, new QTableWidgetItem(id)
        );

        // Columna 2: fecha :joz1303
        ui->tablaHistorial->setItem(
            fila, 1,
            new QTableWidgetItem(
                QString::fromStdString(venta.getFecha())
            )
        );

        // Columna 3: total de la venta :joz1303
        ui->tablaHistorial->setItem(
            fila, 2,
            new QTableWidgetItem(
                QString::number(venta.getTotal(), 'f', 2)
            )
        );

        // Columna 4: dinero pagado :joz1303
        ui->tablaHistorial->setItem(
            fila, 3,
            new QTableWidgetItem(
                QString::number(venta.getPagado(), 'f', 2)
            )
        );
    }
}
