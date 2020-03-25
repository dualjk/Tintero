#include <QtWidgets>
#include <QtNetwork>
#include <QDir>

#include "Client.h"

//! [0]
Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostCombo(new QComboBox)
    , portLineEdit(new QLineEdit)
    , getFortuneButton(new QPushButton(tr("Login!")))
    , newUserButton(new QPushButton(tr("Not a member? Join us here")))
    , tcpSocket(new QTcpSocket(this))
    , pswLineEdit(new QLineEdit)
    , usernameLineEdit(new QLineEdit)


{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
//! [0]
    hostCombo->setEditable(true);
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    /*Comando per oscurare la password durante l'inserimento*/
    pswLineEdit->setEchoMode(QLineEdit::Password);

    hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostCombo);
    portLabel = new QLabel(tr("&Server port:"));
    portLabel->setBuddy(portLineEdit);
    auto usernameLabel = new QLabel(tr("&Username:"));
    usernameLabel->setBuddy(usernameLineEdit);
    auto pswLabel = new QLabel(tr("Password:"));
    usernameLabel->setBuddy(pswLineEdit);

    statusLabel = new QLabel(tr("This examples requires that you run the "
                                "<s>Fortune Server</s> Tintero Server example as well."));

    QString tmp_picPath = ":/img/ink.png";
    picLabel = new QLabel(tr(""));
    appLabel = new QLabel(tr("<b>Tintero Client</b>: esplora con noi il magico mondo del c++\n"
                             "Speriamo di prendere 30."));
    QPixmap pix(tmp_picPath);
    picLabel->setPixmap(pix.scaled(128,128, Qt::KeepAspectRatio));

    picRegLabel = new QLabel(tr(""));
    appRegLabel = new QLabel(tr("<b>Tintero Client</b>: esplora con noi il magico mondo del c++\n"
                             "Speriamo di prendere 30."));
    QPixmap pixReg(tmp_picPath);
    picRegLabel->setPixmap(pixReg.scaled(128,128, Qt::KeepAspectRatio));





    getFortuneButton->setDefault(true);
    getFortuneButton->setEnabled(false);

    auto quitButton = new QPushButton(tr("Quit"));
    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getFortuneButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

//! [1]
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
//! [1]

    connect(hostCombo, &QComboBox::editTextChanged,
            this, &Client::enableGetFortuneButton);
    connect(portLineEdit, &QLineEdit::textChanged,
            this, &Client::enableGetFortuneButton);
    connect(getFortuneButton, &QAbstractButton::clicked,
            this, &Client::requestNewFortune);
    connect(newUserButton, &QAbstractButton::clicked,
            this, &Client::newUser);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);

    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
//! [3]
            this, &Client::displayError);
//! [4]



    QWidget *login = new QWidget();
    mainLayout = new QGridLayout(login);
/*    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QGridLayout(this);
    }*/



    mainLayout->addWidget(picLabel, 0, 0);
    mainLayout->addWidget(appLabel, 0, 1);
    mainLayout->addWidget(hostLabel, 1, 0);
    mainLayout->addWidget(hostCombo, 1, 1);
    mainLayout->addWidget(portLabel, 2, 0);
    mainLayout->addWidget(portLineEdit,2, 1);
    mainLayout->addWidget(usernameLabel,3,0);
    mainLayout->addWidget(usernameLineEdit,3,1);
    mainLayout->addWidget(pswLabel,4,0);
    mainLayout->addWidget(pswLineEdit,4,1);
    mainLayout->addWidget(newUserButton, 5, 0);
    mainLayout->addWidget(statusLabel, 6, 0, 1, 6);
    mainLayout->addWidget(buttonBox, 7, 0, 1, 7);


    // da qui inizia la seconda pagina
    QWidget *reg = new QWidget();
    secondLayout = new QGridLayout(reg);


    usernameForRegLabel = new QLabel(tr("&Username: "));
    usernameForRegLineEdit =  new QLineEdit;
    usernameForRegLabel->setBuddy(usernameForRegLineEdit);

    pswForRegLabel = new QLabel(tr("&Password: "));
    pswForRegLineEdit = new QLineEdit;
    pswForRegLabel->setBuddy(usernameForRegLineEdit);

    pswRepeatLabel = new QLabel(tr("&Repeat your password:"));
    pswRepeatLineEdit = new QLineEdit();
    pswRepeatLabel->setBuddy(pswRepeatLineEdit);

    backButton = new QPushButton(tr("Back"));
    backButton->setDefault(true);
    backButton->setEnabled(true);

    auto quitRegButton = new QPushButton(tr("Quit"));
    auto buttonRegBox = new QDialogButtonBox;
    buttonRegBox->addButton(backButton, QDialogButtonBox::ActionRole);
    buttonRegBox->addButton(quitRegButton, QDialogButtonBox::RejectRole);
    signUpButton = new QPushButton(tr("Join the crew"));
    signUpButton->setEnabled(false);

    avatarLabel = new QLabel(tr("Avatar, pirate!"));
    avatarPathLineEdit = new QLineEdit();
    uploadAvatarButton=new QPushButton(tr("Browse"));



    secondLayout->addWidget(picRegLabel, 0, 0);
    secondLayout->addWidget(appRegLabel, 0, 1);

    secondLayout->addWidget(usernameForRegLabel,3,0);
    secondLayout->addWidget(usernameForRegLineEdit,3,1);
    secondLayout->addWidget(pswForRegLabel,4,0);
    secondLayout->addWidget(pswForRegLineEdit,4,1);
    secondLayout->addWidget(pswRepeatLabel, 5, 0);
    secondLayout->addWidget(pswRepeatLineEdit, 5, 1);
    secondLayout->addWidget(avatarLabel, 7,0);
    secondLayout->addWidget(avatarPathLineEdit, 7, 1);
    secondLayout->addWidget(uploadAvatarButton, 7, 2);

    secondLayout->addWidget(signUpButton, 8,1);

    secondLayout->addWidget(buttonRegBox, 9, 0, 1, 9);

    pswForRegLineEdit->setEchoMode(QLineEdit::Password);
    pswRepeatLineEdit->setEchoMode(QLineEdit::Password);


    connect(quitRegButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(backButton, &QAbstractButton::clicked,
            this, &Client::backToLoginPage);
    connect(signUpButton, &QAbstractButton::clicked,
            this, &Client::signUp);
    connect(uploadAvatarButton, &QAbstractButton::clicked,
            this, &Client::avatar);






    stackedWidget = new QStackedWidget();

    stackedWidget->addWidget(login);
    stackedWidget->addWidget(reg);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(stackedWidget);
    setLayout(layout);





    setWindowTitle(QGuiApplication::applicationDisplayName());
    portLineEdit->setFocus();

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
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        getFortuneButton->setEnabled(false);
        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
//! [5]
}
//! [5]

