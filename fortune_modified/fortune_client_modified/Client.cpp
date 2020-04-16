/* La classe Client gestisce il login e il sign up del client.
 * Le due schermate sono gestite da uno stackedLayout e i widget per "port"
 * e "address" sono riutilizzati sia da un layout che dall'altro (venogono "riassegnati").
 */


#include <QtNetwork>
#include <QDir>
#include <QTextBrowser>
#include "Client.h"

static const int port = 50505;

Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostCombo(new QComboBox)
    , portLineEdit(new QLineEdit)
    , getFortuneButton(new QPushButton(tr("Login!")))
    , pswLineEdit(new QLineEdit)
    , usernameLineEdit(new QLineEdit)
    , newUserButton(new QPushButton(tr("Not a member? Join us here")))

{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

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
    portLineEdit->setText(QString::number(port));

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

    t = new Transmission(this); // viene creata un'istanza di Transmission per gestire il tcpSocket

    connect(hostCombo, &QComboBox::editTextChanged,
            this, &Client::enableGetFortuneButton);
    connect(portLineEdit, &QLineEdit::textChanged,
            this, &Client::enableGetFortuneButton);
    connect(getFortuneButton, &QAbstractButton::clicked,
            this, &Client::logIn);
    connect(newUserButton, &QAbstractButton::clicked,
            this, &Client::newUser);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);

    connect(t->getTcpSocket(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &Client::displayError);



    QWidget *login = new QWidget();
    mainLayout = new QGridLayout(login);


    for(int i=0; i<12; i++) { //creo un vettore di possibili avatar
        pixmapVector.append(QPixmap(":/img/avatar/"+QString::number(i+1)+".png"));
    }


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
}


void Client::logIn()
{
    getFortuneButton->setEnabled(false);

    QJsonObject authentication{
        {"action", 0},
        {"username", usernameLineEdit->text()},
        {"password", pswLineEdit->text()}
    };

    t->sendJson(authentication,hostCombo->currentText(), portLineEdit->text().toInt());
    disconnect(t->getTcpSocket(), &QIODevice::readyRead, 0, 0);
    connect(t->getTcpSocket(), &QIODevice::readyRead, this, &Client::readJsonLogIn);


}

/*
 * Questa funzione legge il jsonObject tramite l'oggetto t ricevuto dopo il sign up
 * e lo interpreta
 */
void Client::readJsonSignUp(){
    QJsonObject jsonObject = t->readJson();
    int c = jsonObject.value("action").toInt();

    switch (c) {
    case 0:
        appRegLabel->setText("<b>Tintero Client:</b> username già in utilizzo, cambialo animale bestia");
        break;

    case 1:
        backToLoginPage();
        appLabel->setText("<b>Tintero Client:</b> registrazione andata a buon fine");
        break;

    }
}

/*
 * Questa funzione legge il jsonObject tramite l'oggetto t ricevuto dopo il log in
 * e lo interpreta
 */
void Client::readJsonLogIn(){

    QJsonArray jarray = t->readJsonArray();
    QJsonObject jsonObject = jarray.first().toObject();
    int c = jsonObject.value("action").toInt();

    switch (c) {
    case 0:
        appLabel->setText("<b>Tintero Client:</b> username o password errati");
        break;

    case 1:
        /* da implementare le pagina */
        numAvatar=jsonObject.value("avatar").toInt();
        appLabel->setText("<b>Tintero Client:</b> benvenuto "+jsonObject.value("username").toString() +
                          ", avatar: " + QString::number( jsonObject.value("avatar").toInt()));
        toMainPage(jsonObject.value("username").toString(), jarray);

        break;

    }

}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Tintero Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Tintero Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Tintero Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(t->getTcpSocket()->errorString()));
    }

    getFortuneButton->setEnabled(true);
}
\

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


/*
 * Funzioni per switchare pagina tra login e sign up e viceversa
 */
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

/*
 * Funzione per passare alla pagina principale dopo il login.
 * Notare come vengano passati l'oggetto t per permettere le trasmissioni di dati tra processi
 * L'username e la lista dei documenti a cui l'utente ha accesso sottoforma di vector.
 */
void Client::toMainPage(QString username, QJsonArray jarray){
    QVector<Document> *documentList = new QVector<Document>;
    for(int i=1; i<jarray.size(); i++) {
        QJsonObject jobj = jarray.at(i).toObject();
        documentList->append(Document(jobj.value("owner").toString(),
                                      jobj.value("document_original_title").toString(),
                                      jobj.value("document_rnd_title").toString()));
    }

    page *p = new page(this, t, username, documentList);
    p->setAvatar(pixmapVector.at(numAvatar).scaled(128,128, Qt::KeepAspectRatio,Qt::SmoothTransformation));
    p->setGridLayout();
    p->show();
    QDialog::hide();

}


void Client::signUp() {
    if(pswForRegLineEdit->text() == pswRepeatLineEdit->text()) {
        QJsonObject authentication{
            {"action", 1},
            {"username", usernameForRegLineEdit->text()},
            {"password", pswForRegLineEdit->text()},
            {"avatar", numAvatar}
        };


        t->sendJson(authentication, hostCombo->currentText(), portLineEdit->text().toInt());

        disconnect(t->getTcpSocket(), &QIODevice::readyRead, 0, 0);
        connect(t->getTcpSocket(), &QIODevice::readyRead, this, &Client::readJsonSignUp);
    }
    else {
        appRegLabel->setText("<b>Tintero Client:</b> le due password non coincidono, riprova");
    }
}



void Client::avatar() {
    QVector<ClickableLabel*> labels;


    wdg = new QWidget;
    QGridLayout *layout = new QGridLayout(wdg);

    for(int i = 0; i < 12; i++)
    {
        labels.append(new ClickableLabel);
        labels[i]->setValue(i);
        labels[i]->setPixmap(pixmapVector.at(i).scaled(64, 64, Qt::KeepAspectRatio));
        layout->addWidget(labels[i], i/3, i%3);
        connect(labels[i], &ClickableLabel::clicked, this, &Client::labelClicked);
    }

    wdg->show();
}


QJsonValue Client::jsonValFromPixmap(const QPixmap &p) {
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    p.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    return {QLatin1String(encoded)};
}



void Client::labelClicked(){
    ClickableLabel *label = (ClickableLabel*)sender();
    emit selectedAvatar(label->getValue());

}

void Client::selectedAvatar(int avatar){
    numAvatar = avatar;
    avatarLabel->setPixmap(pixmapVector.at(avatar).scaled(100,100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    wdg->hide();
}
