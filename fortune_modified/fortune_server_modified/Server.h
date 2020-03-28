#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QDataStream>
#include <QTcpSocket>
#include <QPixmap>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>



QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

//! [0]
class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);

private slots:
    void sessionOpened();
    void sendFortune();
    void receive();

    void DatabaseConnect();
    bool OnSearchClicked(QString username, QString password);
    bool DatabasePopulate(QString username, QString password);
    bool UsernameCheckExistance(QString username);
<<<<<<< HEAD
    QPixmap pixmapFrom(const QJsonValue &val);
    QPixmap getAvatarFromDB(QString username, QString password);
    QJsonValue jsonValFromPixmap(const QPixmap &p);
=======
>>>>>>> parent of 7a61e7d... Aggiunto supporto a Json (login, sign up, avatar)

    void sendJsonFromServer(const QJsonObject &obj);
    QString GetRandomString() const;

private:
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
    QNetworkSession *networkSession = nullptr;
    QDataStream in;
    QTcpSocket *clientConnection=nullptr;
    QSqlDatabase db;

};
//! [0]

#endif
