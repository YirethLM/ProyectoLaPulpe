#ifndef VISTALOGIN_H
#define VISTALOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class VistaLogin : public QWidget {
    Q_OBJECT
public:
    explicit VistaLogin(QWidget *parent = nullptr);
signals:
    void loginExitoso(const QString& nombre, const QString& rol);
private:
    QLineEdit* edtUsuario;
    QLineEdit* edtPass;
    void intentarLogin();
    void setupUi();
};
#endif
