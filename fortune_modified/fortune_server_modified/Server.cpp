#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QSqlRecord>
#include <QDir>

#include "Server.h"

static const int PayloadSize = 20 * 1024; // 64 KB

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

    //! [2]
    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");


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

//! [0] //! [1]
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }
//! [0]
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
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
//! [1]
}

//! [4]
void Server::sendFortune()
{
//! [5]
//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_10);

//    out << fortunes[QRandomGenerator::global()->bounded(fortunes.size())];
////! [4] //! [7]

    clientConnection = tcpServer->nextPendingConnection();
    in.setDevice(clientConnection);
    in.setVersion(QDataStream::Qt_4_0);
//    connect(clientConnection, &QAbstractSocket::disconnected,
//            clientConnection, &QObject::deleteLater);
////! [7] //! [8]

//    clientConnection->write(block);

//! [5]

    //qua ricevo il suca del client
    connect(clientConnection, &QIODevice::readyRead, this, &Server::receive);

}


void Server::receive(){

    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(nextFortune.toLatin1());
    QJsonArray jsonArray = jsonResponse.array();

    if(!jsonArray.isEmpty())
    {
        QJsonObject jsonObject = jsonArray.first().toObject();

        if (!in.commitTransaction())
            return;

        int c = jsonObject.value("action").toInt();
        QString username = jsonObject.value("username").toString();
        QString password = jsonObject.value("password").toString();

        //creazione obj

        QJsonArray jsarray;

        switch (c) {
        case 0: {/* login */
            if(OnSearchClicked(username, password)){
                int avatar = (getAvatarFromDB(username,password));
                statusLabel->setText("A pirate from our crew has returned! Hoorray!\nUsername: " +
                                     username + "\nPassword: " +
                                     password + "\nAvatar scelto: "+QString::number(avatar));

                QJsonObject loginSuccessful{
                    {"action", 1},
                    {"username", username},
                    {"avatar", avatar}
                };
                jsarray.push_back(loginSuccessful);
                DocumentRetrievingByUser(username, jsarray);


            }
            else {
                QJsonObject loginFailed{
                    {"action", 0}
                };
                jsarray.push_back(loginFailed);

                statusLabel->setText("Pirate does not remembah its password: too much rum drunk, bad pirate!");
            }
            sendJsonFromServer(jsarray);
            break;
        }

        case 1: { /* sign up */
            if(Server::UsernameCheckExistance(username)){
                int avatar=jsonObject.value("avatar").toInt();
                Server::DatabasePopulate(username,
                                         password,
                                         avatar);
                statusLabel->setText("A new pirate wants to join our crey! Cheers!\nUsername: " +
                                     username
                                     + "\nPassword: " + password + "\nAvatar scelto: " + QString::number(avatar));


                QJsonObject signUpSuccessful{
                    {"action", 1}
                };
                jsarray.push_back(signUpSuccessful);
            }
            else{
                /* username già esistente */
                QJsonObject signUpFailed{
                    {"action", 0}
                };
                jsarray.push_back(signUpFailed);
            }
            sendJsonFromServer(jsarray);
            break;
        }

        case 2: {
            QString userDoc = jsonObject.value("user").toString();
            QString docname = jsonObject.value("docTitle").toString();
            QString debugdoc = " l'utente " +userDoc + " ha creato il file " + docname;
            QString rndTitle = DocumentInsertion(userDoc, docname);
            if(rndTitle!=nullptr){
                QJsonObject createDocSuccess{
                    {"action", 1},
                    {"rndTitle", rndTitle}
                };
                jsarray.push_back(createDocSuccess);
            }
            else {
                QJsonObject createDocFailed{
                    {"action", 0}
                };
                jsarray.push_back(createDocFailed);
            }

            sendJsonFromServer(jsarray);
            break;
        }

        case 3: {
            QString userDoc = jsonObject.value("user").toString();
            QString docname = jsonObject.value("docTitle").toString();

            DocumentOpening(userDoc, docname);

            break;
        }


        }
    }

}

void Server::DatabaseConnect() {
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_server_modified/database/users.db");    //Giulio
        //db.setDatabaseName("F:/Git/Tintero/fortune_modified/fortune_server_modified/database/users.db");  //Salvo
        //db.setDatabaseName("D:/PDS_project/Tintero/fortune_modified/fortune_server_modified/database/users.db"); //Vale

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}


