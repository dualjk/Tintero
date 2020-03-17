#include <QApplication>

#include "Server.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Server::tr("Fortune Server"));
    Server server;
    server.show();
    return app.exec();
}
