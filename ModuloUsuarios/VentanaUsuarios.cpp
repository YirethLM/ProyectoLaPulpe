
#include "VentanaUsuarios.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

VentanaUsuarios::VentanaUsuarios(
    GestorUsuarios& g,
    int actor,
    QWidget* padre)
    : QDialog(padre),
    gestor(g),
    actorId(actor) {

    setWindowTitle("La Pulpe - Gestion de usuarios");
    resize(800, 560);

    auto principal = new QVBoxLayout(this);

    txtBuscar = new QLineEdit;
    txtBuscar->setPlaceholderText(
        "Buscar por nombre o usuario"
    );

    principal->addWidget(txtBuscar);

    tabla = new QTableWidget(0, 5);

    tabla->setHorizontalHeaderLabels({
        "ID", "Nombre", "Usuario", "Rol", "Estado"
        });

    tabla->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    tabla->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    tabla->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    tabla->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    principal->addWidget(tabla);

    txtNombre = new QLineEdit;
    txtUsuario = new QLineEdit;
    txtClave = new QLineEdit;

    txtNombre->setMaxLength(120);
    txtUsuario->setMaxLength(60);
    txtClave->setMaxLength(128);

    txtClave->setEchoMode(QLineEdit::Password);

    txtClave->setPlaceholderText(
        "Al modificar, deje vacio para conservar la contrasena"
    );

    cmbRol = new QComboBox;
    cmbRol->addItems({ "Administrador", "Cajero" });
    cmbRol->setCurrentIndex(1);

    auto form = new QFormLayout;

    form->addRow("Nombre:", txtNombre);
    form->addRow("Usuario:", txtUsuario);
    form->addRow("Contrasena:", txtClave);
    form->addRow("Rol:", cmbRol);

    principal->addLayout(form);

    auto botones = new QHBoxLayout;

    auto crear = new QPushButton("Crear");
    auto modificar = new QPushButton("Modificar");
    auto estado = new QPushButton("Activar / Desactivar");
    auto nuevo = new QPushButton("Limpiar / Nuevo");

    for (auto b : { crear, modificar, estado, nuevo }) {
        b->setAutoDefault(false);
        botones->addWidget(b);
    }

    principal->addLayout(botones);

    connect(
        txtBuscar,
        &QLineEdit::textChanged,
        this,
        [this] {
            limpiar();
            refrescar();
        }
    );

    connect(
        tabla,
        &QTableWidget::cellClicked,
        this,
        [this](int fila, int) {
            seleccionar(fila);
        }
    );

    connect(
        crear,
        &QPushButton::clicked,
        this,
        [this] {
            guardar(true);
        }
    );

    connect(
        modificar,
        &QPushButton::clicked,
        this,
        [this] {
            guardar(false);
        }
    );

    connect(
        estado,
        &QPushButton::clicked,
        this,
        [this] {
            cambiarEstado();
        }
    );

    connect(
        nuevo,
        &QPushButton::clicked,
        this,
        [this] {
            limpiar();
        }
    );

    if (permiso()) {
        refrescar();
    }
    else {
        setEnabled(false);
    }
}

bool VentanaUsuarios::permiso() {
    const auto u = gestor.buscarUsuarioPorId(actorId);

    if (u && u->estaActivo() &&
        u->obtenerRol() == Rol::Administrador) {
        return true;
    }

    QMessageBox::warning(
        this,
        "Acceso denegado",
        "Se requiere una cuenta de administrador activa."
    );

    return false;
}

void VentanaUsuarios::refrescar() {
    tabla->setRowCount(0);

    const auto filtro = txtBuscar->text().trimmed();

    for (const auto& u : gestor.listarUsuarios()) {
        const auto n = QString::fromStdString(
            u->obtenerNombre()
        );

        const auto login = QString::fromStdString(
            u->obtenerNombreUsuario()
        );

        if (!filtro.isEmpty() &&
            !n.contains(filtro, Qt::CaseInsensitive) &&
            !login.contains(filtro, Qt::CaseInsensitive)) {
            continue;
        }

        const int f = tabla->rowCount();
        tabla->insertRow(f);

        const QStringList datos{
            QString::number(u->obtenerId()),
            n,
            login,
            u->obtenerRol() == Rol::Administrador
                ? "Administrador" : "Cajero",
            u->estaActivo() ? "Activo" : "Inactivo"
        };

        for (int c = 0; c < 5; ++c) {
            tabla->setItem(
                f,
                c,
                new QTableWidgetItem(datos[c])
            );
        }
    }
}

void VentanaUsuarios::limpiar() {
    seleccionado = -1;

    tabla->clearSelection();
    txtNombre->clear();
    txtUsuario->clear();

    txtUsuario->setReadOnly(false);
    txtClave->clear();

    cmbRol->setCurrentIndex(1);
}

void VentanaUsuarios::seleccionar(int fila) {
    if (!tabla->item(fila, 0)) {
        return;
    }

    seleccionado =
        tabla->item(fila, 0)->text().toInt();

    const auto u =
        gestor.buscarUsuarioPorId(seleccionado);

    if (!u) {
        return;
    }

    txtNombre->setText(
        QString::fromStdString(u->obtenerNombre())
    );

    txtUsuario->setText(
        QString::fromStdString(
            u->obtenerNombreUsuario()
        )
    );

    txtUsuario->setReadOnly(true);
    txtClave->clear();

    cmbRol->setCurrentIndex(
        u->obtenerRol() == Rol::Administrador ? 0 : 1
    );
}

void VentanaUsuarios::guardar(bool nuevo) {
    if (!permiso()) {
        return;
    }

    if (nuevo && seleccionado != -1) {
        QMessageBox::information(
            this,
            "Crear",
            "Presione Limpiar / Nuevo antes "
            "de crear otra cuenta."
        );
        return;
    }

    if (!nuevo && seleccionado == -1) {
        QMessageBox::warning(
            this,
            "Seleccion",
            "Seleccione un usuario de la tabla."
        );
        return;
    }

    const Rol rol =
        cmbRol->currentIndex() == 0
        ? Rol::Administrador
        : Rol::Cajero;

    const bool ok = nuevo
        ? gestor.crearUsuario(
            actorId,
            txtNombre->text(),
            txtUsuario->text(),
            txtClave->text(),
            rol
        )
        : gestor.modificarUsuario(
            actorId,
            seleccionado,
            txtNombre->text(),
            rol,
            txtClave->text()
        );

    if (!ok) {
        QMessageBox::warning(
            this,
            "Error",
            gestor.ultimoError()
        );
        return;
    }

    limpiar();
    refrescar();

    QMessageBox::information(
        this,
        "Guardado",
        "Usuario guardado correctamente."
    );
}

void VentanaUsuarios::cambiarEstado() {
    if (!permiso()) {
        return;
    }

    const auto u =
        gestor.buscarUsuarioPorId(seleccionado);

    if (!u) {
        QMessageBox::warning(
            this,
            "Seleccion",
            "Seleccione un usuario."
        );
        return;
    }

    const bool activar = !u->estaActivo();

    if (QMessageBox::question(
        this,
        "Confirmar",
        activar
        ? "Desea activar esta cuenta?"
        : "Desea desactivar esta cuenta?"
    ) != QMessageBox::Yes) {
        return;
    }

    if (!gestor.cambiarEstado(
        actorId,
        seleccionado,
        activar)) {

        QMessageBox::warning(
            this,
            "Error",
            gestor.ultimoError()
        );
        return;
    }

    limpiar();
    refrescar();
}
