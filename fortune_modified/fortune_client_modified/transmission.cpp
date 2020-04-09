#include "transmission.h"

Transmission::Transmission(QObject *client)
    : tcpSocket(new QTcpSocket(client))
{
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
}

QJsonObject Transmission::readJson()
{
    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(nextFortune.toLatin1());
    QJsonArray jsonArray = jsonResponse.array();


    if (!in.commitTransaction()){
        QJsonObject jsonObjectEmpty{

        };

        return jsonObjectEmpty;
    }

    if(!jsonArray.isEmpty())
    {
        QJsonObject jsonObject = jsonArray.first().toObject();
        return jsonObject;
    }
}

void Transmission::sendJson(QJsonObject obj, QString address, int port) {

    if(firstConnection) {
    tcpSocket->abort();
    tcpSocket->connectToHost(address, port);

    firstConnection=false;
    }


    QJsonArray jsarray {obj};
    QJsonDocument jsDoc(jsarray);

    QString jsString = QString::fromLatin1(jsDoc.toJson());

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << jsString;
    tcpSocket->write(block);
}


QTcpSocket* Transmission::getTcpSocket(){
    return tcpSocket;
}
