#include "user.h"

User::User()
{

}

User::User(QString account,QString password,QString nickname){
    m_account = account;
    m_password = password;
    m_nickname = nickname;
    m_win_num = 0;
    m_socket = nullptr;

    m_isOnline = false;
}

User::User(QString account,QString password,QString nickname,int win_num){
    m_account = account;
    m_password = password;
    m_nickname = nickname;
    m_win_num = win_num;
    m_socket = nullptr;

    m_isOnline = false;
}
