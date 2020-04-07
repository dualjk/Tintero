#include "page.h"
#include "ui_page.h"

page::page(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::page)
{
    ui->setupUi(this);
}

page::~page()
{
    delete ui;
}

void page::setAvatar(QPixmap p) {
    ui->avatarLabel->setPixmap(p);
    ui->avatarLabel->setAlignment(Qt::AlignCenter);
}

void page::setUsernameLabel(QString a) {
    ui->usernameLabel->setText(a);
    ui->usernameLabel->setAlignment(Qt::AlignCenter);
}

void page::setGridLayout(){
    QLabel *newDoc = new QLabel();
    newDoc->setPixmap(QPixmap(":/img/icons/new-file.png").scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    newDoc->setAlignment(Qt::AlignCenter);

    auto createDocButton = new QPushButton();
    createDocButton->setEnabled(true);
    createDocButton->setText("new document");

    QFrame *container = new QFrame();
    container->setLayout(new QVBoxLayout);
    container->layout()->addWidget(newDoc);
    container->layout()->addWidget(createDocButton);

    ui->docsGridLayout->addWidget(container, 0, 0);

    QVector<QLabel*> labels;
    labels.append(new QLabel);

    QVector<QLabel*> docLabels;
    docLabels.append(new QLabel);

    QVector<QFrame*> containerVector;
    containerVector.append(new QFrame);
    ui->scrollArea->setEnabled(true);

    for(int i = 1; i < 30; i++)
    {

        containerVector.append(new QFrame);

        labels.append(new QLabel);
        labels[i]->setPixmap(QPixmap(":/img/icons/blank.png").scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        labels[i]->setAlignment(Qt::AlignCenter);

        docLabels.append(new QLabel);
        docLabels[i]->setText("Doc"+QString::number(i));
        docLabels[i]->setAlignment(Qt::AlignCenter);

        containerVector[i]->setLayout(new QVBoxLayout);
        containerVector[i]->layout()->addWidget(labels[i]);
        containerVector[i]->layout()->addWidget(docLabels[i]);
        containerVector[i]->setStyleSheet("background-color: yellow");
        ui->docsGridLayout->addWidget(containerVector[i], i/3, i%3);
    }



    connect(createDocButton, &QAbstractButton::clicked, this, &page::newDocumentSetup);


}

void page::newDocumentSetup(){
    DocTitleDialog d;
    if(d.exec() !=  QDialog::Accepted)
        return;
    qDebug() << d.getDocumentTitle();

}
