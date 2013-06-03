#include "ChatterBoxServer.h"
#include "userlisttext.h"
#include "sqlserver.h"

#include <QTcpSocket>
#include <QRegExp>
#include <QTime>
#include <QDateTime>
#include <QDataStream>

#include <QMap>
#include <QMapIterator>
#include <QMultiMap>

ChatterBoxServer::ChatterBoxServer(QObject *parent) : QTcpServer(parent)
{
    sql = new SQLServer();

    bool ok = sql->connect();

    qDebug() << "SQL Connection established: " << ok;

    connect(this,SIGNAL(acceptError(QAbstractSocket::SocketError)),this,SLOT(checkError(QAbstractSocket::SocketError)));
}

void ChatterBoxServer::checkError(QAbstractSocket::SocketError error)
{
    qDebug() << "Fehler: " << error;
}

void ChatterBoxServer::incomingConnection(qintptr socketfd)
{
    qDebug() << "Juhuu";
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    //If the first Client connects for the first time
    if(clients.empty())
    {
        //than insert the client
        qDebug() << "First Client";
        clients.insert(client);
        //ipAddresses << client->peerAddress();
        qDebug() << "New client from:" << client->peerAddress().toString();
        connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }
    //if the sendersocket is different than the sockets in the list
    else if(clients.size() > 0)
    {
        // check the lists ips with the senderip
        // than
        // insert the client

        foreach(QHostAddress address, ipAddresses)
        {
            if(client->peerAddress().toString() != address.toString())
            {
                clients.insert(client);
                qDebug() << "IP stimmt nicht überein";
                qDebug() << "New client from:" << client->peerAddress().toString();
                connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
                connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
				break;
            }
            // if sendersocket is already in the list
            // ignore the request and disconnect the socket
            else if(client->peerAddress().toString() == address.toString())
            {
                qDebug("IP stimmt überein");
                //client->write(QString("Server: Already logged in\n").toUtf8());
                client->close();
				return;
                //ipAddresses.removeLast();
            }
        }
    }
	ipAddresses.append(client->peerAddress());
    foreach(QHostAddress address, ipAddresses)
    {
        qDebug() << address.toString() << endl;
    }
}

void ChatterBoxServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine())
    {
        qDebug() << endl << endl << "##############################################";
        QDataStream in(client);

        int messageID;
        QString temp;
        if( client->bytesAvailable() < sizeof( messageID ) ) return;
        in >> messageID;

        //char *debug;
        //client->read(debug,client->bytesAvailable());
        //qDebug() << "DEBUG: " << QString(debug);
        qDebug() << "Current messageID: " << messageID << endl << "Status of Datastream: " << in.status();
        qDebug() << "Bytes there: " << client->bytesAvailable() << endl;
        if(messageID == 1)
        {
            QString email;
            QString password;
            QString username;

            in >> email;
            in >> username;
            in >> password;

            qDebug() << email << username << password;

            if(sql->checkMail(email))
            {
                qDebug() << "Email existiert schon\nFehler an Client senden";
                QDataStream out(client);
                out << (int) 0;
                out << (int) 0;
                out << "\n";
            }
            else
            {
                sql->writeRegistration(email,password,username,"","");
                //send successful status message
            }
            qDebug() << "Email: " << email << endl << "Username: " << username << endl << "Password: " << password;

            //return;

        }
        else if(messageID == 2)
        {

        }
        else if(messageID == 3)
        {
            QString fromMail;
            QString toMail;
            QString room;
            QString message;

            in >> fromMail;
            in >> toMail;
            in >> room;
            in >> message;

            qDebug() << "A message from " << fromMail << " to " << toMail << endl
                     << "Room: " << room << endl
                     << "Message: " << message;

            // so let us look if the "toMail"-User is online
            if(sql->isOnline(toMail))
            {
                // send message direct to user
                QTcpSocket *to_friend = users.value(toMail);
                QDataStream out(to_friend);
                QDateTime t = QDateTime::currentDateTime();
				QString text = t.toString("dd.MM.yyyy hh:mm:ss") + "@" + sql->getUsername(fromMail) + ": " + message;
                qDebug() << "Size of Message: " << text.size();
                out << (int) 3;
                out << fromMail;
                out << toMail;
                out << room;
                out << text;
                out << "\n";

                // and write the message into DB
                sql->writeChatMessage(fromMail,toMail,message,1);
            }
            else
            {
                // save message into DB
                sql->writeChatMessage(fromMail,toMail,message,0);
            }
        }
        else if(messageID == 4)
        {
            QString email;
            QString password;

            qDebug() << "Logindata:";

            in >> email;
            in >> password;

            qDebug() << "Email: " << email << endl << "Password: " << password;
            qDebug() << "DEBUG: " << temp;

            if(sql->checkLogin(email,password))
            {
                qDebug() << "Username or password is right. Send authentification";

                // write email as key value
                users[email] = client;

                // update DB User is online
                sql->setOnline(1,email);

                // get username to send him to user
                QString name = sql->getUsername(email);
                qDebug() << users.values();
                QDataStream out1(client);
                out1 << (int) 2;
                out1 << (int) 1;
                out1 << name;
                out1 << "\n";

                // send notification to all friends(who is online), so they know that the user is online
                QMap<QString, bool> map = sql->getFriends(email,1);
                QMapIterator<QString, bool> i(map);

                while(i.hasNext())
                {
                    i.next();
                    QTcpSocket *friends = users.value(i.key());
                    //now send
                    QDataStream out(friends);
                    out << (int) 7;
                    out << (int) 2;
                    out << email; // Email des Onlinegegangenen Users
                    out << "\n";
                }
            }
            else
            {
                qDebug() << "Username or password is wrong. Send error message";
                QDataStream out(client);
                out << (int) 0;
                out << (int) 1;
                out << "\n";
            }
            //qDebug() << "Username: " << username << endl << "Password: " << password;

        }
        else if(messageID == 5)
        {
            int questID;
            in >> questID;

            qDebug() << "QuestID: " << questID;
            if(questID == 1)
            {
                QString usersFrom;
                //get all users(friends) from username
                in >> usersFrom;
                qDebug() << "User: " << usersFrom << " wants his friendlist";

                QMap<QString, bool> list = sql->getFriends(usersFrom);
                qDebug() << "Sende Liste mit: " << list.size() << " Usern.";
                QDataStream out(client);
                out << (int) 6;
                out << list;
                out << "\n";

                //Send Userlist with all Users

            }
            else if(questID == 2)
            {
                QString from;
                QString to;

                in >> from;
                in >> to;

                qDebug() << "UserADD Request: " << from << " # " << to;

                //WICHTIG
                // Prüfen ob der User nicht schon Freund ist
                if(sql->isOnline(to))
                {
                    if(sql->checkInvitations(to))
                    {
                        // send invitation message to user
                        sql->writeInvitation(from,to,1);
                        QMap<QString,QString> listNewFriends = sql->getAskedFriends(to);
                        qDebug() << "Sende Liste mit: " << listNewFriends.size() << " Usern.";
                        QDataStream out(users.value(to));
                        out << (int) 5;
                        out << (int) 3;
                        out << listNewFriends;
                        out << "\n";
                    }
					
					 sql->changeInvitation(from,to);
                }
                else
                {
                    //write the undone job in DB
                    sql->writeInvitation(from,to,0);
                }
            }
            else if(questID == 4)
            {
                QString email;
                in >> email;

                // if user has invitation notes in DB
                if(sql->checkInvitations(email))
                {
                    // send invitation message to user
                    QMap<QString,QString> listNewFriends = sql->getAskedFriends(email);
                    qDebug() << "Sende Liste mit: " << listNewFriends.size() << " Usern.";
                    QDataStream out(client);
                    out << (int) 5;
                    out << (int) 3;
                    out << listNewFriends;
                    out << "\n";
                }
                else
                {
                    // do nothing and go on
                    // send better an message

                    QDataStream out(client);
                    out << (int) 5;
                    out << (int) 9;
                    out << "\n";
                }

            }
            else if(questID == 5)
            {
                QString userEmail;
                QString friendEmail;
                int ok;

                qDebug() << "Clientanfrage for new User";

                in >> userEmail;
                in >> friendEmail;
                in >> ok;

                qDebug() << userEmail << " -> " << friendEmail;
                qDebug() << "Status: " << ok;
                if(!sql->isFriend(userEmail,friendEmail) || !sql->isFriend(friendEmail,userEmail))
                {
                    if(ok == 1)
                    {
                        qDebug() << "Gefragter User sagt JA";
                        // write new friend to user;
                        // and the user to the friend
                        sql->addFriend(userEmail,friendEmail);
                        sql->addFriend(friendEmail,userEmail);
                        // set invitatation query to 1
                        sql->changeInvitation(friendEmail,userEmail);

                         // send new userlist to user
                         QDataStream out(client);
                         out << (int) 5;
                         out << (int) 6;
                         out << friendEmail;
                         out << "\n";

                         // and if online to the friend
                         if(sql->isOnline(friendEmail))
                         {
                             QTcpSocket *fr = users.value(friendEmail);
                             QDataStream out1(fr);

                             out1 << (int) 5;
                             out1 << (int) 6;
                             out1 << userEmail;
                             out1 << "\n";
                         }
                         else
                         {
                             // write an entry in DB (authorize)
                         }

                    }
                    else
                    {
                        qDebug() << "Gefragter User sagt NEIN";
                        // set invitation query to 1
                        sql->changeInvitation(friendEmail,userEmail);
                        // send deny message to friend if online
                        // else write job in DB
                        if(sql->isOnline(friendEmail))
                        {
                            QDataStream out(users.value(friendEmail));
                            out << (int) 7;
                            out << (int) 1;
                            out << friendEmail;
                            out << "\n";
                        }
                        else
                        {
                            qDebug() << "User offline. Write message into DB";
                            sql->writeDenyMessage(userEmail,friendEmail,0);
                        }


                    }
                }
                else
                {
                    // antwort: ist schon freund
                }
            }
            else if(questID == 6)
            {

            }
            else if(questID == 7)
            {
                QString userEmail;
                QString oldFriendEmail;

                qDebug() << "Clientanfrage for deleting a User(Friend)";

                in >> userEmail;
                in >> oldFriendEmail;

                qDebug() << userEmail << " -> " << oldFriendEmail;
                if(!sql->isFriend(userEmail,oldFriendEmail))
                {
                    // why delete when they arent friends
                    qDebug() << "DELETE FRIENDSHIP: They were not friends";
                }
                else
                {
                    // UPDATE DB
                    qDebug() << "DELETE FRIENDSHIP: They were friends";
                    qDebug() << "DELETE ? " << sql->deleteFriendship(userEmail,oldFriendEmail);

                    // Message back to see if successfull
                }
            }
            else if(questID == 8)
            {
                // the user wants unreaded offline usermessages
                qDebug() << "The user wants unreaded messages";
                QString user;
                in >> user;
                if(sql->checkUnreadMessages(user))
                {
                    qDebug() << "New messages are ready to send";
                    // neue nachrichten
                    QMultiMap<int, QString> list = sql->getUnreadMessages(user);
                    QDataStream out(client);

                    out << (int) 7;
                    out << (int) 4;
                    out << list;
                    out << "\n";

                    // set arrived flag to 1
                    sql->changeOfflineMessages(user);

                }
                else
                {
                    qDebug() << "No new messages";
                }
            }
        }
        else if(messageID == 6){}
        else if(messageID == 7)
        {
            int answerID;
            QString user;

            in >> answerID;

            if(answerID == 3)
            {
                // User is now offline
                // Update DB
                in >> user;
                sql->setOnline(false,user);

                // send notification to all friends(who is online), so they know that the user is online
                QMap<QString, bool> map = sql->getFriends(user,1);
                QMapIterator<QString, bool> i(map);

                while(i.hasNext())
                {
                    i.next();
                    QTcpSocket *friends = users.value(i.key());
                    //now send
                    QDataStream out(friends);
                    out << (int) 7;
                    out << (int) 3;
                    out << user; // Email des Offline Users
                    out << "\n";
                }

            }


        }
        in >> temp;

        qDebug() << "Es sind noch " << client->bytesAvailable() << "Bytes im Socket übrig";
        if(temp == "\n")
        {
            qDebug() << "Endzeichen gelesen";
        }

        qDebug() << "##############################################" << endl << endl;
        return;
    }
}

void ChatterBoxServer::kick()
{

}

void ChatterBoxServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    // if the user used quit to close client then set the user offline
    sql->setOnline(0,users.key(client));

    clients.remove(client);
	ipAddresses.removeAt(ipAddresses.indexOf(client->peerAddress()));
}

void ChatterBoxServer::sendUserList()
{

}
