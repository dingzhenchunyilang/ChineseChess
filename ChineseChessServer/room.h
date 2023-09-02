#ifndef ROOM_H
#define ROOM_H
#include<user.h>
#include<QVector>

class Room
{
public:
    Room();

    QVector<User*> m_users;

    bool isGaming;

    //检查房间人数，若有两个人，就开始游戏
    void checkSize();

    void removeUser(const QString& account);

    QVector<QString> moveRecords;

};

#endif // ROOM_H
