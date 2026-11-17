#include "VistaInicio.h"
#include "Estilos.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

VistaInicio::VistaInicio(QWidget *parent) : QWidget(parent) {
    setupUi();
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
    QLabel* lblSaludo = new QLabel("Buenos días");
    lblSaludo->setStyleSheet("font-size: 32px; font-weight: 500; margin-top: 20px; color: #212121;");
    mainLayout->addWidget(lblSaludo);

    QHBoxLayout* subHeaderLayout = new QHBoxLayout();
    QLabel* lblSub = new QLabel("Seleccione una opción para comenzar.");
    lblSub->setObjectName("subtituloSeccion");
    
    QLabel* lblTagRol = new QLabel("● Administrador");
    lblTagRol->setObjectName("tagRol");
    
    subHeaderLayout->addWidget(lblSub);
    subHeaderLayout->addStretch();
    subHeaderLayout->addWidget(lblTagRol);
    mainLayout->addLayout(subHeaderLayout);

    // Grid de tarjetas
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(20);
    
    gridLayout->addWidget(crearTarjeta("IN", "Productos e inventario", "Consulte existencias, precios y estados.", "6 productos registrados", "Inventario"), 0, 0);
    gridLayout->addWidget(crearTarjeta("VC", "Ventas y cobro", "Registre una venta y calcule el cambio.", "Abrir caja", "Ventas"), 0, 1);
    gridLayout->addWidget(crearTarjeta("HV", "Historial de ventas", "Revise transacciones y genere tickets.", "3 ventas recientes", "Historial"), 1, 0);
    gridLayout->addWidget(crearTarjeta("US", "Usuarios", "Administre accesos y permisos del equipo.", "2 usuarios activos", "Usuarios"), 1, 1);
    
    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();
    // ... (resto de la barra inferior igual)
}

QWidget* VistaInicio::crearTarjeta(const QString& iniciales, const QString& titulo, const QString& desc, const QString& detalle, const QString& id) {
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
    lblIcono->setAttribute(Qt::WA_TransparentForMouseEvents); // Para que el clic pase al botón
    lblIcono->setStyleSheet("background-color: #F3E5F5; color: " + Estilos::COLOR_PRIMARIO + "; border-radius: 8px; font-weight: bold; font-size: 18px;");
    
    QVBoxLayout* txtLayout = new QVBoxLayout();
    QLabel* lblTitulo = new QLabel(titulo);
    lblTitulo->setObjectName("tituloTarjeta");
    lblTitulo->setAttribute(Qt::WA_TransparentForMouseEvents);
    
    QLabel* lblDesc = new QLabel(desc);
    lblDesc->setObjectName("descTarjeta");
    lblDesc->setAttribute(Qt::WA_TransparentForMouseEvents);
    
    QLabel* lblDetalle = new QLabel(detalle);
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
