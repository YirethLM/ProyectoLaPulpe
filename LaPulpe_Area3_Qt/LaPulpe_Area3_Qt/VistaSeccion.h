#ifndef VISTASECCION_H
#define VISTASECCION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "Estilos.h"

class VistaSeccion : public QWidget {
    Q_OBJECT
public:
    explicit VistaSeccion(const QString& titulo, QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignCenter);
        
        QLabel* lbl = new QLabel(titulo);
        lbl->setStyleSheet("font-size: 32px; font-weight: bold; color: " + Estilos::COLOR_TEXTO + ";");
        
        QPushButton* btnVolver = new QPushButton("← Volver al Inicio");
        btnVolver->setObjectName("btnAccion");
        btnVolver->setCursor(Qt::PointingHandCursor);
        
        layout->addWidget(lbl);
        layout->addSpacing(20);
        layout->addWidget(btnVolver);
        
        connect(btnVolver, &QPushButton::clicked, this, &VistaSeccion::volverSolicitado);
    }
signals:
    void volverSolicitado();
};

#endif // VISTASECCION_H
