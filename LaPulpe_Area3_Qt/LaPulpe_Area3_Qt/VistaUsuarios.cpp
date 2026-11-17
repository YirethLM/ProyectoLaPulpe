#include "VistaUsuarios.h"
#include "Config.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QShowEvent>
#include <QFrame>

VistaUsuarios::VistaUsuarios(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void VistaUsuarios::showEvent(QShowEvent* event) {
    cargarUsuarios();
    QWidget::showEvent(event);
}

void VistaUsuarios::setupUi() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    setStyleSheet("background-color: #FFFFFF; color: #1E293B;");

    QPushButton* btnVolver = new QPushButton(QString::fromUtf8("← Volver"));
    btnVolver->setStyleSheet("background-color: #FFFFFF; color: #93179E; border: 1px solid #CBD5E1; border-radius: 8px; padding: 8px 16px; font-weight: bold;");
    btnVolver->setCursor(Qt::PointingHandCursor);
    connect(btnVolver, &QPushButton::clicked, this, &VistaUsuarios::volverSolicitado);
    
    lblTotal = new QLabel("Total de usuarios registrados: 0");
    lblTotal->setStyleSheet("background-color: #93179E; color: #FFFFFF; font-weight: bold; padding: 6px 14px; border-radius: 12px;");

    QFormLayout* form = new QFormLayout();
    edtUsuario = new QLineEdit();
    edtNombre = new QLineEdit();
    edtPass = new QLineEdit();
    edtPass->setEchoMode(QLineEdit::Password);
    comboRol = new QComboBox();
    comboRol->addItems({"Administrador", "Cajero"});
    chkActivo = new QCheckBox(QString::fromUtf8("Usuario Activo"));
    
    QPushButton* btnAdd = new QPushButton(QString::fromUtf8("+ Agregar Usuario"));
    QPushButton* btnGuardar = new QPushButton(QString::fromUtf8("Guardar Cambios"));
    QPushButton* btnQuitar = new QPushButton(QString::fromUtf8("Quitar Seleccionado"));
    
    btnAdd->setStyleSheet("background-color: #93179E; color: #FFFFFF; font-weight: bold; border-radius: 8px; height: 36px; padding: 0 16px;");
    btnGuardar->setStyleSheet("background-color: #0284C7; color: #FFFFFF; font-weight: bold; border-radius: 8px; height: 36px; padding: 0 16px;");
    btnQuitar->setStyleSheet("background-color: #DC2626; color: #FFFFFF; font-weight: bold; padding: 8px 16px; border-radius: 8px;");

    connect(btnAdd, &QPushButton::clicked, this, &VistaUsuarios::agregarUsuario);
    connect(btnGuardar, &QPushButton::clicked, this, &VistaUsuarios::guardarCambios);
    connect(btnQuitar, &QPushButton::clicked, this, &VistaUsuarios::quitarUsuario);

    form->addRow("Usuario:", edtUsuario);
    form->addRow("Nombre:", edtNombre);
    form->addRow("Contraseña:", edtPass);
    form->addRow("Rol:", comboRol);
    form->addRow(chkActivo);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnGuardar);
    form->addRow(btnLayout);
    
    tablaUsuarios = new QTableWidget(0, 4);
    tablaUsuarios->setHorizontalHeaderLabels({"Usuario", "Nombre", "Rol", "Estado"});
    tablaUsuarios->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaUsuarios->verticalHeader()->setVisible(false);
    tablaUsuarios->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaUsuarios->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    connect(tablaUsuarios, &QTableWidget::cellClicked, this, &VistaUsuarios::cargarFilaEnFormulario);

    layout->addWidget(btnVolver, 0, Qt::AlignLeft);
    layout->addWidget(lblTotal, 0, Qt::AlignRight);
    layout->addLayout(form);
    layout->addWidget(btnQuitar);
    layout->addWidget(tablaUsuarios);
}

void VistaUsuarios::cargarUsuarios() {
    tablaUsuarios->setRowCount(0);
    QFile file(Config::getJsonPath("usuarios.json"));
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonArray usuarios = QJsonDocument::fromJson(file.readAll()).array();
    file.close();

    for (const QJsonValue& val : usuarios) {
        QJsonObject u = val.toObject();
        int row = tablaUsuarios->rowCount();
        tablaUsuarios->insertRow(row);
        
        QTableWidgetItem* itemUser = new QTableWidgetItem(u["usuario"].toString());
        itemUser->setData(Qt::UserRole, u["contrasena"].toString()); // Guardar pass
        
        tablaUsuarios->setItem(row, 0, itemUser);
        tablaUsuarios->setItem(row, 1, new QTableWidgetItem(u["nombre"].toString()));
        tablaUsuarios->setItem(row, 2, new QTableWidgetItem(u["rol"].toString()));
        tablaUsuarios->setItem(row, 3, new QTableWidgetItem(u["activo"].toBool() ? "Activo" : "Inactivo"));
    }
    lblTotal->setText("Total de usuarios registrados: " + QString::number(tablaUsuarios->rowCount()));
}

