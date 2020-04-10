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
#include <QFile>



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
    bool DatabasePopulate(QString username, QString password, int avatar);
    bool UsernameCheckExistance(QString username);
    QPixmap pixmapFrom(const QJsonValue &val);
    int getAvatarFromDB(QString username, QString password);
    QJsonValue jsonValFromPixmap(const QPixmap &p);

    void sendJsonFromServer(QJsonArray &jsarray);
    QString GetRandomString() const;

    QString DocumentInsertion(QString username, QString document);
    bool DocumentRandomTitleCheckExistance(QString document);
    bool DocumentOriginalTitleCheckExistance(QString document);
    void DocumentRetrievingByUser(QString user, QJsonArray &array);
    void DocumentOpening(QString username, QString document);
    void updateServerProgress();


private:
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
    QNetworkSession *networkSession = nullptr;
    QDataStream in;
    QTcpSocket *clientConnection=nullptr;
    QSqlDatabase db;
    QLabel *piclabel = nullptr;

    QFile *file=nullptr;
    int bytesToWrite = 0;
    int bytesWritten = 0;

};
//! [0]

#endif
