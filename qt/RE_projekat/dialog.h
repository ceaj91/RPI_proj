#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QInputDialog>
#include <QLineEdit>
#include "form.h"

#define IRpin_sensor 4
#define IRpin_diode 1
QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_tabWidget_tabCloseRequested(int index);

    void on_pushButton_clicked();
    void on_tabWidget_currentChanged(int index);

    void on_tabWidget_tabBarClicked(int index);

signals:
    void activeTabChanged(QString &tabname);

private:
    Ui::Dialog *ui;
    QHash<QString, Form*>formMap;
};
#endif // DIALOG_H
