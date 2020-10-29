#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QSqlRecord>
#include <QDir>

#include "Server.h"

static const int PayloadSize = 20 * 1024; // 64 KB
static const int port = 50505;

//costruttore del server
Server::Server(QWidget *parent)
    : QDialog(parent)
    , statusLabel(new QLabel)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
                QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }

    auto quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    //! [3]
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::sendFortune);

    //! [3]


    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = nullptr;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QVBoxLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QVBoxLayout(this);
    }

    piclabel = new QLabel();
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(piclabel);
    mainLayout->addLayout(buttonLayout);

    setWindowTitle(QGuiApplication::applicationDisplayName());


    Server::DatabaseConnect();

}

//stabilisce la connessione con il client
void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }


    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress(ipAddress) , port)) {       //la porta utilizzata è sempre la 50505
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }



    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));

}

// Il server è connesso al client e attende di ricevere dati
void Server::sendFortune()
{
    clientConnection = tcpServer->nextPendingConnection();
    in.setDevice(clientConnection);
    in.setVersion(QDataStream::Qt_4_0);
    connect(clientConnection, &QIODevice::readyRead, this, &Server::receive);

}

// Il server ha ricevuto dei dati dal client sotto forma di json, identifico l'azione richiesta
void Server::receive(){

    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(nextFortune.toLatin1());
    QJsonArray jsonArray = jsonResponse.array();

    if(!jsonArray.isEmpty())    //controllo che ci siano dei dati da leggere
    {
        QJsonObject jsonObject = jsonArray.first().toObject();

        if (!in.commitTransaction())
            return;

        int c = jsonObject.value("action").toInt();                 //il primo campo del json definisce l'azione richiesta

        //creazione obj

        QJsonArray jsarray; //creo l'array che dovrò mandare al client come risposta

        switch (c) {
        case 0: {/* login */
            QString username = jsonObject.value("username").toString(); //salvo il valore username
            QString password = jsonObject.value("password").toString(); //salvo il valore password
            if(OnSearchClicked(username, password)){    //controllo se i dati ricevuti per il login sono corretti
                int avatar = (getAvatarFromDB(username));  //recupero l'avatar dell'utente
                statusLabel->setText("A pirate from our crew has returned! Hoorray!\nUsername: " +
                                     username + "\nPassword: " +
                                     password + "\nAvatar scelto: "+QString::number(avatar));   //aggiorno l'interfaccia grafica

                QJsonObject loginSuccessful{
                    {"action", 1},
                    {"username", username},
                    {"avatar", avatar}
                };  //compilo il json da inviare come conferma al client
                jsarray.push_back(loginSuccessful); //inserisco il json nell'array
                DocumentRetrievingByUser(username, jsarray);    //vado alla funzione che si occupa di mandare al client la lista di file a cui l'utente ha accesso


            }
            else {  //login fallito
                QJsonObject loginFailed{
                    {"action", 0}
                };
                jsarray.push_back(loginFailed);

                statusLabel->setText("Pirate does not remembah its password: too much rum drunk, bad pirate!");
            }
            sendJsonFromServer(jsarray);    //invio il json creato (che indica successo o fallimento) al client
            break;
        }

        case 1: { /* sign up */
            QString username = jsonObject.value("username").toString(); //salvo il valore username
            QString password = jsonObject.value("password").toString(); //salvo il valore password
            int avatar=jsonObject.value("avatar").toInt();  //salvo il valore avatar
            if(DatabasePopulate(username, password, avatar)){   //chiamo la funzione che inserisce il nuovo utente nel database e controllo se ha successo
                statusLabel->setText("A new pirate wants to join our crey! Cheers!\nUsername: " +
                                     username
                                     + "\nPassword: " + password + "\nAvatar scelto: " + QString::number(avatar));  //aggiorno la ui
                QJsonObject signUpSuccessful{
                    {"action", 1}
                };  //creo il json da inviare al client
                jsarray.push_back(signUpSuccessful);    //aggiungo il json all'array
            }
            else{   // username già esistente
                QJsonObject signUpFailed{
                    {"action", 0}
                };  //creo il json da inviare al client
                jsarray.push_back(signUpFailed);    //aggiungo il json all'array
            }
            sendJsonFromServer(jsarray);    //invio il json al client
            break;
        }

        case 2: {   //richiesta di creazione nuovo file
            QString userDoc = jsonObject.value("user").toString();  //salvo il valore username
            QString docname = jsonObject.value("docTitle").toString();  //salvo il valore nome originale file
            QString rndTitle = DocumentInsertion(userDoc, docname); //chiamo la funzione che aggiunge il documento nel database e ritorna il nome random del file
            if(rndTitle!=nullptr){  //controllo che la funzione si sia conclusa con successo
                QJsonObject createDocSuccess{
                    {"action", 1},
                    {"rndTitle", rndTitle}
                };  //creo il json da inviare al client
                jsarray.push_back(createDocSuccess);    //aggiungo il json all'array
            }
            else {  //inserimento del documento nel database concluso con fallimento
                QJsonObject createDocFailed{
                    {"action", 0}
                };
                jsarray.push_back(createDocFailed);
            }

            sendJsonFromServer(jsarray);    //invio il json al client
            break;
        }

        case 3: {   //richiesta di apertura di un file
            QString userDoc = jsonObject.value("user").toString();
            QString docname = jsonObject.value("docTitle").toString();

            DocumentOpening(userDoc, docname);  //chiamo la funzione che invia il file al client

            break;
        }


        case 4: {   //richiesta di aggiunta file condiviso
            QString userDoc = jsonObject.value("user").toString();
            QString docname = jsonObject.value("docTitle").toString();
            if(requestUriSetup(userDoc, docname))   //controllo che l'utente non abbia già accesso al file ed eventualmente aggiorno il db
                DocumentOpening(userDoc, docname);  //se l'aggiunta ha avuto successo, invio il file al client
            break;
        }
        }
    }

}

