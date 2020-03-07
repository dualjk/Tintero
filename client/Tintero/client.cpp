#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
}

client::~client()
{
    delete ui;
}

/* TODO: implementare azioni corrispondenti ad ogni tasto */
void client::on_actionNew_triggered()
{

}

void client::on_actionOpen_triggered()
{

}

void client::on_actionExport_triggered()
{

}

void client::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void client::on_actionAbout_Tintero_triggered()
{

}

void client::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void client::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void client::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void client::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}
