#include "usermanagement.h"


UserManagement::UserManagement()
{

}

void UserManagement::loadData(){
    QFile file(USERDATA_PATH);
    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        QTextCodec * code = QTextCodec::codecForName("UTF-8");
        in.setCodec(code);
        while(!in.atEnd()){
            QString str = in.readLine();

            QStringList list = str.split("_");
            User* user = new User(list[0],list[1],list[2],list[3].toInt());
            qDebug() << user->m_account << " " <<user->m_password <<  " " <<user->m_nickname <<
                        " " << user->m_win_num << endl;
            users.push_back(user);
        }

        qDebug() << "读入" << users.size() << "名用户的数据。" << endl;
    }
    else{
        qDebug() << "文件不存在！" << endl;
    }
    file.close();
}

void UserManagement::saveData(){

    
    //写入文件
    QFile file(USERDATA_PATH);
    QTextCodec * code = QTextCodec::codecForName("UTF-8");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream in(&file);
        in.setCodec(code);
        for(auto user : users){
            in << user->m_account << "_" << user->m_password << "_" << user->m_nickname << "_" << user->m_win_num << endl;
        }
        qDebug() << "成功保存用户数据！" << endl;
    }
    else{
        qDebug() << "文件不存在" << endl;
    }
           
    file.close();
}

void UserManagement::addUser(QString account,QString password,QString nickname){

        //写入文件中，同时更新users
        User * user = new User(account,password,nickname);
        users.push_back(user);
        QFile file(USERDATA_PATH);
        QTextCodec * code = QTextCodec::codecForName("UTF-8");
        if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
            QTextStream in(&file);
            in.setCodec(code);
            in << account << "_" << password << "_" << nickname << "_" << "0" << endl;
            qDebug() << "成功保存" << nickname << "的信息！" << endl;
        }
        else{
            qDebug() << "文件不存在" << endl;
        }

        file.close();
}




