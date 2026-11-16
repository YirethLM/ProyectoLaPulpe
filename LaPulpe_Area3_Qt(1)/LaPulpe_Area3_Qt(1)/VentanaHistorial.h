
#pragma once

#include <QMainWindow>
#include <QString>
#include "../../GestorHistorial.h"

// Formulario creado con Qt Designer :joz1303
namespace Ui {
    class MainWindow;
}

// Ventana para consultar el historial de ventas :joz1303
class VentanaHistorial : public QMainWindow
{
public:
    // Constructor de la ventana :joz1303
    explicit VentanaHistorial(QWidget* parent = nullptr);

    // Destructor para liberar la interfaz :joz1303
    ~VentanaHistorial() override;

private:
    // Formulario visual de Qt Designer :joz1303
    Ui::MainWindow* ui;

    // Gestor que administra el historial de ventas :joz1303
    GestorHistorial gestor{ "ventas.json" };

    // Actualiza la tabla y permite filtrar ventas por ID :joz1303
    void actualizarTabla(const QString& filtro);
};
