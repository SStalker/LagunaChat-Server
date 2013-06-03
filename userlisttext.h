#ifndef USERLISTTEXT_H
#define USERLISTTEXT_H

#include <QFile>

class UserListText
{
public:
    UserListText();
    bool checkData(QString username);
    bool checkData(QString username, QString pass_md5);
    //bool checkData(QString username, QString pass_md5);
    void appendData(QString username, QString pass_md5);
    //void editData(QString username, QString pass_md5);

    void closeFile();
private:
    QFile *file;
    QString filename;
};

#endif // USERLISTTEXT_H
