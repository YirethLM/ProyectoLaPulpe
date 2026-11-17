
#include "VentanaLogin.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

VentanaLogin::VentanaLogin(
    GestorUsuarios& g,
    QWidget* padre)
    : QDialog(padre),
    gestor(g),
    autenticacion(g),
    inicial(g.estaVacio()) {

    setWindowTitle(
        inicial
        ? "La Pulpe - Crear primer administrador"
        : "La Pulpe - Iniciar sesion"
    );

    resize(420, 260);

    auto principal = new QVBoxLayout(this);

    auto titulo = new QLabel(
        inicial
        ? "Configure su cuenta de administrador"
        : "Bienvenida a La Pulpe"
    );

    principal->addWidget(titulo);

    auto form = new QFormLayout;

    txtNombre = new QLineEdit;
    txtUsuario = new QLineEdit;
    txtClave = new QLineEdit;
    txtConfirmar = new QLineEdit;

    txtNombre->setObjectName("nombre");
    txtUsuario->setObjectName("usuario");
    txtClave->setObjectName("clave");
    txtConfirmar->setObjectName("confirmar");

    txtNombre->setMaxLength(120);
    txtUsuario->setMaxLength(60);
    txtClave->setMaxLength(128);
    txtConfirmar->setMaxLength(128);

    txtClave->setEchoMode(QLineEdit::Password);
    txtConfirmar->setEchoMode(QLineEdit::Password);

    if (inicial) {
        form->addRow("Nombre completo:", txtNombre);
    }
    else {
        txtNombre->setParent(this);
        txtNombre->hide();
    }

    form->addRow("Usuario:", txtUsuario);
    form->addRow("Contrasena:", txtClave);

    if (inicial) {
        form->addRow(
            "Confirmar contrasena:",
            txtConfirmar
        );
    }
    else {
        txtConfirmar->setParent(this);
        txtConfirmar->hide();
    }

    principal->addLayout(form);

    if (inicial) {
        principal->addWidget(
            new QLabel(
                "Use una contrasena de 8 a 128 caracteres."
            )
        );
    }

    auto boton = new QPushButton(
        inicial
        ? "Crear administrador e ingresar"
        : "Ingresar"
    );

    boton->setDefault(true);
    principal->addWidget(boton);

    connect(
        boton,
        &QPushButton::clicked,
        this,
        [this] {
            ingresar();
        }
    );
}

int VentanaLogin::obtenerIdUsuario() const {
    return idUsuario;
}

void VentanaLogin::ingresar() {
    if (inicial) {
        if (txtClave->text() != txtConfirmar->text()) {
            QMessageBox::warning(
                this,
                "Contrasena",
                "Las contrasenas no coinciden."
            );
            return;
        }

        if (!gestor.crearAdministradorInicial(
            txtNombre->text(),
            txtUsuario->text(),
            txtClave->text())) {

            QMessageBox::warning(
                this,
                "Error",
                gestor.ultimoError()
            );
            return;
        }
    }

    const auto u = autenticacion.iniciarSesion(
        txtUsuario->text(),
        txtClave->text()
    );

    txtClave->clear();
    txtConfirmar->clear();

    if (!u) {
        QMessageBox::warning(
            this,
            "Acceso denegado",
            "Usuario o contrasena incorrectos, "
            "o cuenta inactiva."
        );
        return;
    }

    idUsuario = u->obtenerId();
    accept();
}
