#ifndef VISTAVENTAS_H
#define VISTAVENTAS_H
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QShowEvent>
#include <QComboBox>
#include <QSpinBox>
#include "GestorProductos.h"
#include "AlmacenVentas.h"

class VistaVentas : public QWidget {
    Q_OBJECT
public:
    explicit VistaVentas(QWidget *parent = nullptr);
signals:
    void volverSolicitado();
protected:
    void showEvent(QShowEvent* event) override;
private:
    void setupUi();
    void cargarComboProductos();
    void agregarAlCarrito();
    void quitarDelCarrito(int row);
    void calcularTotales();
    void procesarCobro();
    void actualizarTablaCarrito();
    double calcularTotalGeneral() const;

    QComboBox* comboProductos;
    QSpinBox* spinCantidad;
    QTableWidget* tablaCarrito;
    QLabel* lblTotal;
    QLineEdit* txtMonto;
    QLabel* lblCambio;
    
    GestorProductos gestorProductos;
    AlmacenVentas gestorVentas;
    struct ItemCarrito {
        Producto producto;
        int cantidad;
    };
    std::vector<ItemCarrito> carrito;
};

#endif // VISTAVENTAS_H
