#include "sqlserver.h"
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QDebug>
#include <QStringList>

#include <QSqlQuery>

SQLServer::SQLServer()
{
}

bool SQLServer::connect()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Laguna");
    db.setUserName("root");
    db.setPassword("");

    qDebug() << " " << db.drivers();
    return db.open();
}

void SQLServer::testQuery()
{
    QSqlQuery query(db);

    query.exec("CREATE database info");
}

int SQLServer::getUid(QString mail)
{
    qDebug() << "Bekomme die UID aus der Email";

        QSqlQuery query(db);
            query.prepare("SELECT uid FROM users WHERE email=?");
            query.addBindValue(mail);
        query.exec();

        if(query.next())
        {
            return query.value(0).toInt();
        }
        else
        {
            return -1;
        }
}

QString SQLServer::getUsername(QString mail)
{
    qDebug() << "Bekomme den Usernamen aus der Email";

        QSqlQuery query(db);
            query.prepare("SELECT firstname FROM users WHERE email=?");
            query.addBindValue(mail);
        query.exec();

        if(query.next())
        {
            return query.value(0).toString();
        }
        else
        {
            return "YOLO";
        }
}

void SQLServer::writeRegistration(QString email, QString pw, QString firstname, QString lastname, QString birth)
{
    qDebug() << "Schreibe in DB (Registrierung)";

    QSqlQuery query(db);
        query.prepare("INSERT INTO users(uid, email, password,firstname,lastname,birthday) "
                      "VALUES (?, ?, ?, ?, ?, ?)");
        query.addBindValue(0);
        query.addBindValue(email);
        query.addBindValue(pw);
        query.addBindValue(firstname);
        query.addBindValue(lastname);
        query.addBindValue(birth);
    query.exec();
}

bool SQLServer::checkMail(QString email)
{
    qDebug() << "Email-Check: " << email;

    QSqlQuery query(db);

    query.prepare("SELECT email FROM users WHERE email=?");
    query.addBindValue(email);

    bool ok = query.exec();

    qDebug() << "Query was: " << ok << endl << query.lastError();
    qDebug() << "Rows: " << query.size();

    return query.size();
}

bool SQLServer::checkLogin(QString email, QString pass)
{
    qDebug() << "Login-Check: " << email << " # " << pass;

    QSqlQuery query(db);

    query.prepare("SELECT uid FROM users WHERE email=? AND password=?");
    query.addBindValue(email);
    query.addBindValue(pass);

    bool ok = query.exec();

    qDebug() << "Query was: " << ok << endl << query.lastError();
    qDebug() << "Rows: " << query.size();

    return query.size();
}

