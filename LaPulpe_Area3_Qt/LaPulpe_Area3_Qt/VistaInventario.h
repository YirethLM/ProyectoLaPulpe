#ifndef VISTAINVENTARIO_H
#define VISTAINVENTARIO_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QShowEvent>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "GestorProductos.h"

class VistaInventario : public QWidget {
    Q_OBJECT
public:
    explicit VistaInventario(QWidget *parent = nullptr);
signals:
    void volverSolicitado();
protected:
    void showEvent(QShowEvent* event) override;
private:
    void setupUi();
    void cargarProductos();
    
    QTableWidget* tablaProductos;
    QLineEdit* txtBusqueda;
    QLabel* lblTotal;
    QLineEdit* edtCod;
    QLineEdit* edtNom;
    QDoubleSpinBox* edtPrecio;
    QSpinBox* edtStock;
    GestorProductos gestor;
};

#endif // VISTAINVENTARIO_H
