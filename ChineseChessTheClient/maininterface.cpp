#include "maininterface.h"
#include "ui_maininterface.h"

MainInterface::MainInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainInterface)
{
    ui->setupUi(this);

    initScence();

    initChessBroad();

    initChatroom();
}

void MainInterface::initScence(){

    setFixedSize(GAME_WIDTH,GAME_HEIGHT);

    timer.setInterval(GAME_RATE);

    timer.start();

    connect(&timer,&QTimer::timeout,[=](){

        updateScence();
        //系统内置方法 重新绘制图片
        update();

    });

}

void MainInterface::initChatroom()
{
    QFont font("Arial", 18); // Arial字体，字号为12
    //聊天记录框
    textroom = new QTextEdit(this);
    textroom->setReadOnly(true);//设置只读
    textroom->resize(550,620);
    textroom->move(50,80);
    textroom->setStyleSheet("background-color: rgba(255, 255, 255, 0.5);");

    //文本发送框
    sendroom = new QTextEdit(this);
    sendroom->resize(550,250);
    sendroom->move(50,720);
    sendroom->setFontPointSize(12);
    sendroom->setStyleSheet("background-color: rgba(255, 255, 255, 0.5);");

    //发送按钮
    btn = new QPushButton("发送",this);
    btn->resize(170,40);
    btn->move(400,910);

    //对手信息窗口
    opponentInfo = new QTextEdit(this);
    opponentInfo->move(1400,100);
    opponentInfo->resize(500,300);
    opponentInfo->setReadOnly(true);
    opponentInfo->setFont(font);
    
    //己方信息窗口
    myInfo = new QTextEdit(this);
    myInfo->move(1400,600);
    myInfo->resize(500,300);
    myInfo->setReadOnly(true);
    myInfo->setFont(font);
    
    //当前回合
    turnInfo = new QTextEdit(this);
    turnInfo->resize(200,100);
    turnInfo->move(1550,450);
    turnInfo->setReadOnly(true);
    turnInfo->setFont(font);

    //返回按钮
    backBtn = new QPushButton(this);
    backBtn->resize(240,70);
    backBtn->setText("返回大厅");
    backBtn->move(730,70);

    //认输按钮
    defBtn = new QPushButton(this);
    defBtn->resize(240,70);
    defBtn->setText("认输");
    defBtn->move(1030,70);



}

//初始化棋盘
void MainInterface::initChessBroad(){

    m_chessBroad.m_posX = CHESSBROAD_POSX;
    m_chessBroad.m_posY = CHESSBROAD_POSY;
    m_chessBroad.m_image.load(CHESSBROAD_PATH);

    m_chessBroad.initChesses();

    //显示棋盘widget
    m_chessBroad.setParent(this);
    m_chessBroad.move(635,180);
    m_chessBroad.resize(800,900);
    m_chessBroad.show();

}

void MainInterface::updateScence(){


    update();
}

//画画事件
void MainInterface::paintEvent(QPaintEvent * paintEvent){
    QPainter painter(this);

    painter.drawPixmap(0,0,QPixmap(BACKGROUD_PATH));
    painter.drawPixmap(m_chessBroad.m_posX,m_chessBroad.m_posY,m_chessBroad.m_image);
}

MainInterface::~MainInterface()
{
    delete ui;
}

