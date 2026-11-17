#include "VistaInicio.h"
#include "Estilos.h"
#include "Config.h"
#include "GestorProductos.h"
#include "AlmacenVentas.h"
#include "Venta.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QShowEvent>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>

VistaInicio::VistaInicio(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void VistaInicio::actualizarContadores() {
    // 1. Contador Productos
    GestorProductos gp;
    gp.cargar(Config::getJsonPath("productos.json").toStdString());
    lblDetalleProductos->setText(QString::number(gp.getProductos().size()) + " productos registrados");

    // 2. Contador Ventas de Hoy
    int ventasHoy = 0;
    QString rutaVentas = Config::getJsonPath("ventas.json");
    QFile fileVentas(rutaVentas);
    if (fileVentas.open(QIODevice::ReadOnly)) {
        QJsonDocument documento = QJsonDocument::fromJson(fileVentas.readAll());
        QJsonArray ventasArray = documento.isObject()
            ? documento.object().value("ventas").toArray()
            : documento.array();
        QString hoy = QDate::currentDate().toString("dd/MM/yyyy");
        for (const QJsonValue& val : ventasArray) {
            QJsonObject venta = val.toObject();
            QString fechaHora = venta["fecha"].toString();
            if (fechaHora.startsWith(hoy)) {
                ventasHoy++;
            }
        }
        fileVentas.close();
    }
    lblDetalleVentas->setText(QString::number(ventasHoy) + " ventas realizadas hoy");

    // 3. Contador Usuarios Registrados
    int totalUsuarios = 0;
    QFile fileUsuarios(Config::getJsonPath("usuarios.json"));
    if (fileUsuarios.open(QIODevice::ReadOnly)) {
        QJsonArray usuariosArray = QJsonDocument::fromJson(fileUsuarios.readAll()).array();
        totalUsuarios = usuariosArray.size();
        fileUsuarios.close();
    }
    lblDetalleUsuarios->setText(QString::number(totalUsuarios) + (totalUsuarios == 1 ? " usuario registrado" : " usuarios registrados"));
}

void VistaInicio::actualizarRol(const QString& rol, const QString& nombreUsuario) {
    if (!nombreUsuario.isEmpty()) {
        lblSaludo->setText(QString::fromUtf8("Buenos días, ") + nombreUsuario);
    }
    
    if (rol.compare("Cajero", Qt::CaseInsensitive) == 0) {
        lblTagRol->setText(QString::fromUtf8("● Cajero"));
        lblTagRol->setStyleSheet("background-color: #E0F2FE; color: #0369A1; border-radius: 12px; padding: 6px 14px; font-weight: bold;");
    } else {
        lblTagRol->setText(QString::fromUtf8("● ") + rol);
        lblTagRol->setStyleSheet("background-color: #DCFCE7; color: #15803D; border-radius: 12px; padding: 6px 14px; font-weight: bold;");
    }
}

void VistaInicio::showEvent(QShowEvent* event) {
    actualizarContadores();
    QWidget::showEvent(event);
}

void VistaInicio::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);

    // Header
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QVBoxLayout* tituloLayout = new QVBoxLayout();
    
    QLabel* lblHeader = new QLabel("LA PULPE");
    lblHeader->setStyleSheet("color: " + Estilos::COLOR_PRIMARIO + "; font-weight: bold; font-size: 12px; letter-spacing: 1px;");
    
    QLabel* lblTitulo = new QLabel("Inicio");
    lblTitulo->setObjectName("tituloSeccion");
    
    tituloLayout->addWidget(lblHeader);
    tituloLayout->addWidget(lblTitulo);
    
    QPushButton* btnCerrarSesion = new QPushButton("Cerrar sesión");
    btnCerrarSesion->setObjectName("btnCerrar");
    btnCerrarSesion->setCursor(Qt::PointingHandCursor);
    connect(btnCerrarSesion, &QPushButton::clicked, this, &VistaInicio::cerrarSesionSolicitado);
    
    headerLayout->addLayout(tituloLayout);
    headerLayout->addStretch();
    headerLayout->addWidget(btnCerrarSesion, 0, Qt::AlignTop);
    
    mainLayout->addLayout(headerLayout);

    // Saludo
    lblSaludo = new QLabel("Buenos días");
    lblSaludo->setStyleSheet("font-size: 32px; font-weight: 500; margin-top: 20px; color: #212121;");
    mainLayout->addWidget(lblSaludo);

    QHBoxLayout* subHeaderLayout = new QHBoxLayout();
    QLabel* lblSub = new QLabel("Seleccione una opción para comenzar.");
    lblSub->setObjectName("subtituloSeccion");
    
    lblTagRol = new QLabel("● Administrador");
    lblTagRol->setObjectName("tagRol");
    
    subHeaderLayout->addWidget(lblSub);
    subHeaderLayout->addStretch();
    subHeaderLayout->addWidget(lblTagRol);
    mainLayout->addLayout(subHeaderLayout);

    // Grid de tarjetas
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(20);
    
    lblDetalleProductos = new QLabel();
    lblDetalleVentas = new QLabel();
    lblDetalleUsuarios = new QLabel();

    gridLayout->addWidget(crearTarjeta("IN", "Productos e inventario", "Consulte existencias, precios y estados.", lblDetalleProductos, "Inventario"), 0, 0);
    gridLayout->addWidget(crearTarjeta("VC", "Ventas y cobro", "Registre una venta y calcule el cambio.", new QLabel("Abrir caja"), "Ventas"), 0, 1);
    gridLayout->addWidget(crearTarjeta("HV", "Historial de ventas", "Revise transacciones y genere tickets.", lblDetalleVentas, "Historial"), 1, 0);
    gridLayout->addWidget(crearTarjeta("US", "Usuarios", "Administre accesos y permisos del equipo.", lblDetalleUsuarios, "Usuarios"), 1, 1);
    
    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();
}

