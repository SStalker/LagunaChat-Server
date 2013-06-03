#ifndef SQLSERVER_H
#define SQLSERVER_H

#include <QSqlDatabase>
#include <QList>
#include <QMap>
#include <QMultiMap>

class SQLServer
{
public:
    SQLServer();
    bool connect();
    void testQuery();

    void writeRegistration(QString email,QString pw, QString firstname,QString lastname, QString birth);
    void writeInvitation(QString from, QString to, bool transmitted);
    void writeDenyMessage(QString from, QString to, bool transmitted);
    void writeChatMessage(QString from, QString to, QString message, bool transmitted);

    bool deleteFriendship(QString userEmail, QString oldFriendEmail);

    bool checkMail(QString email);
    bool checkLogin(QString email, QString pass);
    bool checkInvitations(QString mail);
    bool checkUnreadMessages(QString user);

    void setOnline(bool b, QString mail);

    bool isOnline(QString mail);
    bool isFriend(QString user, QString newFriend);

    void changeInvitation(QString from, QString to);
    void changeOfflineMessages(QString from);

    void addFriend(QString from, QString to);

    int getUid(QString mail);
    QString getUsername(QString mail);
    QMap<QString, bool> getFriends(QString mail,bool online);
    QMap<QString, bool> getFriends(QString mail);

    QMap<QString,QString> getAskedFriends(QString mail);
    QMultiMap<int,QString> getUnreadMessages(QString user);
private:
    QSqlDatabase db;
};

#endif // SQLSERVER_H
