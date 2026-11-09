#pragma once
#include <QWidget>
#include "ServicioVenta.h"
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QTableWidget;
class QLabel;

// Ventana independiente, sin Q_OBJECT y sin un segundo main().
class VentanaVentas : public QWidget {
public:
    explicit VentanaVentas(const QString& rutaProductos, QWidget* parent = nullptr);
private:
    ServicioVenta servicio;
    QComboBox* productosCombo = nullptr;
    QSpinBox* cantidad = nullptr;
    QDoubleSpinBox* efectivo = nullptr;
    QTableWidget* tabla = nullptr;
    QLabel* totalLabel = nullptr;
    void actualizarProductos();
    void actualizarCarrito();
    void agregarProducto();
    void quitarProducto();
    void cobrar();
};
