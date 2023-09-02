#include "room.h"

Room::Room()
{
    //初始化房间信息
    isGaming = false;
}

void Room::checkSize()
{
    if(m_users.size() == 2){

        isGaming = true;

    }
}

void Room::removeUser(const QString& account)
{
    for(auto it = m_users.begin(); it != m_users.end(); ++it)
    {
        if((*it)->m_account == account)
        {
            m_users.erase(it);
            break;
        }
    }
}
