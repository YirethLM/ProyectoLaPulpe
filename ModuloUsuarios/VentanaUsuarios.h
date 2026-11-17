
#pragma once

#include "GestorUsuarios.h"
#include <QDialog>

class QTableWidget;
class QLineEdit;
class QComboBox;

class VentanaUsuarios : public QDialog {
    GestorUsuarios& gestor;

    int actorId;
    int seleccionado = -1;

    QTableWidget* tabla;

    QLineEdit* txtBuscar;
    QLineEdit* txtNombre;
    QLineEdit* txtUsuario;
    QLineEdit* txtClave;

    QComboBox* cmbRol;

    void refrescar();
    void limpiar();
    void seleccionar(int fila);
    void guardar(bool nuevo);
    void cambiarEstado();
    bool permiso();

public:
    VentanaUsuarios(
        GestorUsuarios& gestor,
        int actorId,
        QWidget* padre = nullptr
    );
};

