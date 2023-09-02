#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QVector>
#include<config.h>
#include<user.h>
#include<QFile>
#include<QTextStream>
#include<QMessageBox>
#include<QTextCodec>
#include<QDebug>
#include<QString>
#include<registerdialog.h>
#include<QTcpSocket>
#include<gamecenter.h>
#include<maininterface.h>
#include<loginwindow.h>
#include<QNetworkProxyFactory>


#include"ui_loginwindow.h"
#include"ui_gamecenter.h"
#include"ui_maininterface.h"
#include"ui_registerdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //连接一些功能
    void connectOperate();

    //连接到客户端
    void connectServer();

    //初始化所有窗口
    void initWindows();

    //大厅转到游戏界面
    void gamecenterToMaininterface();

    //游戏界面转到大厅
    void maininterfaceToGamecenter();
    
    //初始化游戏房间
    void initGamingRoom();

    //打扫游戏房间
    void cleanGameroom();

private:
    Ui::MainWindow *ui;

    //当前用户
    User * currentUser;

    QTcpSocket * socket;

    //窗口 全部将loginwindow声明为友元
    LoginWindow * loginwindow;//登录窗口

    RegisterDialog * reg;  //注册窗口

    GameCenter * gamecenter;//大厅

    MainInterface * maininterface;//游戏界面

    //鼠标点击事件
    void mousePressEvent(QMouseEvent * e);

public slots:
    void copeService();
};

#endif // MAINWINDOW_H
