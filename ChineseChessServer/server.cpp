#include "server.h"
#include<QNetworkProxyFactory>
Server::Server(QWidget *parent) : QWidget(parent)
{

}


void Server::init()
{
    //初始化服务器的连接
    // 设置服务端端口
    port = 6677;

    // 创建服务器对象并监听指定地址和端口
    server = new QTcpServer(this);

    if (!server->listen(QHostAddress::Any, port))
    {
        qDebug() << "服务器启动失败";
        return;
    }

    // 当有新连接时触发该槽函数
    connect(server, &QTcpServer::newConnection, this, &Server::handleNewConnection);

    qDebug() << "服务器于" << port << "端口启动成功";
    qDebug() << server->serverAddress().toString();

    //

}

void Server::handleNewConnection()
{
    qDebug() << "有人尝试连接！" << endl;
    while (server->hasPendingConnections())
    {
        //创建新的socket通道
        QTcpSocket *socket = server->nextPendingConnection();
        
        //记录ip并输出连接成功的信息
        QString user_ip = socket->peerAddress().toString();
        qDebug() << user_ip << " connected";

        // 当有数据可读时触发该槽函数
        connect(socket, &QTcpSocket::readyRead, this, [=]() {
            //具体操作，处理数据

            service(socket);
        });
    }
}

