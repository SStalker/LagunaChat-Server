#include "userlisttext.h"
#include <QFile>
#include <QDateTime>
#include <QDebug>

UserListText::UserListText()
{
    filename = "userlist.txt";
    file = new QFile(filename);

    if(file->exists())
    {
        file->open(QIODevice::ReadWrite);
    }
    else
    {
        qDebug() << "Datei existiert nicht \n Öffne trotzdem";
        file->open(QIODevice::ReadWrite);
    }
}

void UserListText::appendData(QString username, QString pass_md5)
{
    qDebug() << "Neue Daten werden in Datei geschrieben";
    QByteArray array;

    array.append(QDateTime::currentDateTime().toString("dd.MM.yyyy|hh:mm:ss"));
    array.append(",");
    array.append(username);
    array.append(",");
    array.append(pass_md5);
    array.append("\n");
    file->seek(file->size());
    file->write(array);
}

bool UserListText::checkData(QString username)
{
    while(!file->atEnd())
    {
        QByteArray line = file->readLine();

        if(line.contains(username.toUtf8()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

bool UserListText::checkData(QString username,QString pass_md5)
{
    while(!file->atEnd())
    {
        QByteArray line = file->readLine();

        if(line.contains(username.toUtf8()) && line.contains(pass_md5.toUtf8()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

void UserListText::closeFile()
{
    file->flush();
    file->close();
}
