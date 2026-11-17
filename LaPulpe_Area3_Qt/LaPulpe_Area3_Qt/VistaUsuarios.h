#ifndef VISTAUSUARIOS_H
#define VISTAUSUARIOS_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QShowEvent>

class VistaUsuarios : public QWidget {
    Q_OBJECT
public:
    explicit VistaUsuarios(QWidget *parent = nullptr);
signals:
    void volverSolicitado();
protected:
    void showEvent(QShowEvent* event) override;
private:
    void setupUi();
    void cargarUsuarios();
    void agregarUsuario();
    void guardarCambios();
    void quitarUsuario();
    void cargarFilaEnFormulario(int row);

    QTableWidget* tablaUsuarios;
    QLineEdit* edtUsuario;
    QLineEdit* edtNombre;
    QLineEdit* edtPass;
    QComboBox* comboRol;
    QCheckBox* chkActivo;
    QLabel* lblTotal;
};

#endif
