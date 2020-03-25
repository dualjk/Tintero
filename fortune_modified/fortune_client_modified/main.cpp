#include <QApplication>
#include "Client.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(client);

    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Client::tr("Tintero Client"));
    Client client;
    client.show();
    return app.exec();
}
