#include "doctitledialog.h"
#include "ui_doctitledialog.h"

#include <QDebug>

DocTitleDialog::DocTitleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocTitleDialog)
{
    ui->setupUi(this);

}

DocTitleDialog::~DocTitleDialog()
{
    delete ui;
}


QString DocTitleDialog::getDocumentTitle(){
    return ui->titleLineEdit->text();
}
