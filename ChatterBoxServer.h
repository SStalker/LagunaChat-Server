#ifndef __ChatterBoxServer_H__
#define __ChatterBoxServer_H__

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QMap>
#include <QSet>

#include "sqlserver.h"

class ChatterBoxServer : public QTcpServer
{
    Q_OBJECT

    public:
        ChatterBoxServer(QObject *parent=0);

        void gotRegistrationMessage(QTcpSocket *client);
        void gotLoginMessage(QTcpSocket *client);
        void gotChatMessage();
        void gotUserAddMessage();
        void gotUserDeleteMessage();
        void gotFileTranserMessage(QTcpSocket *client);

        void hasUserInvitations(QTcpSocket *client);
        void hasUserUnreadMessages(QTcpSocket *client);

        void answerOfUserAdd(QTcpSocket *client);
        void answerOfFileTranser(QTcpSocket *client);
        void answerOfFileTransferReady();

        void userGoOffline();
        void sendUserlist(QTcpSocket *client);

    private slots:
        void readyRead();
        void disconnected();

        void kick();
        void checkError(QAbstractSocket::SocketError);
    protected:
        void incomingConnection(qintptr socketfd);

    private:
        QSet<QTcpSocket*> clients;
        QMap<QString,QTcpSocket*> users;

        QList<QHostAddress> ipAddresses;
        SQLServer *sql;

        QDataStream in;
};

#endif
