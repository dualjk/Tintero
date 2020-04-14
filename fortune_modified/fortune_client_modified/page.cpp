#include "page.h"
#include "ui_page.h"

#include <QStandardItemModel>

static const int user = 1; //giulio 1 salvo 0


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
        qDebug() << documentVector->value(i).getOwner() + " " + documentVector->value(i).getTitle() + " " +
                    documentVector->value(i).getRndTitle();
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
    ui->usernameLabel->setText("Username: <b>"+username+"</b>");
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

    QVector<QLabel*> docButtons;
    docButtons.append(new QLabel);

    QVector<ClickableFrame*> containerVector;
    containerVector.append(new ClickableFrame);
    ui->scrollArea->setEnabled(true);

    std::sort(documentVector->begin(), documentVector->end(),
              [](Document a, Document b){ return a.getTitle() < b.getTitle();}); //ho scritto la lambda function a first-try

    for(int i = 1; i <= documentVector->size(); i++)
    {
        containerVector.append(new ClickableFrame);


        labels.append(new QLabel);
        if(username == documentVector->value(i-1).getOwner()) {

            labels[i]->setPixmap(QPixmap(":/img/icons/blank.png").scaled(64,64,
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else
            labels[i]->setPixmap(QPixmap(":/img/icons/blankshared.png").scaled(64,64,
                                    Qt::KeepAspectRatio, Qt::SmoothTransformation));
        labels[i]->setAlignment(Qt::AlignCenter);

        ownerLabels.append(new QLabel);
        ownerLabels.at(i)->setText(documentVector->value(i-1).getOwner());

        ownerLabels.at(i)->setAlignment(Qt::AlignCenter);

        docButtons.append(new QLabel);
        docButtons[i]->setText(documentVector->value(i-1).getTitle());
        docButtons.at(i)->setAlignment(Qt::AlignCenter);

        containerVector[i]->setLayout(new QVBoxLayout);
        containerVector[i]->layout()->addWidget(labels[i]);
        containerVector[i]->layout()->addWidget(ownerLabels.at(i));
        containerVector[i]->layout()->addWidget(docButtons[i]);
        containerVector.at(i)->setIndex(i-1);


        ui->docsGridLayout->addWidget(containerVector[i], i/3, i%3);
        connect(containerVector.at(i), &ClickableFrame::clicked, this, &page::documentButtonClicked);
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

    titleDocumentOriginal = d->getDocumentTitle();
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
    QString rndTitle = readJsonNewDocument();
    if(rndTitle!=nullptr){
#if !user
        QFile file("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/"
                       +rndTitle+".html" );
        file.open(QIODevice::ReadWrite);

        TextEdit *te=textEditStart();
        te->load("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/"
                 +rndTitle+".html"); //giulio
#else
        QFile file("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/"
                               +rndTitle+".html" );
        file.open(QIODevice::ReadWrite);

        TextEdit *te=textEditStart();
        te->load("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/"
                 +rndTitle+".html"); //salvo
#endif
        te->setCurrentFileName(titleDocumentOriginal);
        te->show();


        this->hide();

        qDebug() << titleDocumentOriginal;
    }
    else {
        QMessageBox msgBox(this);
        msgBox.setText("Titolo documento già in uso.");
        msgBox.setInformativeText("Inserisci un altro titolo");
        msgBox.exec();
        qDebug()<<"Nome documento già esistente";
    }



}
QString page::readJsonNewDocument(){

    QJsonObject jsonObject = t->readJson();
    int c = jsonObject.value("action").toInt();

    switch (c) {
        case 0:
            return nullptr;
        break;

        case 1:
            return jsonObject.value("rndTitle").toString();
        break;

    }
    return nullptr;
}


void page::documentButtonClicked(){
    qDebug()<<"sono arrivato in documentButtonclicked";
    ClickableFrame *f = (ClickableFrame*)sender();

    QJsonObject title{
        {"action", 3},
        {"user", username},
        {"docTitle", documentVector->value(f->getIndex()).getRndTitle()}
    };

    titleDocumentRnd=documentVector->value(f->getIndex()).getRndTitle();
    titleDocumentOriginal=documentVector->value(f->getIndex()).getTitle();


    this->t->sendJson(title, "", -1); //questo e' molto brutto ma dovrebbe funzionare

    disconnect(t->getTcpSocket(), &QIODevice::readyRead, 0, 0);
    connect(t->getTcpSocket(), &QIODevice::readyRead, this, &page::readFile);

}


void page::readFile(){
    qDebug()<<"sono arrivato in readFile";
//    QByteArray buffer;

//    while (t->getTcpSocket()->bytesAvailable() > 0)
//    {
//        buffer.append(t->getTcpSocket()->readAll());
//    }
//    QByteArray *buffer = new QByteArray;

//        qDebug()<<"arrivo qui 0.5";
//        qDebug()<<"arrivo qui 0.6";
//    qint64 size = 0;
//    qDebug()<<"arrivo qui";
//    while (t->getTcpSocket()->bytesAvailable() > 0)
//    while (buffer->size() < size || size == 0)
//    {
//        buffer->append(t->getTcpSocket()->readAll());

//        qDebug()<<"arrivo qui, byte ricevuti: " +QString::number(buffer->size())+ " di " + QString::number(size);

//            if (size == 0 && buffer->size() >= 8) //if size of data has received completely, then store it on our global variable
//            {
//                size = ArrayToInt(buffer->mid(0, 8));
//                buffer->remove(0, 8);


//            }
//            if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL with the data
//            {
//                QByteArray data = buffer->mid(0, size);

//                //size = 0;
//                emit dataReceived(data);
//                qDebug()<<"arrivo qui 4";
//                disconnect(t->getTcpSocket(), &QIODevice::readyRead, 0,0);
//            }
//        }


//    buffer->remove(0, size);
//    qDebug()<<"nome random: "+titleDocumentRnd;
    QByteArray block;
    int blockSize = 0;
    QDataStream in(t->getTcpSocket());
        in.setVersion(QDataStream::Qt_5_5);

        if (blockSize == 0) {
            // data to read available
            if ( t->getTcpSocket()->bytesAvailable() <
                (int)sizeof(quint64) )
                return;
            in >> blockSize;

        }
        if (t->getTcpSocket()->bytesAvailable() < blockSize)
            return;

        QByteArray nextByte;
        // read
        in >> nextByte;
        nextByte.remove(0,4);

        QByteArray currentByte = nextByte;
        dataReceived(currentByte);

}




void page::dataReceived(QByteArray data) {
#if !user
    QSaveFile file("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/"
                   +titleDocumentRnd+".html" ); //giulio
#else
    QSaveFile file("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/"
                   +titleDocumentRnd+".html" ); //salvo
#endif


    file.open(QIODevice::WriteOnly);
    file.write(data);
    // Calling commit() is mandatory, otsherwise nothing will be written.
    file.commit();    

    TextEdit *te=textEditStart();
#if !user
    te->load("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/"
             +titleDocumentRnd+".html");    //giulio
#else
    te->load("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/"
             +titleDocumentRnd+".html");    //salvo
#endif
    te->setCurrentFileName(titleDocumentOriginal);
    te->show();

    this->hide();
}


qint64 page::ArrayToInt(QByteArray source)
{
    qint64 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}
