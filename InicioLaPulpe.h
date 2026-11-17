
#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QFont>
#include <QCursor>
#include <QPen>
#include <QPolygonF>
#include <functional>

// Crea los iconos directamente con Qt.
// No necesitas descargar imagenes.

inline QPixmap iconoLaPulpe(int tipo)
{
    QPixmap imagen(72, 72);
    imagen.fill(Qt::transparent);

    QPainter p(&imagen);
    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);
    p.drawRoundedRect(QRectF(3, 3, 66, 66), 15, 15);

    QColor color = tipo == 1
        ? QColor("#FF626B")
        : QColor("#2979EB");

    QPen lapiz(color, 3.5);
    lapiz.setCapStyle(Qt::RoundCap);
    lapiz.setJoinStyle(Qt::RoundJoin);

    p.setPen(lapiz);
    p.setBrush(Qt::NoBrush);

    if (tipo == 1) {
        // Caja de inventario
        QPolygonF caja;
        caja << QPointF(17, 26)
            << QPointF(36, 16)
            << QPointF(55, 26)
            << QPointF(36, 37);

        p.drawPolygon(caja);
        p.drawLine(17, 26, 17, 49);
        p.drawLine(17, 49, 36, 59);
        p.drawLine(36, 37, 36, 59);
        p.drawLine(55, 26, 55, 49);
        p.drawLine(55, 49, 36, 59);
        p.drawLine(26, 21, 45, 32);
    }
    else {
        // Carrito de compras
        p.drawLine(13, 19, 20, 19);
        p.drawLine(20, 19, 27, 46);
        p.drawLine(27, 46, 55, 46);
        p.drawLine(24, 26, 59, 26);
        p.drawLine(59, 26, 54, 40);
        p.drawLine(54, 40, 26, 40);

        p.setBrush(color);
        p.drawEllipse(QPointF(31, 54), 4, 4);
        p.drawEllipse(QPointF(51, 54), 4, 4);
    }

    return imagen;
}

inline QPushButton* botonLaPulpe(
    const QString& texto,
    bool productos,
    QWidget* padre)
{
    QPushButton* boton = new QPushButton(texto, padre);

    boton->setIcon(
        iconoLaPulpe(productos ? 1 : 2)
    );

    boton->setIconSize(QSize(68, 68));
    boton->setMinimumHeight(112);
    boton->setCursor(Qt::PointingHandCursor);

    QString normal = productos
        ? "#FF7377" : "#448FFF";

    QString oscuro = productos
        ? "#F64F58" : "#176CE8";

    QString hover = productos
        ? "#F94F5B" : "#216FEE";

    boton->setStyleSheet(QString(R"(
        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:1,
                stop:0 %1,
                stop:1 %2
            );

            color: white;
            border: 1px solid %2;
            border-radius: 25px;

            font-family: "Segoe UI";
            font-size: 22px;
            font-weight: 700;

            text-align: center;
            padding: 16px 24px;
        }

        QPushButton:hover {
            background-color: %3;
            border: 2px solid white;
        }

        QPushButton:pressed {
            background-color: %2;
            padding-top: 20px;
        }

        QPushButton:focus {
            border: 3px solid #FFD166;
        }
    )").arg(normal, oscuro, hover));

    auto* sombra = new QGraphicsDropShadowEffect(boton);
    sombra->setBlurRadius(23);
    sombra->setOffset(0, 8);
    sombra->setColor(QColor(70, 60, 60, 48));

    boton->setGraphicsEffect(sombra);

    return boton;
}

// Pantalla principal con funciones personalizables.
inline QWidget* crearPantallaInicio(
    std::function<void()> abrirProductos,
    std::function<void()> abrirVentas)
{
    QWidget* ventana = new QWidget();

    ventana->setObjectName("pantallaInicio");
    ventana->setWindowTitle("La Pulpe - Inicio");
    ventana->resize(840, 670);
    ventana->setMinimumSize(760, 610);

    ventana->setStyleSheet(R"(
        QWidget#pantallaInicio {
            background-color: #FFF7F2;
        }
    )");

    auto* principal = new QVBoxLayout(ventana);
    principal->setContentsMargins(48, 28, 48, 25);
    principal->setSpacing(17);

    // Icono superior
    QLabel* tienda = new QLabel("✦  🏪  ✦");
    tienda->setAlignment(Qt::AlignCenter);
    tienda->setStyleSheet(
        "font-size: 45px;"
        "color: #F5A23B;"
        "background: transparent;"
    );

    principal->addWidget(tienda);

    // Titulo
    QFrame* marcoTitulo = new QFrame();
    marcoTitulo->setStyleSheet(R"(
        QFrame {
            background-color: #FFFFFF;
            border: 3px solid #FFD9D5;
            border-radius: 26px;
        }
    )");

    auto* tituloLayout = new QVBoxLayout(marcoTitulo);
    tituloLayout->setContentsMargins(12, 13, 12, 13);

    QLabel* titulo = new QLabel("LA PULPE");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet(R"(
        QLabel {
            color: #FF626B;
            font-family: "Segoe UI";
            font-size: 43px;
            font-weight: 800;
            border: none;
            background: transparent;
        }
    )");

    tituloLayout->addWidget(titulo);
    principal->addWidget(marcoTitulo);

    principal->addSpacing(7);

    // Bienvenida
    QLabel* bienvenida = new QLabel(
        "Bienvenido al sistema de gestion comercial"
    );

    bienvenida->setAlignment(Qt::AlignCenter);
    bienvenida->setStyleSheet(R"(
        QLabel {
            color: #343D4A;
            font-family: "Segoe UI";
            font-size: 21px;
            font-weight: 700;
            background: transparent;
        }
    )");

    principal->addWidget(bienvenida);

    QLabel* indicacion = new QLabel(
        "Seleccione el area que desea utilizar"
    );

    indicacion->setAlignment(Qt::AlignCenter);
    indicacion->setStyleSheet(R"(
        QLabel {
            color: #626B79;
            font-family: "Segoe UI";
            font-size: 17px;
            background: transparent;
        }
    )");

    principal->addWidget(indicacion);
    principal->addSpacing(25);

    // Botones principales
    QPushButton* btnProductos = botonLaPulpe(
        "PRODUCTOS E INVENTARIO  ›",
        true,
        ventana
    );

    QPushButton* btnVentas = botonLaPulpe(
        "VENTAS Y COBRO  ›",
        false,
        ventana
    );

    principal->addWidget(btnProductos);
    principal->addSpacing(13);
    principal->addWidget(btnVentas);

    principal->addStretch();

    // Pie de pagina
    QLabel* pie = new QLabel(
        "La Pulpe  |  Inventario y ventas"
    );

    pie->setAlignment(Qt::AlignCenter);
    pie->setStyleSheet(R"(
        QLabel {
            color: #788394;
            font-family: "Segoe UI";
            font-size: 14px;
            background: transparent;
        }
    )");

    principal->addWidget(pie);

    // Mantener las acciones de cada boton
    QObject::connect(
        btnProductos,
        &QPushButton::clicked,
        ventana,
        [abrirProductos]() {
            if (abrirProductos) abrirProductos();
        }
    );

    QObject::connect(
        btnVentas,
        &QPushButton::clicked,
        ventana,
        [abrirVentas]() {
            if (abrirVentas) abrirVentas();
        }
    );

    return ventana;
}   
