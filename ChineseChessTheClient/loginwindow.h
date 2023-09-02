#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
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

#include"ui_gamecenter.h"
#include"ui_maininterface.h"
#include"ui_registerdialog.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

    friend class MainWindow;
public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();


private:
    Ui::LoginWindow *ui;

    //当前用户
    User currentUser;

    QTcpSocket * socket;

    //窗口 全部将loginwindow声明为友元
    RegisterDialog * reg;  //注册窗口

    GameCenter * gamecenter;//大厅

    MainInterface * maininterface;//游戏界面

    void paintEvent(QPaintEvent * e);



signals:



public slots:


};

#endif // LOGINWINDOW_H
