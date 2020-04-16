/* Classe "generale" utilizzata dal client per gestire la connessione con il socket
 * e per inviare JsonArray e ricevere JsonObject.
 *
 * Tenere presente che la gerarchia di incapsulamento dei json è la seguente:
 * JsonObject -> JsonArray -> JsonDocument -> QString -> INVIO
 * Pertanto la ricezione di un json prevederà lo "scapsulamento":
 * RICEZIONE -> QString -> JsonDocument -> JsonArray -> JsonObject
 *
 * N.B. Le funzioni contenute in questa classe inviano solo JsonARRAY, ma leggono JsonOBJECT.
 *
 */


#include "transmission.h"

/*
 * Il costruttore di Transmission inizializza un nuovo tcpSocket
 * e istanzia un DataStream che utilizzerà nelle successive trasmissioni.
 *
 */
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

QJsonArray Transmission::readJsonArray()
{
    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(nextFortune.toLatin1());
    QJsonArray jsonArray = jsonResponse.array();


    if (!in.commitTransaction()){
        QJsonObject jsonObjectEmpty{

        };
        jsonArray.push_back(jsonObjectEmpty);
        return jsonArray;
    }

    if(!jsonArray.isEmpty())
    {
        return jsonArray;
    }
}


/* Prendo come parametro un JsonObject, un indirizzo Ip e una porta e invio le informazioni contenute nel JsonObject
 */
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
