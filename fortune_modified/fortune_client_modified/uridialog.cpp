/*
 * Finestra di dialogo che si apre dopo che un client ha richiesto la generazine dell'URI
 * oppure quando il client richiede di inserire un URI. Se qualcosa dovesse andare storto
 * la classe "page" si occupa di gestire i messaggi di errore.
 */


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

/* La funzione si occupa di settare le label della finestra di dialogo:
 * essendo utilizzata sia per inserire che per generare l'URI, c'è il
 * bisogno di cambiare le label, a seconda dell'utilizzo.
 */
void UriDialog::setLabel(QString s){
    ui->uriLabel->setText(s);
}

QString UriDialog::getUri(){
    return ui->uriTextEdit->toPlainText();
}
