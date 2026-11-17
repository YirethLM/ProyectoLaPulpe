#ifndef VISTAINICIO_H
#define VISTAINICIO_H

#include <QWidget>

class VistaInicio : public QWidget {
    Q_OBJECT
public:
    explicit VistaInicio(QWidget *parent = nullptr);
signals:
    void navegarASolicitado(const QString& destino);
    void cerrarSesionSolicitado();
private:
    void setupUi();
    QWidget* crearTarjeta(const QString& iniciales, const QString& titulo, const QString& desc, const QString& detalle, const QString& id);
};

#endif // VISTAINICIO_H
