#ifndef USER_H
#define USER_H

#include<QString>
#include<QTcpSocket>

class User
{
public:
    User();
    User(QString account,QString password,QString nickname);
    User(QString account,QString password,QString nickname,int win_num);
    
    
    //属性
    QString m_account;
    QString m_nickname;
    QString m_password;
    int m_win_num;
    QTcpSocket * m_socket;
    bool m_isOnline;
};

#endif // USER_H