//funzione che si occupa di aprire la connessione con il database
void Server::DatabaseConnect() {
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        db = QSqlDatabase::addDatabase(DRIVER);
        //db.setDatabaseName("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_server_modified/database/users.db");    //Giulio
        db.setDatabaseName("F:/Git/Tintero/fortune_modified/fortune_server_modified/database/users.db");  //Salvo
        //db.setDatabaseName("D:/PDS_project/Tintero/fortune_modified/fortune_server_modified/database/users.db"); //Vale

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}

//funzione che si occupa di controllare se i dati inviati per il login sono corretti
bool Server::OnSearchClicked(QString username, QString password)
{
    QString sale;
    QSqlQuery query_sale;
    query_sale.prepare("SELECT sale FROM user WHERE username=?;");
    query_sale.addBindValue(username);
    if(!query_sale.exec())  //recupero il sale per l'username con il quale si sta tentando di fare il login
        qWarning() << "MainWindow::OnSearchClicked - ERROR Query Sale: " << query_sale.lastError().text();
    if(query_sale.first()) {
        sale = query_sale.value(0).toString();
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM user WHERE username=? AND password=?;");
    query.addBindValue(username);
    QString passwordHashed = QCryptographicHash::hash((password+sale).toUtf8(), QCryptographicHash::Sha256);    //calcolo l'hash della password inviata dall'utente aggiungendo il sale recuperato dal db
    query.addBindValue(passwordHashed);

    if(!query.exec())
        qWarning() << "MainWindow::OnSearchClicked - ERROR: " << query.lastError().text();

    if(query.first()) { //se la query mi torna una tupla, allora i dati inviati dall'utente sono corretti e permetto di proseguire
        qDebug()<< query.value(0).toString();
        return true;
    }
    else {
        return false;
    }
}

//funzione che si occupa di recuperare l'avatar dell'utente dal db
int Server::getAvatarFromDB(QString username)
{
    QSqlQuery query;
    query.prepare("SELECT avatar FROM user WHERE username=?;");
    query.addBindValue(username);

    if(!query.exec())
        qWarning() << "MainWindow::OnSearchClicked - ERROR: " << query.lastError().text();

    if(query.first()) {
        return query.value(0).toInt();
    }
    else
        return -1;
}


//funzione che si occupa di aggiungere un nuovo utente al database
bool Server::DatabasePopulate(QString username, QString password, int avatar) {
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("select id from user where username=? ;");
    query.addBindValue(username);

    if(!query.exec())
        qWarning() << " - ERROR: Server::UsernameCheckExistance " << query.lastError().text();

    if(query.first()) { //se la query ritorna una tupla vuol dire che l'username era già utilizzato
        QSqlDatabase::database().rollback();
        return false;
    }

    QString sale = GetRandomString();   //chiamo la funzione che genera una stringa casuale da usare come sale
    QString passwordHashed = QCryptographicHash::hash((password+sale).toUtf8(), QCryptographicHash::Sha256) ;   //genero l'hash della password
    query.prepare("INSERT INTO user(username, password, sale, avatar) VALUES(?, ?, ?, ?)"); //inserisco il nuovo utente nel db
    query.addBindValue(username);
    query.addBindValue(passwordHashed);
    query.addBindValue(sale);
    query.addBindValue(avatar);


    if(!query.exec()) {
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
        QSqlDatabase::database().rollback();
        return false;
    }
    else {
        QSqlDatabase::database().commit();
        return true;

    }
}

//funzione da utilizzare per inserire avatar personalizzati
/*QPixmap Server::pixmapFrom(const QJsonValue &val) {
    auto const encoded = val.toString().toLatin1();
    QPixmap p;
    p.loadFromData(QByteArray::fromBase64(encoded), "PNG");
    return p;
}*/


//funzione che si occupa di inviare il json al client attraverso il tcpSocket
void Server::sendJsonFromServer(QJsonArray &jsarray) {
    QJsonDocument jsDoc(jsarray);

    QString jsString = QString::fromLatin1(jsDoc.toJson());

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << jsString;
    clientConnection->write(block);

    return;
}

//funzione da utilizzare per inserire avatar personalizzati
/*QJsonValue Server::jsonValFromPixmap(const QPixmap &p) {
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    p.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    return {QLatin1String(encoded)};
}*/

//funzione che genera una stringa casuale
QString Server::GetRandomString() const
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 12; // assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

//funzione che si occupa di aggiungere un nuovo documento al db
QString Server::DocumentInsertion(QString username, QString document) {
    QSqlQuery query;
    QString titleDocRnd;
    if(DocumentOriginalTitleCheckExistance(document,username)) { //controllo se esiste un documento creato dall'utente con stesso nome originale
        do
            titleDocRnd = GetRandomString();    //genero il nome casuale per il file
        while (!DocumentRandomTitleCheckExistance(titleDocRnd));    //itero finché il nome casuale non è univoco

        query.prepare("INSERT INTO documents(user, document_rnd_title, document_original_title, owner) "
                      "VALUES(?, ?, ?, ?)");
        query.addBindValue(username);
        query.addBindValue(titleDocRnd);
        query.addBindValue(document);
        query.addBindValue(username);

        if(!query.exec()) {
            qWarning() << "MainWindow::DocumentInsertion - ERROR: " << query.lastError().text();
            return nullptr;
        }
        else {
            //se l'inserimento è andato a buon fine scrivo "something" nel file per avere del contenuto
            //QFile file("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_server_modified/doc/"
            //           +titleDocRnd+".html");    //giulio
                        QFile file("F:/Git/Tintero/fortune_modified/fortune_server_modified/doc/"
                                             +titleDocRnd+".");    //salvo
            if ( file.open(QIODevice::ReadWrite) )
            {
                QTextStream stream( &file );
                stream << "something" << endl;
            }
            return titleDocRnd;

        }
    }
    else
        return nullptr;
}

//controllo se il nome casuale è univoco nel db
bool Server::DocumentRandomTitleCheckExistance(QString document){
    QSqlQuery query;
    query.prepare("select document_rnd_title from documents where document_rnd_title=? ;");
    query.addBindValue(document);

    if(!query.exec())
        qWarning() << " - ERROR: Server::DocumentRndTitleCheckExistance " << query.lastError().text();

    if(query.first()) {
        return false;
    }
    else {
        return true;
    }
}

//funzione usata per prendere e inviare al client la lista dei documenti a cui ha accesso
void Server::DocumentRetrievingByUser(QString user, QJsonArray &array){

    QSqlQuery query;
    query.prepare("select document_rnd_title, document_original_title, owner "
                  "from documents where user=? "
                  "order by last_access desc");
    query.addBindValue(user);

    query.setForwardOnly(true);
    if (!query.exec()){
        QJsonObject error{{"error", -1}};
        return;
    }

    while(query.next()) {   //prossima tupla
        QJsonObject recordObject;

        for(int x=0; x < query.record().count(); x++) { //itero tra gli attributi
            recordObject.insert( query.record().fieldName(x), QJsonValue::fromVariant(query.value(x)) );
        }
        array.push_back(recordObject);
    }
    return;

}

bool Server::DocumentOriginalTitleCheckExistance(QString document, QString username){
    QSqlQuery query;
    query.prepare("select document_original_title from documents where document_original_title=? AND owner=? ;");
    query.addBindValue(document);
    query.addBindValue(username);

    if(!query.exec())
        qWarning() << " - ERROR: Server::DocumentOriginalTitleCheckExistance " << query.lastError().text();

    if(query.first()) {
        return false;
    }
    else {
        return true;
    }
}


void Server::DocumentOpening(QString username, QString document) {
    //file = new QFile("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_server_modified/doc/"
    //                 +document+".html");    //giulio
    file = new QFile("F:/Git/Tintero/fortune_modified/fortune_server_modified/doc/"
                         +document+".mp4");    //salvo
    if (!file->open(QIODevice::ReadWrite))
    {
        qDebug()<<"Couldn't open the file";
        return;
    }


    QByteArray block; // Data that will be sent
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    out << (quint64)file->size(); // Space for size of data
    out << file->readAll(); // Actual data

    // signal
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    // write the string into the socket
    clientConnection->write(block);
    // Wait until data are written to the native socket buffer
    clientConnection->waitForBytesWritten();
    return;
}

QByteArray Server::IntToArray(qint64 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}




void Server::updateServerProgress() {

    // only write more if not finished and when the Qt write buffer is below a certain size.
    if (bytesToWrite > 0 && clientConnection->bytesToWrite() <= 4*PayloadSize) {
        bytesToWrite -= (int)clientConnection->write(file->read(qMin(bytesToWrite, PayloadSize)));
        qDebug()<<"ho inviato "+QString::number(PayloadSize)+" e devo ancora mandare "+ QString::number(bytesToWrite);
    }
    if(bytesToWrite == 0) {
        disconnect(clientConnection, SIGNAL(bytesWritten(qint64)), 0, 0);
        qDebug()<<"ho inviato tutto yay!!";
    }

}


bool Server::requestUriSetup(QString username, QString documentRndTitle){
    QSqlDatabase::database().transaction();

    QSqlQuery query;
    query.prepare("select count(*) from documents where document_rnd_title=? and user=?");
    query.addBindValue(documentRndTitle);
    query.addBindValue(username);

    if(!query.exec()) {
        qWarning() << " - ERROR: Server::requestUriSetup #1 " << query.lastError().text();
        QSqlDatabase::database().rollback();
        return false;
    }

    if(query.first()) {
        if(query.value(0)>0) {
            QSqlDatabase::database().rollback();
            return false;
        }

        query.prepare("select document_original_title, owner from documents where document_rnd_title=?");
        query.addBindValue(documentRndTitle);

        if(!query.exec()) {
            qWarning() << " - ERROR: Server::requestUriSetup #2 " << query.lastError().text();
            QSqlDatabase::database().rollback();
        }

        if(query.first()) {
            QString documentOriginalTitle= query.value(0).toString();
            QString owner = query.value(1).toString();
            qDebug()<<documentOriginalTitle << " " << owner;

            query.prepare("INSERT INTO documents(user, document_rnd_title, document_original_title, owner) "
                          "VALUES(?, ?, ?, ?)");
            query.addBindValue(username);
            query.addBindValue(documentRndTitle);
            query.addBindValue(documentOriginalTitle);
            query.addBindValue(owner);

            if(!query.exec()) {
                qWarning() << " - ERROR: Server::requestUriSetup #3 " << query.lastError().text();
                QSqlDatabase::database().rollback();
                return false;
            }
            QSqlDatabase::database().commit();
            return true;
        }
    }

}
