#include "user.h"

User::User()
{

}

User::User(QString account,QString password,QString nickname,int winNum){
    m_account = account;
    m_password = password;
    m_nickname = nickname;
    m_winNum = winNum;

    roomid = -1;
    m_isWatching = false;
    m_isGaming = false;
    m_hasBack = false;
}
