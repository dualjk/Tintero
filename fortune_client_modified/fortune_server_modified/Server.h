#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QDataStream>
#include <QTcpSocket>


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

private:
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
    QVector<QString> fortunes;
    QNetworkSession *networkSession = nullptr;
    QDataStream in;
    QTcpSocket *clientConnection=nullptr;

};
//! [0]

#endif