void VistaUsuarios::cargarFilaEnFormulario(int row) {
    edtUsuario->setText(tablaUsuarios->item(row, 0)->text());
    edtNombre->setText(tablaUsuarios->item(row, 1)->text());
    edtPass->setText(tablaUsuarios->item(row, 0)->data(Qt::UserRole).toString());
    comboRol->setCurrentText(tablaUsuarios->item(row, 2)->text());
    chkActivo->setChecked(tablaUsuarios->item(row, 3)->text() == "Activo");
}

void VistaUsuarios::agregarUsuario() {
    if (edtUsuario->text().isEmpty() || edtPass->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Campos obligatorios vacíos.");
        return;
    }

    QFile file(Config::getJsonPath("usuarios.json"));
    if (!file.open(QIODevice::ReadWrite)) return;
    QJsonArray usuarios = QJsonDocument::fromJson(file.readAll()).array();

    for (const QJsonValue& val : usuarios) {
        if (val.toObject()["usuario"].toString() == edtUsuario->text()) {
            QMessageBox::warning(this, "Error", "El usuario ya existe.");
            file.close(); return;
        }
    }

    QJsonObject u;
    u["usuario"] = edtUsuario->text();
    u["nombre"] = edtNombre->text();
    u["contrasena"] = edtPass->text();
    u["rol"] = comboRol->currentText();
    u["activo"] = chkActivo->isChecked();
    usuarios.append(u);

    file.resize(0);
    file.write(QJsonDocument(usuarios).toJson());
    file.close();
    cargarUsuarios();
}

void VistaUsuarios::guardarCambios() {
    int row = tablaUsuarios->currentRow();
    if (row < 0) return;
    
    QFile file(Config::getJsonPath("usuarios.json"));
    if (!file.open(QIODevice::ReadWrite)) return;
    QJsonArray usuarios = QJsonDocument::fromJson(file.readAll()).array();
    
    QString oldUser = tablaUsuarios->item(row, 0)->text();
    bool isLastAdmin = false;
    int adminCount = 0;
    
    // Contar administradores activos
    for (const QJsonValue& val : usuarios) {
        if (val.toObject()["rol"] == "Administrador" && val.toObject()["activo"].toBool()) adminCount++;
    }

    for (int i = 0; i < usuarios.size(); ++i) {
        QJsonObject u = usuarios[i].toObject();
        if (u["usuario"].toString() == oldUser) {
            if (u["rol"] == "Administrador" && u["activo"].toBool() && !chkActivo->isChecked() && adminCount == 1) {
                QMessageBox::warning(this, "Error", "No puedes desactivar el único administrador activo.");
                file.close(); return;
            }
            u["usuario"] = edtUsuario->text();
            u["nombre"] = edtNombre->text();
            u["rol"] = comboRol->currentText();
            u["activo"] = chkActivo->isChecked();
            if (!edtPass->text().isEmpty()) u["contrasena"] = edtPass->text();
            usuarios[i] = u;
            break;
        }
    }
    file.resize(0);
    file.write(QJsonDocument(usuarios).toJson());
    file.close();
    cargarUsuarios();
}

void VistaUsuarios::quitarUsuario() {
    int row = tablaUsuarios->currentRow();
    if (row < 0) return;
    
    if (QMessageBox::question(this, "Confirmar", "¿Seguro de eliminar este usuario?") != QMessageBox::Yes) return;

    QFile file(Config::getJsonPath("usuarios.json"));
    if (!file.open(QIODevice::ReadWrite)) return;
    QJsonArray usuarios = QJsonDocument::fromJson(file.readAll()).array();
    
    QString user = tablaUsuarios->item(row, 0)->text();
    for (int i = 0; i < usuarios.size(); ++i) {
        if (usuarios[i].toObject()["usuario"].toString() == user) {
            if (usuarios[i].toObject()["rol"] == "Administrador") {
                QMessageBox::warning(this, "Error", "No puedes eliminar un administrador.");
                file.close(); return;
            }
            usuarios.removeAt(i);
            break;
        }
    }
    file.resize(0);
    file.write(QJsonDocument(usuarios).toJson());
    file.close();
    cargarUsuarios();
}
