#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QWidget>
#include<chessarea.h>
#include<QPainter>
#include<config.h>
#include<QIcon>
#include<QPaintEvent>
#include<QTimer>
#include<user.h>
#include<QMessageBox>
#include<QTextEdit>
#include<QPushButton>
#include<QFont>

namespace Ui {
class MainInterface;
}

class MainInterface : public QWidget
{
    Q_OBJECT

    friend class MainWindow;

public:
    explicit MainInterface(QWidget *parent = 0);
    ~MainInterface();

    void initScence();

    void initChessBroad();

    void paintEvent(QPaintEvent * paintEvent);

    void updateScence();

    void initChatroom();


    //聊天窗口
    QPushButton * btn;
    QTextEdit * textroom;
    QTextEdit * sendroom;

    //信息窗口
    QTextEdit * opponentInfo;
    QTextEdit * myInfo;
    QTextEdit * turnInfo;

    QTimer  timer;

    //返回按钮
    QPushButton * backBtn;
    //认输按钮
    QPushButton * defBtn;

private:
    Ui::MainInterface *ui;


protected:
    ChessArea m_chessBroad;

};

#endif // MAININTERFACE_H
