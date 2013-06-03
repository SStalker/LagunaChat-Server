#ifndef __ChatterBoxServer_H__
#define __ChatterBoxServer_H__

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>

#include "sqlserver.h"

class ChatterBoxServer : public QTcpServer
{
    Q_OBJECT

    public:
        ChatterBoxServer(QObject *parent=0);

    private slots:
        void readyRead();
        void disconnected();
        void sendUserList();
        void kick();
        void checkError(QAbstractSocket::SocketError);
    protected:
        void incomingConnection(qintptr socketfd);

    private:
        QSet<QTcpSocket*> clients;
        QMap<QString,QTcpSocket*> users;

        QList<QHostAddress> ipAddresses;
        SQLServer *sql;
};

#endif
