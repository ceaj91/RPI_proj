#include "dialog.h"
#include "ui_dialog.h"
#include "form.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
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
        ui->tabWidget->addTab(new Form(),tabname);
    }
}

