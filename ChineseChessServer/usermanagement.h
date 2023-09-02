#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include<QHash>
#include<QString>
#include<QFile>
#include<QTextStream>
#include<QTextCodec>
#include<user.h>
#include<QVector>


#define USERDATA_PATH "D://codeworks//qt//ChineseChessServer//data//data.txt"
class UserManagement
{
public:
    UserManagement();
    

    QVector<User*> users;
    
    void addUser(QString account,QString password,QString nickname);
    void loadData();
    void saveData();
    
    
};

#endif // USERMANAGEMENT_H