bool Server::OnSearchClicked(QString username, QString password)
{
    QString sale;
    QSqlQuery query_sale;
    query_sale.prepare("SELECT sale FROM user WHERE username=?;");
    query_sale.addBindValue(username);
    if(!query_sale.exec())
        qWarning() << "MainWindow::OnSearchClicked - ERROR Query Sale: " << query_sale.lastError().text();
    if(query_sale.first()) {
        sale = query_sale.value(0).toString();
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM user WHERE username=? AND password=?;");
    query.addBindValue(username);
    QString passwordHashed = QCryptographicHash::hash((password+sale).toUtf8(), QCryptographicHash::Sha256);
    qDebug()<<passwordHashed;
    query.addBindValue(passwordHashed);

    if(!query.exec())
        qWarning() << "MainWindow::OnSearchClicked - ERROR: " << query.lastError().text();

    if(query.first()) {
        qDebug()<< query.value(0).toString();
        return true;
    }
    else {
        return false;
    }
}


int Server::getAvatarFromDB(QString username, QString password)
{
    QSqlQuery query;
    query.prepare("SELECT avatar FROM user WHERE username=?;");
    query.addBindValue(username);
    //query.addBindValue(password);

    if(!query.exec())
        qWarning() << "MainWindow::OnSearchClicked - ERROR: " << query.lastError().text();

    if(query.first()) {
            return query.value(0).toInt();
    }
}



bool Server::DatabasePopulate(QString username, QString password, int avatar) {
    QSqlQuery query;
    QString sale = GetRandomString();
    QString passwordHashed = QCryptographicHash::hash((password+sale).toUtf8(), QCryptographicHash::Sha256) ;

    qDebug()<<passwordHashed;


    query.prepare("INSERT INTO user(username, password, sale, avatar) VALUES(?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(passwordHashed);
    query.addBindValue(sale);
    query.addBindValue(avatar);


    if(!query.exec()) {
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
        return false;
    }
    else {

        return true;

    }
}

bool Server::UsernameCheckExistance(QString username){
    QSqlQuery query;
    query.prepare("select id from user where username=? ;");
    query.addBindValue(username);

    if(!query.exec())
        qWarning() << " - ERROR: Server::UsernameCheckExistance " << query.lastError().text();

    if(query.first()) {
        qDebug()<< query.value(0).toString();
        return false;
    }
    else {
        qDebug() << "person not found";
        return true;
    }
}


QPixmap Server::pixmapFrom(const QJsonValue &val) {
  auto const encoded = val.toString().toLatin1();
  QPixmap p;
  p.loadFromData(QByteArray::fromBase64(encoded), "PNG");
  return p;
}



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

QJsonValue Server::jsonValFromPixmap(const QPixmap &p) {
  QBuffer buffer;
  buffer.open(QIODevice::WriteOnly);
  p.save(&buffer, "PNG");
  auto const encoded = buffer.data().toBase64();
  return {QLatin1String(encoded)};
}


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



QString Server::DocumentInsertion(QString username, QString document) {
    QSqlQuery query;
    QString titleDocRnd;
    if(DocumentOriginalTitleCheckExistance(document)) {
        do
            titleDocRnd = GetRandomString();
        while (!DocumentRandomTitleCheckExistance(titleDocRnd));

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

            QFile file("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_server_modified/doc/"
                             +titleDocRnd+".html");    //giulio
//            QFile file("F:/Git/Tintero/fortune_modified/fortune_server_modified/doc/"
//                                 +titleDocRnd+".html");    //salvo
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


bool Server::DocumentRandomTitleCheckExistance(QString document){
    QSqlQuery query;
    query.prepare("select document_rnd_title from documents where document_rnd_title=? ;");
    query.addBindValue(document);

    if(!query.exec())
        qWarning() << " - ERROR: Server::DocumentRndTitleCheckExistance " << query.lastError().text();

    if(query.first()) {
//        qDebug()<< query.value(0).toString();
        return false;
    }
    else {
//        qDebug() << "DocumentRndTitle not found";
        return true;
    }
}


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

    while(query.next()) {
    QJsonObject recordObject;

        for(int x=0; x < query.record().count(); x++) {
         recordObject.insert( query.record().fieldName(x), QJsonValue::fromVariant(query.value(x)) );
        }
     array.push_back(recordObject);
    }
    return;

}

bool Server::DocumentOriginalTitleCheckExistance(QString document){
    QSqlQuery query;
    query.prepare("select document_original_title from documents where document_original_title=? ;");
    query.addBindValue(document);

    if(!query.exec())
        qWarning() << " - ERROR: Server::DocumentRndTitleCheckExistance " << query.lastError().text();

    if(query.first()) {
//        qDebug()<< query.value(0).toString();
        return false;
    }
    else {
//        qDebug() << "DocumentRndTitle not found";
        return true;
    }
}


void Server::DocumentOpening(QString username, QString document) {
    //connect(clientConnection, &QIODevice::bytesWritten, this, &Server::updateServerProgress);
    file = new QFile("/Users/giuliodg/Documents/GitHub/Tintero/fortune_modified/fortune_server_modified/doc/"
                     +document+".html");    //giulio
    /*file = new QFile("F:/Git/Tintero/fortune_modified/fortune_server_modified/doc/"
                         +document+".html");    //salvo*/
    if (!file->open(QIODevice::ReadWrite))
    {
        qDebug()<<"Couldn't open the file";
        return;
    }

//    qDebug()<<"sto per mandare il file " + document + " e l'ho anche aperto mamma mia oh";
//    int TotalBytes = file->size();
//    qDebug()<<"il file ha dimensione "+QString::number(TotalBytes);


//    bytesToWrite = TotalBytes - (int)clientConnection->write(file->read(qMin(TotalBytes, PayloadSize)));
//    qDebug()<<"ho inviato "+QString::number(PayloadSize)+" e devo ancora mandare "+ QString::number(bytesToWrite);

//    if(bytesToWrite == 0) {
//        disconnect(clientConnection, SIGNAL(bytesWritten(qint64)), 0, 0);
//        qDebug()<<"ho inviato tutto yay!!";
//    }




    /* secondo metodo */
//    if(clientConnection->state() == QAbstractSocket::ConnectedState)
//        {
//            qint64 sentSize = clientConnection->write(IntToArray(file->size())); //write size of data
//            qDebug()<<"ho inviato la dimensione= "+QString::number(sentSize);
//            qint64 sentData = clientConnection->write(file->readAll()); //write the data itself
//            qDebug()<<"ho inviato questi dati= "+QString::number(sentData);


//        }

    QByteArray block; // Data that will be sent
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    out << (quint64)file->size(); // Space for size of data
    out << file->readAll(); // Actual data
    //out.device()->seek(0);
    //out << (quint64)(block.size() - sizeof(quint64));

    // signal
    connect(clientConnection, SIGNAL(disconnected()),
        clientConnection, SLOT(deleteLater()));
    // write the string into the socket
    clientConnection->write(block);
    qDebug()<<block.size();
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
