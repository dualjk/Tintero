#include "page.h"
#include "ui_page.h"

#include <QStandardItemModel>


page::page(QWidget *parent, Transmission* t, QString username, QVector<Document>* documentVector) :
    QMainWindow(parent),
    ui(new Ui::page),
    username(username),
    t(t),
    documentVector(documentVector)
{
    ui->setupUi(this);
    setUsernameLabel();

    ui->recentTableWidget->setColumnCount(2);
    ui->recentTableWidget->setRowCount(documentVector->size());

    ui->recentTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Title"));
    ui->recentTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Owner"));

    for(int i=0; i<documentVector->size(); i++) {
        QTableWidgetItem *title = new QTableWidgetItem(documentVector->value(i).getTitle());
        ui->recentTableWidget->setItem(i,0, title);
        QTableWidgetItem *owner = new QTableWidgetItem(documentVector->value(i).getOwner());
        ui->recentTableWidget->setItem(i,1, owner);
        qDebug() << documentVector->value(i).getOwner() + " " + documentVector->value(i).getTitle();
    }

}

page::~page()
{
    delete ui;
}

void page::setAvatar(QPixmap p) {

    ui->avatarLabel->setPixmap(p);
    ui->avatarLabel->setAlignment(Qt::AlignCenter);
}

void page::setUsernameLabel() {
    ui->usernameLabel->setText(username);
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

    QVector<QLabel*> ownerLabels;
    ownerLabels.append(new QLabel);

    QVector<QPushButton*> docButtons;
    docButtons.append(new QPushButton);

    QVector<QFrame*> containerVector;
    containerVector.append(new QFrame);
    ui->scrollArea->setEnabled(true);

    std::sort(documentVector->begin(), documentVector->end(),
              [](Document a, Document b){ return a.getTitle() < b.getTitle();}); //ho scritto la lambda function a first-try

    for(int i = 1; i <= documentVector->size(); i++)
    {
        containerVector.append(new QFrame);

        labels.append(new QLabel);
        labels[i]->setPixmap(QPixmap(":/img/icons/blank.png").scaled(64,64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        labels[i]->setAlignment(Qt::AlignCenter);

        ownerLabels.append(new QLabel);
        ownerLabels.at(i)->setText(documentVector->value(i-1).getOwner());
        ownerLabels.at(i)->setAlignment(Qt::AlignCenter);

        docButtons.append(new QPushButton);
        docButtons[i]->setText(documentVector->value(i-1).getTitle());

        containerVector[i]->setLayout(new QVBoxLayout);
        containerVector[i]->layout()->addWidget(labels[i]);
        containerVector[i]->layout()->addWidget(ownerLabels.at(i));
        containerVector[i]->layout()->addWidget(docButtons[i]);

        ui->docsGridLayout->addWidget(containerVector[i], i/3, i%3);
    }



    connect(createDocButton, &QAbstractButton::clicked, this, &page::newDocumentSetup);


}

void page::newDocumentSetup(){

    DocTitleDialog *d = new DocTitleDialog(this);
    if((d->exec() !=  QDialog::Accepted) ||d->getDocumentTitle().isEmpty())
        return;

    QJsonObject title{
        {"action", 2},
        {"user", username},
        {"docTitle", d->getDocumentTitle()}
    };

    this->t->sendJson(title, "", -1); //questo e' molto brutto ma dovrebbe funzionare

    titleDocument = d->getDocumentTitle();
    disconnect(t->getTcpSocket(), &QIODevice::readyRead, 0, 0);
    connect(t->getTcpSocket(), &QIODevice::readyRead, this, &page::newDocumentCreate);

}

TextEdit* page::textEditStart(){
    TextEdit *te = new TextEdit(this);

    const QRect availableGeometry = te->screen()->availableGeometry();
    te->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    te->move((availableGeometry.width() - te->width()) / 2,
            (availableGeometry.height() - te->height()) / 2);

    //t->load(QLatin1String(":/example.html"));
    //t->fileNew();
    return te;

}


void page::newDocumentCreate(){
    if(readJsonNewDocument()){
        TextEdit *te=textEditStart();
        te->fileNew();
        te->setCurrentFileName(titleDocument);
        te->show();


        this->hide();

        qDebug() << titleDocument;
    }
    else {
        QMessageBox msgBox(this);
        msgBox.setText("Titolo documento già in uso.");
        msgBox.setInformativeText("Inserisci un altro titolo");
        msgBox.exec();
        qDebug()<<"Nome documento già esistente";
    }



}
bool page::readJsonNewDocument(){

    QJsonObject jsonObject = t->readJson();
    int c = jsonObject.value("action").toInt();

    switch (c) {
        case 0:
            return false;
        break;

        case 1:
            return true;
        break;

    }
    return false;
}
