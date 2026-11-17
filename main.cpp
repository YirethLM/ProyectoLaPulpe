#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>

#include "InicioLaPulpe.h"
#include "VentanaProductos.h"
#include "VentanaVentas.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("LaPulpe");
    app.setOrganizationName("LaPulpe");

    // =====================================
    // ESTILOS GENERALES DEL SISTEMA
    // =====================================

    app.setStyleSheet(R"(
        QWidget {
            background-color: #FFF8F5;
            color: #344054;
            font-family: "Segoe UI";
            font-size: 11pt;
        }

        QLabel {
            background: transparent;
        }

        QPushButton {
            background-color: #FF6B6B;
            color: white;
            border: none;
            border-radius: 12px;
            padding: 10px 16px;
            font-weight: bold;
            min-height: 28px;
        }

        QPushButton:hover {
            background-color: #F94F5B;
        }

        QPushButton:pressed {
            background-color: #E74450;
        }

        QLineEdit, QComboBox,
        QSpinBox, QDoubleSpinBox {
            background-color: white;
            color: #344054;
            border: 2px solid #D6E7FF;
            border-radius: 10px;
            padding: 7px;
            min-height: 24px;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 2px solid #438BFA;
        }

        QComboBox QAbstractItemView {
            background-color: white;
            color: #344054;
            selection-background-color: #438BFA;
            selection-color: white;
        }

        QTableWidget {
            background-color: white;
            alternate-background-color: #F7FBFF;
            color: #344054;
            border: 1px solid #DDEBFF;
            border-radius: 10px;
            gridline-color: #EAF2FF;
            selection-background-color: #FFE2E2;
            selection-color: #344054;
        }

        QHeaderView::section {
            background-color: #438BFA;
            color: white;
            border: none;
            padding: 10px;
            font-weight: bold;
        }

        QTableCornerButton::section {
            background-color: #438BFA;
            border: none;
        }

        QTableWidget::item {
            padding: 5px;
        }

        QTableWidget::item:selected {
            background-color: #FFE2E2;
            color: #344054;
        }

        QMessageBox {
            background-color: #FFF8F5;
        }
    )");

    // =====================================
    // CARPETA PARA GUARDAR LOS DATOS
    // =====================================

    QString carpetaDatos =
        QStandardPaths::writableLocation(
            QStandardPaths::AppLocalDataLocation
        );

    if (carpetaDatos.isEmpty() ||
        !QDir().mkpath(carpetaDatos)) {

        QMessageBox::critical(
            nullptr,
            "Error de almacenamiento",
            "No se pudo crear la carpeta "
            "de datos de La Pulpe."
        );

        return 1;
    }

    QString rutaProductos =
        QDir(carpetaDatos).filePath(
            "productos.json"
        );

    QString rutaVentas =
        QDir(carpetaDatos).filePath(
            "ventas_area2.json"
        );

    // =====================================
    // LOCALIZAR EL INVENTARIO ORIGINAL
    // =====================================

    QString carpetaEjecutable =
        QCoreApplication::applicationDirPath();

    QString carpetaTrabajo =
        QDir::currentPath();

    QStringList posiblesRutas;

    posiblesRutas
        << QDir(carpetaTrabajo)
        .filePath("productos.json")

        << QDir(carpetaEjecutable)
        .filePath("productos.json")

        << QDir(carpetaEjecutable)
        .filePath("../productos.json")

        << QDir(carpetaEjecutable)
        .filePath("../../productos.json");

    QString origenProductos;

    for (const QString& posible : posiblesRutas) {
        if (QFileInfo::exists(posible)) {
            origenProductos = posible;
            break;
        }
    }

    // Copiar el inventario solo si todavia
    // no existe en la carpeta de datos.
    if (!QFile::exists(rutaProductos)) {

        if (origenProductos.isEmpty()) {

            QMessageBox::critical(
                nullptr,
                "Inventario no encontrado",
                "No se encontro productos.json.\n\n"
                "El inventario original no fue modificado."
            );

            return 1;
        }

        if (!QFile::copy(
            origenProductos,
            rutaProductos)) {

            QMessageBox::critical(
                nullptr,
                "Error al copiar inventario",
                "No se pudo copiar productos.json.\n\n"
                "Origen:\n" + origenProductos +
                "\n\nDestino:\n" + rutaProductos
            );

            return 1;
        }
    }

    // =====================================
    // CONSERVAR EL HISTORIAL EXISTENTE
    // =====================================

    // Intentar localizar el historial anterior
    // sin sobrescribir el nuevo.
    if (!QFile::exists(rutaVentas)) {

        QString origenVentas;

        for (const QString& rutaBase : posiblesRutas) {

            QString posible =
                QFileInfo(rutaBase)
                .dir()
                .filePath("ventas_area2.json");

            if (QFileInfo::exists(posible)) {
                origenVentas = posible;
                break;
            }
        }

        if (!origenVentas.isEmpty()) {

            if (!QFile::copy(
                origenVentas,
                rutaVentas)) {

                QMessageBox::warning(
                    nullptr,
                    "Aviso de historial",
                    "No se pudo copiar el historial anterior.\n"
                    "El archivo original sigue intacto.\n\n"
                    "No realices nuevos cobros hasta "
                    "revisar los datos."
                );

                return 1;
            }
        }
    }

    // =====================================
    // VENTANAS ORIGINALES DEL PROYECTO
    // =====================================


    // Ventana de productos e inventario
    VentanaProductos ventanaProductos(
        rutaProductos.toStdString()
    );

    // Ventana de ventas y cobro
    VentanaVentas ventanaVentas(
        rutaProductos
    );


    // =====================================
    // NUEVA PANTALLA PRINCIPAL
    // =====================================

    QWidget* ventanaPrincipal =
        crearPantallaInicio(

            // PRODUCTOS E INVENTARIO
            [&]() {
                ventanaProductos.show();
                ventanaProductos.raise();
                ventanaProductos.activateWindow();
            },

            // VENTAS Y COBRO
            [&]() {
                ventanaVentas.show();
                ventanaVentas.raise();
                ventanaVentas.activateWindow();
            }
        );

    ventanaPrincipal->show();

    // =====================================
    // EJECUTAR EL SISTEMA
    // =====================================

    int resultado = app.exec();

    delete ventanaPrincipal;

    return resultado;
}