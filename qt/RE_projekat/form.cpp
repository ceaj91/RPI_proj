#include "form.h"
#include "ui_form.h"
#include <string.h>

Form::Form(QString tabname,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->tab_device_name = tabname;
    flag_reading = false;
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    bool ok;
    QString buttonName = QInputDialog::getText(this,"New Button","Nova komanda:",QLineEdit::Normal,"",&ok);
    if(ok && !buttonName.isEmpty())
    {
        QPushButton *newbutton = new QPushButton(buttonName,this);
        command_name = buttonName;
        ui->verticalLayout->addWidget(newbutton);
        infoMessageBox = new QMessageBox(QMessageBox::Information, "Transmit", "Please wait for command loading from the device.", QMessageBox::NoButton, this);
        infoMessageBox->show();
        //write to file
        load_command();

        infoMessageBox->close();
        connect(newbutton, &QPushButton::clicked, this, &Form::handleButtonClick);
    }
}

void Form::onActiveTabChanged(QString &tabname)
{
    current_active_tab = tabname;
    if(!QString::compare(current_active_tab, tab_device_name, Qt::CaseInsensitive))
        printf("tab opened: %s\n",tabname.toStdString().c_str());
}

void Form::handleButtonClick()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton)
    {
        QString buttonName = clickedButton->text();
        sendCommand(buttonName);
        // Search for code in text and send command

    }
}
void Form::read_command_from_file(QString &buttonName)
{

    FILE *device;
    char line[100];
    char read_command[100];
    char temp[100];
    uint16_t value=0;
    int i =0;
    bool command_found = false;
    num_of_puleses=0;

    device = fopen(tab_device_name.toStdString().c_str(),"r");
    if (device == NULL) {
        printf("File could not be opened.");
    }
    while (fgets(line,sizeof(line),device) != NULL) {
        if(!strcmp(line, "}"))
        {
            command_found = false;
        break;
        }

        if(command_found){
            sscanf(line,"%[^(]", temp);
            if(!strcmp(temp, "pulseIR"))
            {
                sscanf(line,"pulseIR(%d);", &value);
                send_command[i][0] = value;
            }
        else if(!strcmp(temp, "delayMicroseconds"))
            {
        sscanf(line,"delayMicroseconds(%d);", &value);
        //printf("value: %d\n",value);
                send_command[i][1] = value;
                i++;
                num_of_puleses++;
            }

        }

        sscanf(line,"int %[^[]", read_command);
        if(!strcmp(buttonName.toStdString().c_str(), read_command))
        {
         command_found = true;
         //printf("USPESNO PRONASAO KOMANDU : %s\n",read_command);
        }
        else
            command_found = false;
    }
}

void Form::pulseIR(long microsecs) {
    while (microsecs > 0) {
        digitalWrite(IRpin_diode, HIGH);
        delayMicroseconds(10);
        digitalWrite(IRpin_diode, LOW);
        delayMicroseconds(10);
        microsecs -= 26;
    }
}

void Form::sendCommand(QString &buttonName)
{
    read_command_from_file(buttonName);
    for (int i = 0; i < num_of_puleses-1; ++i)
    {
    //printf("pulseIR(%d)\n",send_command[i][0]);
    //printf("delayMicroseconds(%d)\n",send_command[i][1]);
        pulseIR(send_command[i][0]);
        delayMicroseconds(send_command[i][1]);
    }
    pulseIR(send_command[num_of_puleses-1][0]);
    printf("IR SIGNAL SENT!\n");
}


void Form::printToFile(uint16_t pulses[][2], uint8_t currentpulse)
{
    FILE *file;
    file = fopen(tab_device_name.toStdString().c_str(), "a+");
    if (file == NULL){
        printf("Error opening the file.\n");
  //      return 1;
    }

    fprintf(file, "int %s[] = {\n",command_name.toStdString().c_str());
    for (uint8_t i = 0; i < currentpulse - 1; i++) {
        fprintf(file, "pulseIR(%d);\n",pulses[i][1] * RESOLUTION);
        fprintf(file, "delayMicroseconds(%d);\n",pulses[i + 1][0] * RESOLUTION);
    }

    fprintf(file, "pulseIR(%d);\n", pulses[currentpulse - 1][1] * RESOLUTION );
    fprintf(file,"}\n");
    fclose(file);
}

void Form::ir_command_read()
{
    uint16_t highpulse, lowpulse;
    highpulse = lowpulse = 0;
    uint16_t pulses[100][2];
    uint8_t currentpulse = 0;

    while (digitalRead(IRpin_sensor)) {
        highpulse++;
        delayMicroseconds(RESOLUTION);

        if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
            printToFile(pulses,currentpulse);
            currentpulse = 0;
            flag_reading = true;
            return;
        }
    }

    pulses[currentpulse][0] = highpulse;

    while (!digitalRead(IRpin_sensor)) {
        lowpulse++;
        delayMicroseconds(RESOLUTION);

        if ((lowpulse >= MAXPULSE) && (currentpulse != 0)) {
            printToFile(pulses,currentpulse);
            currentpulse = 0;
            flag_reading =true;
            return;
        }
    }

    pulses[currentpulse][1] = lowpulse;

    currentpulse++;
}

void Form::load_command(){
    while(flag_reading == false){
        ir_command_read();
    }
    flag_reading=false;
}
