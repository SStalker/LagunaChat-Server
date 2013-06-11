#include "LagunaChatServer.h"
#include <QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    LagunaChatServer *server = new LagunaChatServer();

    bool success = server->listen(QHostAddress::Any, 4201);
    if(!success)
    {
        qFatal("Could not listen on port 4201.");
    }

    qDebug() << "Ready";

    return app.exec();
}
