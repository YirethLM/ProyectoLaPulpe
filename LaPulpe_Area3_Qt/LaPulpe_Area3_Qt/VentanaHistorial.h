
#pragma once

#include <QMainWindow>
#include <QObject>
#include <QLabel>
#include <QString>
#include "GestorHistorial.h"
#include <QPushButton>
#include <QVBoxLayout>

// Formulario creado con Qt Designer :joz1303
namespace Ui {
    class MainWindow;
}

// Ventana para consultar el historial de ventas :joz1303
class VentanaHistorial : public QMainWindow
{
    Q_OBJECT
public:
    // Constructor de la ventana :joz1303
    explicit VentanaHistorial(QWidget* parent = nullptr);

    // Destructor para liberar la interfaz :joz1303
    ~VentanaHistorial() override;

signals:
    void volverSolicitado();

protected:
    void showEvent(QShowEvent* event) override;

private:
        // Formulario visual de Qt Designer :joz1303
        Ui::MainWindow* ui;

        // Gestor que administra el historial de ventas :joz1303
        GestorHistorial gestor{ "ventas.json" };
        QLabel* lblTotalHoy = nullptr;

        void mostrarFacturaSeleccionada();

        // Recarga el archivo JSON sin interrumpir la ventana :joz1303
        void cargarHistorial();


    // filtro: ID exacto buscado; una cadena vacía muestra todas las ventas :joz1303
    void actualizarTabla(const QString& filtro);

    // formatoHtml: true genera HTML; false genera TXT y abre el archivo creado :joz1303
    void generarComprobante(bool formatoHtml);
};