QWidget* VistaInicio::crearTarjeta(const QString& iniciales, const QString& titulo, const QString& desc, QLabel* lblDetalle, const QString& id) {
    QPushButton* tarjeta = new QPushButton();
    tarjeta->setObjectName("tarjetaBoton");
    tarjeta->setCursor(Qt::PointingHandCursor);
    tarjeta->setMinimumHeight(140);
    
    QHBoxLayout* layout = new QHBoxLayout(tarjeta);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);
    
    QLabel* lblIcono = new QLabel(iniciales);
    lblIcono->setFixedSize(48, 48);
    lblIcono->setAlignment(Qt::AlignCenter);
    lblIcono->setAttribute(Qt::WA_TransparentForMouseEvents);
    lblIcono->setStyleSheet("background-color: #F3E5F5; color: " + Estilos::COLOR_PRIMARIO + "; border-radius: 8px; font-weight: bold; font-size: 18px;");
    
    QVBoxLayout* txtLayout = new QVBoxLayout();
    QLabel* lblTitulo = new QLabel(titulo);
    lblTitulo->setObjectName("tituloTarjeta");
    lblTitulo->setAttribute(Qt::WA_TransparentForMouseEvents);
    
    QLabel* lblDesc = new QLabel(desc);
    lblDesc->setObjectName("descTarjeta");
    lblDesc->setAttribute(Qt::WA_TransparentForMouseEvents);
    
    lblDetalle->setObjectName("detalleTarjeta");
    lblDetalle->setAttribute(Qt::WA_TransparentForMouseEvents);
    
    txtLayout->addWidget(lblTitulo);
    txtLayout->addWidget(lblDesc);
    txtLayout->addWidget(lblDetalle);
    
    layout->addWidget(lblIcono, 0, Qt::AlignTop);
    layout->addLayout(txtLayout);
    layout->addStretch();
    
    QLabel* lblFlecha = new QLabel("→");
    lblFlecha->setStyleSheet("color: #BDBDBD; font-size: 20px; font-weight: bold;");
    lblFlecha->setAttribute(Qt::WA_TransparentForMouseEvents);
    layout->addWidget(lblFlecha, 0, Qt::AlignVCenter);
    
    connect(tarjeta, &QPushButton::clicked, [this, id]() { emit navegarASolicitado(id); });
    
    return tarjeta;
}
