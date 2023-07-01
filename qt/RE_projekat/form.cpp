#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
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
        ui->verticalLayout->addWidget(newbutton);
    }
}


