
#pragma once

#include "Autenticacion.h"
#include <QDialog>

class QLineEdit;

class VentanaLogin : public QDialog {
    GestorUsuarios& gestor;
    Autenticacion autenticacion;

    QLineEdit* txtNombre;
    QLineEdit* txtUsuario;
    QLineEdit* txtClave;
    QLineEdit* txtConfirmar;

    bool inicial;
    int idUsuario = -1;

    void ingresar();

public:
    explicit VentanaLogin(
        GestorUsuarios& gestor,
        QWidget* padre = nullptr
    );

    int obtenerIdUsuario() const;
};
