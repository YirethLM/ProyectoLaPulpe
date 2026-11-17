#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include "NavegadorPantallas.h"

class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

private:
    NavegadorPantallas* navegador;

public:
    VentanaPrincipal(NavegadorPantallas* nav, QWidget* parent = nullptr);
    void configurarMenu();
};

#endif // VENTANAPRINCIPAL_H
