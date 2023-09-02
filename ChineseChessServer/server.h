#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QWidget>
#include<QTcpServer>
#include<QTcpSocket>
#include<QDebug>
#include<usermanagement.h>
#include<room.h>
#include<QTimer>

class Server : public QWidget
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);

    //方法
    void init();

    void service(QTcpSocket * socket);

    void broadcastRoomInfo();

    //属性
    QTcpServer * server;

    int port;

    UserManagement usermanagement;

    Room room[3];

signals:



public slots:
    void handleNewConnection();
};

#endif // SERVER_H