//处理接收的数据
void Server::service(QTcpSocket * socket){
    QString str = socket->readAll();
    QStringList lines = str.split("*$E$*");
    for(int i = 0;i < lines.size();i++ ){
        if(lines[i] == "") continue;
        qDebug() << "接收到" << lines[i] << endl;

        QStringList list = lines[i].split("_");

        int index = list[0].toInt();

        switch (index) {
        case 0://登录功能  //发送的数据格式：0_account_password
        {
            bool find = false;
            QString account = list[1];
            QString password = list[2];
            //检查账号密码
            for(auto user : usermanagement.users){
                if(user->m_account == account){
                    if(user->m_password == password){
                        //正确，允许登录，将用户设为在线？返回房间信息去显示
                        find = true;
                        user->m_isOnline = true;
                        //向用户更新房间信息
                        QString info = "2_" + QString::number(room[0].m_users.size()) + "_" +
                                QString::number(room[1].m_users.size()) + "_" +
                                QString::number(room[2].m_users.size()) + "_*$E$*";

                        // 第二条消息发送
                        socket->write(info.toUtf8());

                        //返回登录成功信息
                        QString str = "1_" + user->m_account + "_" +user->m_password + "_" +user->m_nickname +
                                "_" + QString::number(user->m_win_num) + "_*$E$*";
                        // 第二条消息发送
                        socket->write(str.toUtf8());


                        //重要：绑定用户与该socket通道
                        user->m_socket = socket;
                        if(user->m_socket == nullptr) qDebug() << "这里这里这里会理会了婚礼" << endl;
                        break;

                    }
                }
            }
            //检查不通过，不予登录
            if(!find){
                socket->write("0_*$E$*");
            }
            break;
        }
        case 1://注册功能 1_account_password_nickname
        {
            //检查账号信息能否通过
            for(auto user : usermanagement.users){
                if(list[1] == user->m_account || list[1] == "" || list[2] == "" || list[3] == ""){//有重复
                    QString str = "8_no_*$E$*";
                    socket->write(str.toUtf8());
                    break;
                }
            }

            //写入文件，更新当前users,返回成功信息
            usermanagement.addUser(list[1],list[2],list[3]);

            usermanagement.saveData();

            QString str = "8_yes_*$E$*";
            socket->write(str.toUtf8());

            break;
        }
        case 2://加入房间功能 （观战，做局） t:2_account_房间号
        {
            QString account = list[1];
            int roomid = list[2].toInt() - 1;
            //遍历找到当前用户：
            for(auto user : usermanagement.users){
                if(account == user->m_account){//找到了目标用户
                    //判断是去加入对局还是观战
                    if(room[roomid].isGaming)//观战功能
                    {
                        //将用户加入到房间之中
                        room[roomid].m_users.push_back(user);
                        //发送此人为观战者的信息
                        QString jud = "7_*$E$*";
                        user->m_socket->write(jud.toUtf8());

                        //发送历史走棋
                        for(auto info : room[roomid].moveRecords){
                            QString str = "4_" + info + "_*$E$*";
                            user->m_socket->write(str.toUtf8());
                        }

                        //发送给观战者初始棋盘信息
                    }
                    else{
                        //将用户加入到房间之中，如果房间人满，就开始游戏
                        room[roomid].m_users.push_back(user);
                        if(room[roomid].m_users.size() == 2){
                            room[roomid].isGaming = true;

                            //开始游戏
                            //给两个人互相发送对手信息  t:3_nickname_winnum_isred
                            //决定谁是红方
                            // 设置随机种子
                            srand(time(NULL));

                            // 生成一个随机整数（0或1）0就vector中第一个人为红方
                            int randomNum = rand() % 2;

                            if(randomNum == 0){//分配为红方
                                QString str1 = "3_" + room[roomid].m_users[1]->m_nickname + "_" +
                                        QString::number(room[roomid].m_users[1]->m_win_num)
                                        + "_yes"+ "_*$E$*";
                                if(room[roomid].m_users[0]->m_socket != nullptr){
                                    room[roomid].m_users[0]->m_socket->write(str1.toUtf8());
                                }
                                else{
                                    qDebug() << "游戏开始中出现空指针！" << endl;
                                }

                                QString str2 = "3_" + room[roomid].m_users[0]->m_nickname + "_" +
                                        QString::number(room[roomid].m_users[0]->m_win_num)
                                        + "_no" + "_*$E$*";
                                if(room[roomid].m_users[1]->m_socket != nullptr){
                                    room[roomid].m_users[1]->m_socket->write(str2.toUtf8());
                                }
                                else{
                                    qDebug() << "游戏开始中出现空指针！" << endl;
                                }
                            }
                            else{
                                QString str1 = "3_" + room[roomid].m_users[0]->m_nickname + "_" +
                                        QString::number(room[roomid].m_users[0]->m_win_num)
                                        + "_yes" + "_*$E$*";
                                if(room[roomid].m_users[1]->m_socket != nullptr){
                                    room[roomid].m_users[1]->m_socket->write(str1.toUtf8());
                                }
                                else{
                                    qDebug() << "游戏开始中出现空指针！" << endl;
                                }

                                QString str2 = "3_" + room[roomid].m_users[1]->m_nickname + "_" +
                                        QString::number(room[roomid].m_users[1]->m_win_num)
                                        + "_no" + "_*$E$*";
                                if(room[roomid].m_users[0]->m_socket != nullptr){
                                    room[roomid].m_users[0]->m_socket->write(str2.toUtf8());
                                }
                                else{
                                    qDebug() << "游戏开始中出现空指针！" << endl;
                                }
                            }
                        }

                        broadcastRoomInfo();
                    }
                }
            }

            break;
        }
        case 3://将棋子变动传给其他用户，同时转换下棋权 t: 3_对方走的棋的x坐标_对方走的棋y_target x_target y_account_roomid
        {
            int roomid = list[6].toInt() - 1;
            QString account = list[5];
            QString str = "4_" + list[1] + "_" + list[2] + "_" + list[3] + "_" + list[4] + "_*$E$*";

            //储存这一步棋 x_y_x_y
            QString info = list[1] + "_" + list[2] + "_" + list[3] + "_" + list[4];
            room[roomid].moveRecords.push_back(info);

            //对房间里所有人更新信息
            for(auto user : room[roomid].m_users){
                if(user->m_account != account){
                    user->m_socket->write(str.toUtf8());
                }
            }
            break;
        }
        case 4: //4_message_account
        {
            int roomid = list[3].toInt() - 1;
            QString message = list[1];
            QString account = list[2];
            //找到用户所在房间，将聊天内容发给其他用户
            QString str = "5_" + message + "_*$E$*";
            for(auto user : room[roomid].m_users){
                if(user->m_account != account){
                    user->m_socket->write(str.toUtf8());
                }
            }
            break;
        }
        case 5://向所有房间里的人宣布获胜方
        {
            QString winner = list[1];
            int roomid = list[2].toInt() - 1;
            QString str = "6_" + winner + "_*$E$*";
            for(auto user : room[roomid].m_users){
                user->m_socket->write(str.toUtf8());
            }

            //清空房间
            room[roomid].m_users.clear();

            room[roomid].isGaming = false;

            room[roomid].moveRecords.clear();

            broadcastRoomInfo();

            break;
        }
        case 6:
        {
            int roomid = list[2].toInt() -  1;
            room[roomid].removeUser(list[1]);

            broadcastRoomInfo();
            break;
        }

    }
}
}

//向其他用户更新房间信息
void Server::broadcastRoomInfo(){
    QString info = "2_" + QString::number(room[0].m_users.size()) + "_" +
            QString::number(room[1].m_users.size()) + "_" +
            QString::number(room[2].m_users.size()) + "_*$E$*";

    for(auto user : usermanagement.users){
        if(user->m_isOnline){//用户在线，才给他更新信息
            if (user->m_socket != nullptr) {
                user->m_socket->write(info.toUtf8());
            }
            else{
                qDebug() << "指针为空" << endl;
            }
        }
    }
}










