#ifndef FACTURADIALOG_H
#define FACTURADIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QTableWidget>
#include <QPushButton>

class FacturaDialog : public QDialog {
    Q_OBJECT
public:
    explicit FacturaDialog(const QJsonObject& venta, QWidget* parent = nullptr);
private slots:
    void exportarPDF();
};

#endif // FACTURADIALOG_H