//! [6]
void Client::requestNewFortune()
{
    getFortuneButton->setEnabled(false);
    tcpSocket->abort();
//! [7]
    tcpSocket->connectToHost(hostCombo->currentText(),
                             portLineEdit->text().toInt());
//! [7]
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << "0;"+usernameLineEdit->text() + ";" + pswLineEdit->text();
    tcpSocket->write(block);


}
//! [6]

//! [8]
void Client::readFortune()
{
//    in.startTransaction();

//    QString nextFortune;
//    in >> nextFortune;

//    if (!in.commitTransaction())
//        return;

//    if (nextFortune == currentFortune) {
//        QTimer::singleShot(0, this, &Client::requestNewFortune);
//        return;
//    }

//    currentFortune = nextFortune;
//    statusLabel->setText(currentFortune);
//    getFortuneButton->setEnabled(true);



    //qua mando le cose indietro


}
//! [8]

//! [13]
void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("<s>Fortune Client</s> Tintero Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    getFortuneButton->setEnabled(true);
}
//! [13]

void Client::enableGetFortuneButton()
{
    getFortuneButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostCombo->currentText().isEmpty() &&
                                 !portLineEdit->text().isEmpty());

    signUpButton->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostCombo->currentText().isEmpty() &&
                                 !portLineEdit->text().isEmpty());

}

void Client::sessionOpened()
{
    // Save the used configuration
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

    statusLabel->setText(tr("This examples requires that you run the "
                            "Fortune Server example as well."));

    enableGetFortuneButton();
}


void Client::newUser(){

    secondLayout->addWidget(hostLabel, 1, 0);
    secondLayout->addWidget(hostCombo, 1, 1);
    secondLayout->addWidget(portLabel, 2, 0);
    secondLayout->addWidget(portLineEdit,2, 1);

    stackedWidget->setCurrentIndex(1);

}


void Client::backToLoginPage(){

    mainLayout->addWidget(hostLabel, 1, 0);
    mainLayout->addWidget(hostCombo, 1, 1);
    mainLayout->addWidget(portLabel, 2, 0);
    mainLayout->addWidget(portLineEdit,2, 1);

    stackedWidget->setCurrentIndex(0);
}


void Client::signUp() {
    if(pswForRegLineEdit->text() == pswRepeatLineEdit->text()) {
        tcpSocket->abort();
    //! [7]
        tcpSocket->connectToHost(hostCombo->currentText(),
                                 portLineEdit->text().toInt());
    //! [7]
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_10);

        out << "1;" + usernameForRegLineEdit->text() + ";" + pswForRegLineEdit->text();
        tcpSocket->write(block);



    }
    else {

        qDebug()<<"non bravo";
    }
}



void Client::avatar() {
    QString s1 = QFileDialog::getOpenFileName(this, "Open a file", "directoryToOpen",
            "Images (*.png *.xpm *.jpg)");
    avatarPathLineEdit->setText(s1);
    qDebug() << s1;
}
