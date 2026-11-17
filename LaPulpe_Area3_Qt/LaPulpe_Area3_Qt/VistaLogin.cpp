#include "VistaLogin.h"
#include "Config.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFrame>

VistaLogin::VistaLogin(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void VistaLogin::setupUi() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Panel Izquierdo
    QFrame* panelIzq = new QFrame();
    panelIzq->setStyleSheet("background-color: #2E0836;");
    QVBoxLayout* izqLayout = new QVBoxLayout(panelIzq);
    izqLayout->setContentsMargins(40, 40, 40, 40);
    
    QFrame* logo = new QFrame();
    logo->setFixedSize(64, 64);
    logo->setStyleSheet("background-color: #9C18A8; border-radius: 14px;");
    QLabel* lblLogo = new QLabel("LP", logo);
    lblLogo->setStyleSheet("color: white; font-weight: bold; font-size: 24px;");
    lblLogo->setAlignment(Qt::AlignCenter);
    lblLogo->setGeometry(0, 0, 64, 64);

    QLabel* lblSub = new QLabel(QString::fromUtf8("SISTEMA DE CAJA"));
    lblSub->setStyleSheet("color: #E879F9; font-weight: bold; letter-spacing: 1px;");
    QLabel* lblTitulo = new QLabel("La Pulpe");
    lblTitulo->setStyleSheet("color: #FFFFFF; font-size: 42px; font-weight: bold;");

    izqLayout->addWidget(logo);
    izqLayout->addStretch();
    izqLayout->addWidget(lblSub);
    izqLayout->addWidget(lblTitulo);
    izqLayout->addStretch();

    // Panel Derecho
    QFrame* panelDer = new QFrame();
    panelDer->setStyleSheet("background-color: #FAF8F5;");
    QVBoxLayout* derLayout = new QVBoxLayout(panelDer);
    derLayout->setAlignment(Qt::AlignCenter);

    QFrame* loginTarjeta = new QFrame();
    loginTarjeta->setFixedWidth(380);
    loginTarjeta->setStyleSheet("background-color: white; border-radius: 12px;");
    QVBoxLayout* tarjetaLayout = new QVBoxLayout(loginTarjeta);
    tarjetaLayout->setContentsMargins(28, 28, 28, 28);
    tarjetaLayout->setSpacing(12);

    QLabel* lblBienvenido = new QLabel("BIENVENIDO");
    lblBienvenido->setStyleSheet("color: #93179E; font-weight: bold; font-size: 11px;");
    QLabel* lblInicia = new QLabel("Iniciar sesión");
    lblInicia->setStyleSheet("color: #1E293B; font-size: 26px; font-weight: bold;");
    QLabel* lblInstr = new QLabel("Ingrese con sus credenciales para continuar.");
    lblInstr->setStyleSheet("color: #64748B; font-size: 13px;");

    edtUsuario = new QLineEdit();
    edtUsuario->setPlaceholderText("Escribe tu usuario...");
    edtUsuario->setStyleSheet("color: #1E293B; padding: 10px; border: 1px solid #CBD5E1; border-radius: 6px;");

    edtPass = new QLineEdit();
    edtPass->setPlaceholderText("Escribe tu contraseña...");
    edtPass->setEchoMode(QLineEdit::Password);
    edtPass->setStyleSheet("color: #1E293B; padding: 10px; border: 1px solid #CBD5E1; border-radius: 6px;");
    
    QPushButton* btnEntrar = new QPushButton("Entrar");
    btnEntrar->setStyleSheet("background-color: #93179E; color: #FFFFFF; font-weight: bold; padding: 12px; border-radius: 8px;");
    btnEntrar->setCursor(Qt::PointingHandCursor);
    
    connect(btnEntrar, &QPushButton::clicked, this, &VistaLogin::intentarLogin);
    connect(edtPass, &QLineEdit::returnPressed, this, &VistaLogin::intentarLogin);
    
    tarjetaLayout->addWidget(lblBienvenido);
    tarjetaLayout->addWidget(lblInicia);
    tarjetaLayout->addWidget(lblInstr);
    tarjetaLayout->addWidget(new QLabel("Usuario:"));
    tarjetaLayout->addWidget(edtUsuario);
    tarjetaLayout->addWidget(new QLabel("Contraseña:"));
    tarjetaLayout->addWidget(edtPass);
    tarjetaLayout->addWidget(btnEntrar);

    derLayout->addWidget(loginTarjeta);

    mainLayout->addWidget(panelIzq, 1);
    mainLayout->addWidget(panelDer, 1);
}

void VistaLogin::intentarLogin() {
    QString ruta = Config::getJsonPath("usuarios.json");
    QFile file(ruta);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", QString::fromUtf8("No se pudo cargar el archivo de usuarios en: ") + ruta);
        return;
    }

    QJsonArray usuarios = QJsonDocument::fromJson(file.readAll()).array();
    file.close();

    QString inputUser = edtUsuario->text().trimmed();
    QString inputPass = edtPass->text();

    for (const QJsonValue& val : usuarios) {
        QJsonObject u = val.toObject();
        
        QString usuario = u.contains("usuario") ? u["usuario"].toString() : u["username"].toString();
        QString pass = u.contains("contrasena") ? u["contrasena"].toString() : u["password"].toString();
        bool activo = u["activo"].isBool() ? u["activo"].toBool() : (u["activo"].toString() == "Activo");

        if (usuario.trimmed() == inputUser) {
            if (pass == inputPass) {
                if (activo) {
                    emit loginExitoso(u["nombre"].toString(), u["rol"].toString());
                    edtUsuario->clear();
                    edtPass->clear();
                    return;
                } else {
                    QMessageBox::warning(this, "Error", "Usuario desactivado.");
                    return;
                }
            } else {
                QMessageBox::warning(this, "Error", "Contraseña incorrecta.");
                return;
            }
        }
    }
    QMessageBox::warning(this, "Error", QString::fromUtf8("Usuario no encontrado en: ") + ruta);
}
