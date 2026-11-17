#ifndef VISTAINICIO_H
#define VISTAINICIO_H

#include <QWidget>
#include <QLabel>

class VistaInicio : public QWidget {
    Q_OBJECT
public:
    explicit VistaInicio(QWidget *parent = nullptr);
    void actualizarRol(const QString& rol, const QString& nombreUsuario = "");
    void actualizarContadores();
signals:
    void navegarASolicitado(const QString& destino);
    void cerrarSesionSolicitado();
protected:
    void showEvent(QShowEvent* event) override;
private:
    void setupUi();
    QWidget* crearTarjeta(const QString& iniciales, const QString& titulo, const QString& desc, QLabel* lblDetalle, const QString& id);
    QLabel* lblDetalleProductos;
    QLabel* lblDetalleVentas;
    QLabel* lblDetalleUsuarios;
    QLabel* lblTagRol;
    QLabel* lblSaludo;
};

#endif // VISTAINICIO_H
