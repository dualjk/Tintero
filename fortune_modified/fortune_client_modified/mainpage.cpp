#include "mainpage.h"


void Ui_MainWindow::setUsernameLabel(QString username){

    this->usernameLabel->setText("<b>"+username+"</b>");


}

void Ui_MainWindow::setAvatarLabel(QPixmap avatar){
    this->avatarLabel->setPixmap(avatar);

}


void Ui_MainWindow::setNewDocumentIcon(){
    QLabel *newDoc = new QLabel();
    newDoc->setPixmap(QPixmap(":/img/icons/new-file.png").scaled(64,64, Qt::KeepAspectRatio));

    this->docTableWdg->addWidget(newDoc, 0, 0);
    QVector<QLabel*> labels;

    labels.append(new QLabel);

    for(int i = 1; i < 12; i++)
    {
        labels.append(new QLabel);
        labels[i]->setPixmap(QPixmap(":/img/icons/blank.png").scaled(64,64, Qt::KeepAspectRatio));
        docTableWdg->addWidget(labels[i], i/3, i%3);
    }


}
