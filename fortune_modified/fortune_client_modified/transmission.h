#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <QObject>
#include <QtNetwork>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QTcpSocket>

class Transmission : public QObject
{
    Q_OBJECT
public:
    Transmission(QObject *client=nullptr);
    QJsonObject readJson();
    void sendJson(QJsonObject obj, QString address, int port);
    QTcpSocket* getTcpSocket();

private:
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    bool firstConnection=true;







};

#endif // TRANSMISSION_H
