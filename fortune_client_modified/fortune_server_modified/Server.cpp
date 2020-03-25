#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>


#include "Server.h"

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

    mainLayout->addWidget(statusLabel);
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

        if (!in.commitTransaction())
            return;

        QStringList credentials = nextFortune.split(';', QString::SkipEmptyParts);



        QString c = credentials.at(0);
        switch (c.toInt()) {
            case 0: /* login */
                if(OnSearchClicked(credentials.at(1), credentials.at(2))){
                    statusLabel->setText("A pirate from our crew has returned! Hoorray!\nUsername: " +
                                     credentials.at(1) + "\nPassword: " + credentials.at(2));
                }
                else {
                    statusLabel->setText("Pirate does not remembah its password: too much rum drunk, bad pirate!");
                }
            break;

            case 1: /* sign up */
            if(Server::UsernameCheckExistance(credentials.at(1))){

                Server::DatabasePopulate(credentials.at(1), credentials.at(2));
                statusLabel->setText("A new pirate wants to join our crey! Cheers!\nUsername: " +
                                     credentials.at(1) + "\nPassword: " + credentials.at(2));

            }
            else{
                /* username già esistente */
                qDebug()<<"Username già esistente, per favore implementami bene. Mai per comando.";
            }

            break;


        }


        connect(clientConnection, &QIODevice::readyRead, this, &Server::receive);
        Server::clientConnection->disconnectFromHost();

}

void Server::DatabaseConnect() {
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName(QDir::homePath()+"/fortune_server_modified/database/users.db");

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}


bool Server::OnSearchClicked(QString username, QString password)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM user WHERE username=? AND password=?;");
    query.addBindValue(username);
    query.addBindValue(password);

    if(!query.exec())
        qWarning() << "MainWindow::OnSearchClicked - ERROR: " << query.lastError().text();

    if(query.first()) {
        qDebug()<< query.value(0).toString();
        return true;
    }
    else {
        qDebug() << "person not found";
        return false;
    }




}

bool Server::DatabasePopulate(QString username, QString password) {
    QSqlQuery query;
    query.prepare("INSERT INTO user(username, password, avatar) VALUES(?, ?, null)");
    query.addBindValue(username);
    query.addBindValue(password);

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

