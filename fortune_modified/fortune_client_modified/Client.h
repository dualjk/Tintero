#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QtWidgets>
#include <QPixmap>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

#include "clickablelabel.h"
#include "transmission.h"
#include "page.h"
#include "document.h"


QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE


class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void logIn();
    //QJsonObject readJson();
    void readJsonSignUp();
    void readJsonLogIn();

    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetFortuneButton();
    void sessionOpened();
    void newUser();
    void backToLoginPage();
    void signUp();
    void avatar();
    QJsonValue jsonValFromPixmap(const QPixmap &p);
    void labelClicked();
    void selectedAvatar(int avatar);
    void toMainPage(QString username, QJsonArray jarray);

private:

    Transmission *t = nullptr;
    QGridLayout *mainLayout = nullptr;
    QGridLayout *secondLayout = nullptr;

    /* Elementi grafici per la pagina di login */
    QComboBox *hostCombo = nullptr;
    QLineEdit *portLineEdit = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *getFortuneButton = nullptr;
    QString currentFortune;
    QNetworkSession *networkSession = nullptr;
    QLineEdit *pswLineEdit = nullptr;
    QLineEdit *usernameLineEdit = nullptr;
    QLabel *picLabel = nullptr;
    QLabel *appLabel = nullptr;
    QPushButton *newUserButton = nullptr;
    QStackedWidget *stackedWidget = nullptr;


    QLabel *hostLabel = nullptr;
    QLabel *portLabel = nullptr;

    QVector<QPixmap> pixmapVector;


    /* Elementi grafici per la pagina di registrazione */
    QLabel *usernameForRegLabel = nullptr;
    QLineEdit *usernameForRegLineEdit = nullptr;
    QLabel *pswForRegLabel = nullptr;
    QLineEdit *pswForRegLineEdit = nullptr;
    QLabel *pswRepeatLabel = nullptr;
    QLineEdit *pswRepeatLineEdit = nullptr;
    QLabel *picRegLabel = nullptr;
    QLabel *appRegLabel = nullptr;
    QPushButton *backButton = nullptr;
    QPushButton *signUpButton = nullptr;


    QComboBox *hostRegCombo = nullptr;
    QLineEdit *portRegLineEdit = nullptr;

    QPushButton *uploadAvatarButton = nullptr;
    QLineEdit *avatarPathLineEdit=nullptr;
    QLabel *avatarLabel=nullptr;

    bool firstConnection = true;
    QWidget *wdg = nullptr;

    int numAvatar;

    QLabel *avatarUser=nullptr;
    QLabel *usernameLabelMain=nullptr;





};

#endif
