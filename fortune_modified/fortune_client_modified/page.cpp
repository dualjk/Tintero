/* Page gestisce la schermata principale successiva al login, in cui è presente l'icona dell'avatar,
 * l'username dell'utente, l'elenco dei file in ordine alfabetico e la lista dei file in ordine di ultimo accesso.
 * Questa classe gestisce la comunicazione con il server nel momento in cui il client decidesse di compiere una
 * delle seguenti azioni:
 * 1 - creazione di un nuovo documento
 * 2 - apertura di un documento a cui ha accesso
 * 3 - inserzione dell'uri per ottenere l'accesso ad un file
 *
 * Dopo ogni azione viene aperto il text editor. Per tornare alla schermata principale l'utente dovrebbe chiudere il text
 * editor, lanciare nuovamente l'applicazione e quindi rifare l'accesso.
 *
 */


#include "page.h"
#include "ui_page.h"
#include "uridialog.h"

#include <QStandardItemModel>
#define user 0      //giulio = 1, salvo = 0



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
    }

/* Ad ogni login viene rimossa la cartella tmp e viene ricreata */
#if user
    QDir dir("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/");
    dir.removeRecursively();
    if(!QDir("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/").exists())
        QDir().mkdir("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/");

#else
    QDir dir("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/");
    dir.removeRecursively();
    if(!QDir("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/").exists())
        QDir().mkdir("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/");

#endif

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

/* Questa funzione riempie la griglia contenente tutti i documenti a cui ha accesso l'utente
 * tramite l'utlizzo di ClickableFrame
 * Frame = Label con icona + Label con owner + Label con titolo documento (titolo originale)
 */
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


/* Dico al server che ho intenzione di creare un documento.
 */
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

    return te;

}


/*
 * Il server mi risponde con un json contenente il titolo randomico (univoco) che ha generato.
 */
void page::newDocumentCreate(){
    titleDocumentRnd = readJsonNewDocument();
    if(titleDocumentRnd!=nullptr){
#if user
        QFile file("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/"
                       +titleDocumentRnd+".html" );
        file.open(QIODevice::ReadWrite);

        TextEdit *te=textEditStart();
        te->load("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/"
                 +titleDocumentRnd+".html"); //giulio
#else
        QFile file("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/"
                               +titleDocumentRnd+".mp4" );
        file.open(QIODevice::ReadWrite);

        TextEdit *te=textEditStart();
        te->load("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/"
                 +titleDocumentRnd+".mp4"); //salvo
#endif

        te->setCurrentFileName(titleDocumentOriginal);
        te->show();

        this->hide();
    }
    else {
        QMessageBox msgBox(this);
        msgBox.setText("Titolo documento già in uso.");
        msgBox.setInformativeText("Inserisci un altro titolo");
        msgBox.exec();
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
    if(firstTry == true){
        blockSize=0;
        in= new QDataStream(t->getTcpSocket()) ;
        in->setVersion(QDataStream::Qt_5_5);
        in->startTransaction();
    }
    if (blockSize == 0) {
        block.append(t->getTcpSocket()->readAll());
        blockSize = ArrayToInt(block.mid(0, 8));
        block.remove(0, 12);

    }
    if(total < blockSize){
        total += t->getTcpSocket()->bytesAvailable();
        if(firstTry!=true){
            block.append(t->getTcpSocket()->readAll());
        } else
            firstTry=false;
        if(block.size()<blockSize){
            return;
        }else{
            dataReceived(block);
        }
    }
}




void page::dataReceived(QByteArray data) {


#if user

    QSaveFile file("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/"
                   +titleDocumentRnd+".html" ); //giulio

#else

    QSaveFile file("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/"
                   +titleDocumentRnd+".mp4" ); //salvo
#endif


    file.open(QIODevice::WriteOnly);
    file.write(data);
    // Calling commit() is mandatory, otsherwise nothing will be written.
    file.commit();    

    TextEdit *te=textEditStart();
#if user
    te->load("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_client_modified/tmp/"
             +titleDocumentRnd+".html");    //giulio
#else
    te->load("F:/Git/Tintero/fortune_modified/fortune_client_modified/tmp/"
             +titleDocumentRnd+".mp4");    //salvo
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

void page::on_actionOpen_triggered()
{
    UriDialog d(this);
    d.setLabel("Insert here a URI address:");
    if((d.exec() !=  QDialog::Accepted) || d.getUri().isEmpty())
        return;



    QString uri = d.getUri();
    QStringList list = uri.split("/");
    titleDocumentRnd= list.at(1);
    titleDocumentOriginal= list.at(2);

    for(int i=0;i<documentVector->size(); i++) {
        if(documentVector->value(i).getRndTitle() == titleDocumentRnd) {
            QMessageBox msgBox(this);
            msgBox.setText("Il documento è già condiviso");
            msgBox.setInformativeText("Inserisci un altro URI");
            msgBox.exec();
            return;
        }

    }


    QJsonObject title{
        {"action", 4},
        {"user", username},
        {"docTitle", titleDocumentRnd}
    };

    this->t->sendJson(title, "", -1); //questo e' molto brutto ma dovrebbe funzionare

    disconnect(t->getTcpSocket(), &QIODevice::readyRead, 0, 0);
    connect(t->getTcpSocket(), &QIODevice::readyRead, this, &page::readFile);

}
