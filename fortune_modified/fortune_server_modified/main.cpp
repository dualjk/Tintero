#include <QApplication>

#include "Server.h"

int main(int argc, char *argv[])
{

    Q_INIT_RESOURCE(server);

    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Server::tr("Fortune Server"));
    Server server;
    server.show();
    return app.exec();
}
