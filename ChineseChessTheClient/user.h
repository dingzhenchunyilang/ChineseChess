#ifndef USER_H
#define USER_H
#include<QString>
#include<QTcpSocket>
class User
{
public:
    User();

    User(QString account,QString password,QString nickname,int winNum);

    //基础信息
    QString m_nickname;
    QString m_account;
    QString m_password;
    int m_winNum;
    QTcpSocket * socket;
    int roomid;

    //对局时信息

    bool m_isMyTurn;//是否轮到当前用户下棋
    bool m_isRed;//是否是红方
    bool m_isGaming;//是否是对局进行中的状态
    bool m_isWatching;
    bool m_hasBack;//是否悔过棋了

};

#endif // USER_H
