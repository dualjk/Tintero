#include "uridialog.h"
#include "ui_uridialog.h"

UriDialog::UriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UriDialog)
{
    ui->setupUi(this);
}

UriDialog::~UriDialog()
{
    delete ui;
}

void UriDialog::setUri(QString s){
    ui->uriTextEdit->setText("tintero/"+s);
    ui->uriTextEdit->setReadOnly(true);

}

void UriDialog::setLabel(QString s){
    ui->uriLabel->setText(s);
}

QString UriDialog::getUri(){
    return ui->uriTextEdit->toPlainText();
}