bool SQLServer::checkInvitations(QString mail)
{
    qDebug() << "Invitation-Check: " << mail;

    QSqlQuery query(db);

    query.prepare("SELECT transmitted FROM added_users WHERE to_user=? AND transmitted=0");
    query.addBindValue(mail);

    bool ok = query.exec();

    qDebug() << "Query was: " << ok << endl << query.lastError();
    qDebug() << "Rows: " << query.size();

    if(query.size() >= 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SQLServer::writeInvitation(QString from, QString to, bool transmitted)
{
    qDebug() << "Schreibe in DB (User hinzufügen)";

    QSqlQuery query(db);
        query.prepare("INSERT INTO added_users(auid, from_user, to_user, transmitted) "
                      "VALUES (?, ?, ?, ?)");
        query.addBindValue(0);
        query.addBindValue(from);
        query.addBindValue(to);
        query.addBindValue(transmitted);
    query.exec();
}

void SQLServer::setOnline(bool b, QString mail)
{
    /*
     * TRUE: Then set the variable to TRUE
     * Client is online
     *
     * FALSE: Then set the variable to FALSE
     * Client is offline
     */

    if(b)
    {
        qDebug() << "Schreibe in DB (User ist online)";

        QSqlQuery query(db);
            query.prepare("UPDATE users SET online=1 WHERE email=:mail");
            query.bindValue(":mail",mail);
        query.exec();

        qDebug() << "SQLERROR: " << query.lastError().databaseText();
    }
    else
    {
        qDebug() << "Schreibe in DB (User ist offline)";

        QSqlQuery query(db);
            query.prepare("UPDATE users SET online=0 WHERE email=:mail");
            query.bindValue(":mail",mail);
        query.exec();

        qDebug() << "SQLERROR: " << query.lastError().databaseText();
    }
}

bool SQLServer::isOnline(QString mail)
{
    bool status;

    QSqlQuery query(db);
        query.prepare("SELECT online FROM users WHERE email=?");
        query.addBindValue(mail);
        query.exec();
    if(query.next())
    {
        status = query.value(0).toBool();
    }

    qDebug() << "Status des Users: " << status;
    return status;
}


QMap<QString, bool> SQLServer::getFriends(QString mail)
{
    QMap<QString, bool> friends;
    QSqlQuery query(db);
        query.prepare("SELECT u.email, u.online FROM users AS u,user_friends AS uf WHERE uf.uid=:uid AND u.uid=uf.fid");
        query.bindValue(":uid",getUid(mail));
        query.exec();

    while(query.next())
    {
        //qDebug() << "SQL: getFriends(" << mail << ")" << query.value(0).toString();
        friends.insert(query.value(0).toString(),query.value(1).toBool());
    }

    qDebug() << "SQLERROR: " << query.lastError().databaseText();

    return friends;
}

QMap<QString, bool> SQLServer::getFriends(QString mail,bool online)
{
    QMap<QString, bool> friends;
    QSqlQuery query(db);
        query.prepare("SELECT u.email, u.online FROM users AS u,user_friends AS uf WHERE uf.uid=:uid AND u.uid=uf.fid AND u.online=:on");
        query.bindValue(":uid",getUid(mail));
        query.bindValue(":on",online);
        query.exec();

    while(query.next())
    {
        friends.insert(query.value(0).toString(),online);
    }

    qDebug() << "SQLERROR: " << query.lastError().databaseText();

    return friends;
}

QMap<QString,QString> SQLServer::getAskedFriends(QString mail)
{
    QMap<QString,QString> newfriends;
    QSqlQuery query(db);
        query.prepare("SELECT u.firstname,au.from_user FROM users AS u, added_users AS au WHERE u.email = au.from_user AND au.to_user=? AND au.transmitted=0");
        query.addBindValue(mail);
        query.exec();

    while(query.next())
    {
        newfriends.insert(query.value(1).toString(),query.value(0).toString());
    }

    return newfriends;
}

bool SQLServer::isFriend(QString user, QString newFriend)
{
    QSqlQuery query(db);
    query.prepare("SELECT uf.uid FROM user_friends AS uf WHERE uf.uid=(SELECT uid FROM users WHERE email=?) AND uf.fid=(SELECT uid FROM users WHERE email=?)");
        query.addBindValue(user);
        query.addBindValue(newFriend);
        query.exec();

        qDebug() << "SQLERROR: " << query.lastError().databaseText();

        if(query.size() == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
}

void SQLServer::changeInvitation(QString from, QString to)
{
    qDebug() << "Update DB (transmitted zu 1)";
    qDebug() << "Parameter 1: " << from << " Parameter 2: " << to;
    QSqlQuery query(db);
    query.prepare("UPDATE added_users SET transmitted=1 WHERE from_user=:from AND to_user=:to");
        query.bindValue(":from",from);
        query.bindValue(":to",to);
    query.exec();

    qDebug() << "SQLERROR: " << query.lastError().databaseText();
    qDebug() << "Query: " << query.lastQuery();
}

void SQLServer::changeOfflineMessages(QString from)
{
    qDebug() << "Update DB (transmitted zu 1)";
    qDebug() << "Parameter 1: " << from;
    QSqlQuery query(db);
    query.prepare("UPDATE messages SET transmitted=1 WHERE to_user=:from");
        query.bindValue(":from",from);
    query.exec();

    qDebug() << "SQLERROR: " << query.lastError().databaseText();
    qDebug() << "Query: " << query.lastQuery();
}

/**
* NOTE: Dies ist eine Methode
*/
void SQLServer::addFriend(QString from, QString to)
{
    qDebug() << "Schreibe in DB (Freund hinzufügen)";

        QSqlQuery query(db);
            query.prepare("INSERT INTO user_friends(uid, fid) "
                          "VALUES (?, ?)");
            query.addBindValue(getUid(from));
            query.addBindValue(getUid(to));
        query.exec();
}

void SQLServer::writeDenyMessage(QString from, QString to, bool transmitted)
{
    qDebug() << "Schreibe in DB (User Deny Message)";

    QSqlQuery query(db);
        query.prepare("INSERT INTO user_deny_messages(udmid, from_user,to_user,transmitted) "
                          "VALUES (?, ?, ?, ?)");
            query.addBindValue(0);
            query.addBindValue(from);
            query.addBindValue(to);
            query.addBindValue(transmitted);
        query.exec();
}

bool SQLServer::deleteFriendship(QString userEmail, QString oldFriendEmail)
{
    qDebug() << "DELETE in DB (Friendship)";
    qDebug() << "Parameter 1: " << userEmail << " Parameter 2: " << oldFriendEmail;
    QSqlQuery query(db);
    query.prepare("DELETE FROM user_friends WHERE uid=:uid AND fid=:fid"); //OR (uid=:fid AND fid=:uid)
        query.bindValue(":uid",getUid(userEmail));
        query.bindValue(":fid",getUid(oldFriendEmail));

    query.exec();

    qDebug() << "SQLERROR: " << query.lastError().databaseText();
    return true;
}

bool SQLServer::checkUnreadMessages(QString user)
{
    qDebug() << "Check in DB (Ungelesene Nachrichten) von " << user;

        QSqlQuery query(db);
            query.prepare("SELECT from_user,to_user,message,arrived FROM messages WHERE to_user=:user AND transmitted=0");
            query.bindValue(":user",user);
        query.exec();

        if(query.size() >= 1)
        {
            return true;
        }
        else
        {
            return false;
        }
}

QMultiMap<int, QString> SQLServer::getUnreadMessages(QString user)
{
    qDebug() << "Lese in DB (Ungelesene Nachrichten) von " << user;

    QMultiMap<int, QString> list;
    int counter = 1;

        QSqlQuery query(db);
            query.prepare("SELECT u.firstname, m.from_user, m.message, DATE_FORMAT(m.arrived,'%d.%m.%Y %H:%i:%S') FROM messages AS m, users AS u WHERE to_user=:user AND transmitted=0 AND m.from_user=u.email");
            query.bindValue(":user",user);
        query.exec();

        while(query.next())
        {
            list.insertMulti(counter,query.value(0).toString());
            list.insertMulti(counter,query.value(1).toString());
            list.insertMulti(counter,query.value(2).toString());
            list.insertMulti(counter,query.value(3).toString());
            counter++;
        }
        qDebug() << "SQLERROR: " << query.lastError().databaseText();
        qDebug() << list.values();

        return list;
}

void SQLServer::writeChatMessage(QString from, QString to, QString message, bool transmitted)
{
    qDebug() << "Schreibe in DB (Chat Message)";

    QSqlQuery query(db);
        query.prepare("INSERT INTO messages(mid, from_user,to_user,message,transmitted) "
                          "VALUES (?, ?, ?, ?, ?)");
            query.addBindValue(0);
            query.addBindValue(from);
            query.addBindValue(to);
            query.addBindValue(message);
            query.addBindValue(transmitted);
        query.exec();

    qDebug() << "SQLERROR: " << query.lastError().databaseText();
}
