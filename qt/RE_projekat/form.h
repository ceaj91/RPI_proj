#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QThread>
#include <iostream>
#include <string.h>
#include <wiringPi.h>

#define IRpin_sensor 4
#define IRpin_diode 1
#define MAXPULSE 65000
#define RESOLUTION 20
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QString tabname, QWidget *parent = nullptr);
    ~Form();


public slots:
    void onActiveTabChanged(QString &tabname);
    void handleButtonClick();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Form *ui;
    QString tab_device_name;
    QString current_active_tab;
    QString command_name;
    QMessageBox* infoMessageBox;
    int flag;
    uint16_t send_command[1000][2];
    uint16_t num_of_puleses;
    uint16_t pulses[100][2];
    uint8_t currentpulse;
    void printToFile(uint16_t pulses[][2], uint8_t currentpulse);
    void printPulses(uint16_t pulses[][2], uint8_t currentpulse);
    void ir_command_read();
    void load_command();
    void sendCommand(QString &buttonName);
    void read_command_from_file(QString &buttonName);
    void pulseIR(long microsecs);
};

#endif // FORM_H
