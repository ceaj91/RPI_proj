#include "dialog.h"
#include "ui_dialog.h"
#include "form.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    wiringPiSetup();
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void Dialog::on_pushButton_clicked()
{
    bool ok;
    QString tabname = QInputDialog::getText(this,"New Button","Ime novog uredjaja:",QLineEdit::Normal,"",&ok);
    if(ok && !tabname.isEmpty())
    {
        Form* form = new Form(tabname);
        ui->tabWidget->addTab(form,tabname);
        formMap.insert(tabname, form);
        connect(this, &Dialog::activeTabChanged, form, &Form::onActiveTabChanged);
    }
}


void Dialog::on_tabWidget_currentChanged(int index)
{
    //QString tabName = ui->tabWidget->tabText(index);
    //emit activeTabChanged(tabName);
}


void Dialog::on_tabWidget_tabBarClicked(int index)
{
    QString tabName = ui->tabWidget->tabText(index);
    emit activeTabChanged(tabName);
}

