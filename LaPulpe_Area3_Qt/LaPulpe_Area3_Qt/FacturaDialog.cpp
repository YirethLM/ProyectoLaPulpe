#include "FacturaDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QJsonArray>
#include <QFrame>
#include <QDateTime>
#include <QPainter>
#include <QPdfWriter>
#include <QFileDialog>
#include <QMessageBox>

FacturaDialog::FacturaDialog(const QJsonObject& venta, QWidget* parent) : QDialog(parent) {
    setWindowTitle("Detalle de Factura");
    setMinimumSize(500, 600);
    setStyleSheet("background-color: #FFFFFF; color: #1E293B;");

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Encabezado
    QHBoxLayout* header = new QHBoxLayout();
    QVBoxLayout* logoBox = new QVBoxLayout();
    logoBox->addWidget(new QLabel("<b>LA PULPE</b>"));
    logoBox->addWidget(new QLabel("Empresa: La Pulpe - Administración de Pulpería"));
    header->addLayout(logoBox);
    
    QVBoxLayout* infoBox = new QVBoxLayout();
    infoBox->addWidget(new QLabel("<b>Fecha:</b> " + venta["fecha"].toString()));
    infoBox->addWidget(new QLabel("<b>Factura #:</b> " + QString::number(venta["id"].toInt())));
    header->addLayout(infoBox);
    layout->addLayout(header);

    layout->addWidget(new QLabel("<b>Factura para:</b> Cliente Contado / Consumidor Final"));

    // Tabla productos
    QTableWidget* tabla = new QTableWidget(0, 4);
    tabla->setHorizontalHeaderLabels({"UNIDADES", "DESCRIPCION", "PRECIO UNIT", "IMPORTE"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    QJsonArray prods = venta["lineas"].toArray();
    double totalVenta = venta["total"].toDouble();

    for (const QJsonValue& val : prods) {
        QJsonObject p = val.toObject();
        
        // Carga robusta de campos (compatibilidad)
        int cantidad = p["cantidad"].toInt();
        double precioUnit = p["precioUsado"].toDouble();
        if (precioUnit == 0.0) precioUnit = p["precio_unit"].toDouble();
        if (precioUnit == 0.0) precioUnit = p["precio"].toDouble();
        if (precioUnit == 0.0) precioUnit = p["precioUnitario"].toDouble();

        double importe = p["importe"].toDouble();
        if (importe == 0.0) importe = p["subtotal"].toDouble();
        if (importe == 0.0) importe = cantidad * precioUnit; // fallback
        
        int row = tabla->rowCount();
        tabla->insertRow(row);
        tabla->setItem(row, 0, new QTableWidgetItem(QString::number(cantidad)));
        tabla->setItem(row, 1, new QTableWidgetItem(p["nombre"].toString()));
        tabla->setItem(row, 2, new QTableWidgetItem(QString::number(precioUnit, 'f', 2)));
        tabla->setItem(row, 3, new QTableWidgetItem(QString::number(importe, 'f', 2)));
    }

    if (totalVenta == 0.0) {
        for (const QJsonValue& val : prods) {
            QJsonObject p = val.toObject();
            int c = p["cantidad"].toInt();
            double pu = p["precioUsado"].toDouble();
            if (pu == 0.0) pu = p["precio"].toDouble();
            totalVenta += c * pu;
        }
    }
    layout->addWidget(tabla);

    // Totales
    layout->addWidget(new QLabel("<b>BASE IMPONIBLE:</b> ₡" + QString::number(totalVenta / 1.13, 'f', 2)));
    layout->addWidget(new QLabel("<b>IVA (13%):</b> ₡" + QString::number(totalVenta - (totalVenta / 1.13), 'f', 2)));
    layout->addWidget(new QLabel("<b>TOTAL:</b> ₡" + QString::number(totalVenta, 'f', 2)));

    // Botones
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnPdf = new QPushButton("Exportar PDF");
    QPushButton* btnCerrar = new QPushButton("Cerrar");
    connect(btnPdf, &QPushButton::clicked, this, &FacturaDialog::exportarPDF);
    connect(btnCerrar, &QPushButton::clicked, this, &FacturaDialog::close);
    btnLayout->addWidget(btnPdf);
    btnLayout->addWidget(btnCerrar);
    layout->addLayout(btnLayout);
}

void FacturaDialog::exportarPDF() {
    QString fileName = QFileDialog::getSaveFileName(this, "Guardar PDF", "", "*.pdf");
    if (fileName.isEmpty()) return;
    
    QPdfWriter writer(fileName);
    QPainter painter(&writer);
    painter.drawText(100, 100, "Factura La Pulpe");
    painter.end();
    QMessageBox::information(this, "Éxito", "PDF guardado.");
}
