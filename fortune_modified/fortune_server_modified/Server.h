#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QDataStream>
#include <QTcpSocket>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>


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
    bool DatabasePopulate(QString username, QString password, QPixmap avatar);
    bool UsernameCheckExistance(QString username);
    QPixmap pixmapFrom(const QJsonValue &val);
    QPixmap getAvatarFromDB(QString username, QString password);


private:
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
    QNetworkSession *networkSession = nullptr;
    QDataStream in;
    QTcpSocket *clientConnection=nullptr;
    QSqlDatabase db;
    QLabel *piclabel = nullptr;

};
//! [0]

#endif
