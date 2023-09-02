#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectServer();

    initWindows();

    connectOperate();
}

//初始化所有窗口
void MainWindow::initWindows(){
    gamecenter = new GameCenter(this);
    reg = new RegisterDialog(this);
    maininterface = new MainInterface(this);
    loginwindow = new LoginWindow(this);


    gamecenter->hide();
    reg->hide();
    maininterface->hide();
    loginwindow->show();

    this->setFixedSize(600,600);
    this->setWindowTitle("象棋");

    setWindowIcon(QIcon(GAME_ICON));

    currentUser  = nullptr;

}

void MainWindow::connectServer(){
    socket = new QTcpSocket(this);
//    QNetworkProxyFactory::setUseSystemConfiguration(false);
    socket->connectToHost(SERVER_IP,SERVER_PORT);
    if(socket->waitForConnected(100000)){
        //连接失败
        connect(socket,&QTcpSocket::disconnected,[=](){
            QMessageBox::critical(this,"错误","与服务器断开连接！");
        });

        qDebug() << "已经连接上服务器。" << endl;
        connect(socket,&QTcpSocket::readyRead,this,&MainWindow::copeService);
    }
    else{
        QMessageBox::critical(this,"错误","网络异常，请检查网络后重启");
    }
}

//处理服务端发送的数据
//---------------------------------重要------------------------------------
void MainWindow::copeService(){
    QString str = socket->readAll();
    QStringList lines = str.split("*$E$*");

    for(int i = 0;i < lines.size();i++){
        if(lines[i] == "") continue;
        qDebug() << "收到" << lines[i];
        QStringList list = lines[i].split("_");
        switch(list[0].toInt()){
        case 0:{//登录失败信息  t:0
            QMessageBox::critical(this,"错误","密码或账号错误！");
            break;
        }
        case 1:{//登录成功，当前密码通过 t:0 account password nickname winnum
            currentUser = new User(list[1],list[2],list[3],list[4].toInt());
            qDebug() << list[1] << list[2] << list[3] << list[4];
            qDebug() << currentUser->m_account << " " << currentUser->m_password << " " << currentUser->m_nickname << " " << currentUser->m_winNum << endl;
            currentUser->socket = socket;
            QMessageBox::information(this,"成功","登陆成功，欢迎回来！");
            //跳转页面
            loginwindow->hide();
            this->setFixedSize(600,400);
            gamecenter->show();
            qDebug() << "切换到大厅" << endl;
            break;
        }
        case 2:{ // 处理服务器发来的房间信息 t:3_房间1的人数_房间2的人数_房间3的人数
            QString m1 = "人数:" + list[1] + "/2";
            gamecenter->ui->pushButton->setText(m1);
            QString m2 = "人数:" + list[2] + "/2";
            gamecenter->ui->pushButton_2->setText(m2);
            QString m3 = "人数:" + list[3] + "/2";
            gamecenter->ui->pushButton_3->setText(m3);

            update();
            break;
        }
        case 3:{ //处理对手信息 先手情况 游戏开始 t:3_nickname_winnum_isred
            //设置对手信息
            QString nickname = list[1];
            QString win_num = list[2];
            QString isRed = list[3];
            QString str = "姓名:" + nickname + "\n"
                    + "胜场:" + win_num;
            maininterface->opponentInfo->setText(str);

            currentUser->m_isGaming = true;
            qDebug() << isRed << endl;
            if(isRed == "yes"){
                QMessageBox::information(maininterface,"提示","游戏开始！您执红棋。");
                currentUser->m_isRed = true;
                currentUser->m_isMyTurn = true;

                maininterface->turnInfo->setText("你的回合");

                maininterface->myInfo->append("阵营：红方");
                maininterface->opponentInfo->append("阵营：黑方");
            }
            else{
                QMessageBox::information(maininterface,"提示","游戏开始！您执黑棋。");
                currentUser->m_isRed = false;
                currentUser->m_isMyTurn = false;

                maininterface->turnInfo->setText("对手回合");

                maininterface->myInfo->append("阵营：黑方");
                maininterface->opponentInfo->append("阵营：红方");
            }

            break;
        }
        case 4:{ //下棋权转换 t:4_x_y_x_y
            //将对方走的移植到我的棋盘上
            int x1 = list[1].toInt();
            int y1 = list[2].toInt();
            int x2 = list[3].toInt();
            int y2 = list[4].toInt();
            int id1 = maininterface->m_chessBroad.getChessID(x1,y1);
            int id2 = maininterface->m_chessBroad.getChessID(x2,y2);
            maininterface->m_chessBroad.moveChess(id1,id2,x2,y2);
            //该我下棋了
            currentUser->m_isMyTurn = true;
            //更新信息
            maininterface->turnInfo->setText("你的回合");

            break;
        }
        case 5:{
            QString str = list[1];
            maininterface->textroom->append(str);
            break;
        }
        case 6:{
            QString winner = list[1];
            if(winner == "red"){
                QMessageBox::information(maininterface,"提示","红方获胜！");
                //更新胜场
                if(currentUser->m_isRed) currentUser->m_winNum++;             
            }
            else{
                QMessageBox::information(maininterface,"提示","黑方获胜！");
                //更新胜场
                if(!currentUser->m_isRed) currentUser->m_winNum++;
            }

            //更新个人信息
            currentUser->m_isGaming = false;
            currentUser->m_isWatching = false;

            //退出房间
            maininterfaceToGamecenter();

            //打扫房间
            cleanGameroom();

            break;
        }
        case 7:{
            currentUser->m_isWatching = true;

            QMessageBox::information(maininterface,"提示","你正在观战！");
            break;
        }
        case 8:{
            if(list[1] == "yes") QMessageBox::information(this,"提示","注册成功，请返回登录！");
            else{
                QMessageBox::critical(this,"提示","账号已被使用，或有信息为空！");
            }
        }
        }
    }

}

void MainWindow::connectOperate(){

    //登录
    connect(loginwindow->ui->pushButton, &QPushButton::clicked, [=](){
        QString tar = "0_" + loginwindow->ui->lineEdit->text() + "_" + loginwindow->ui->lineEdit_2->text() + "_*$E$*";
        socket->write(tar.toUtf8());
    });

    //切换到注册窗口
    connect(loginwindow->ui->pushButton_2,&QPushButton::clicked,[=](){
        loginwindow->hide();
        reg->show();
    });

    //切换到登录窗口
    connect(reg->ui->pushButton_2,&QPushButton::clicked,[=](){
        reg->hide();
        loginwindow->show();
    });

    //注册
    connect(reg->ui->pushButton,&QPushButton::clicked,[=](){
       QString nickname = reg->ui->lineEdit->text();
       QString account = reg->ui->lineEdit_2->text();
       QString password = reg->ui->lineEdit_3->text();
       QString str = "1_" + account + "_" + password + "_" + nickname + "_*$E$*";

       socket->write(str.toUtf8());
    });

    //进入房间按钮
    //第一个房间
    connect(gamecenter->ui->pushButton_4,&QPushButton::clicked,[=](){
        currentUser->roomid = 1;
        //给服务器发送进入信息,服务
        QString info = "2_" + currentUser->m_account + "_1" + "_*$E$*";
        currentUser->socket->write(info.toUtf8());

        initGamingRoom();

        gamecenterToMaininterface();
    });

    //第二个房间
    connect(gamecenter->ui->pushButton_5,&QPushButton::clicked,[=](){
        currentUser->roomid = 2;
        //给服务器发送进入信息,服务
        QString info = "2_" + currentUser->m_account + "_2" + "_*$E$*";
        currentUser->socket->write(info.toUtf8());

        initGamingRoom();

        gamecenterToMaininterface();
    });

    //第三个房间
    connect(gamecenter->ui->pushButton_6,&QPushButton::clicked,[=](){
        currentUser->roomid = 3;
        //给服务器发送进入信息,服务
        QString info = "2_" + currentUser->m_account + "_3" + "_*$E$*";
        currentUser->socket->write(info.toUtf8());

        initGamingRoom();

        gamecenterToMaininterface();
    });

    //发送信息按钮
    connect(maininterface->btn,&QPushButton::clicked,[=](){
        //在我的窗口上加上我发的信息
        QString message = currentUser->m_nickname + ":\n" + maininterface->sendroom->toPlainText() + "\n";
        maininterface->textroom->append(message);
        //将我的信息传给服务器 t:4_message_account_roomid
        QString str = "4_" + message + "_" + currentUser->m_account + "_" + QString::number(currentUser->roomid) + "_*$E$*";
        currentUser->socket->write(str.toUtf8());
        //清空信息栏
        maininterface->sendroom->setText("");
    });

    //返回大厅
    connect(maininterface->backBtn,&QPushButton::clicked,[=](){
        if(currentUser->m_isGaming){
            QMessageBox::information(this,"提示","无法退出，正在游戏中！");
        }
        else{
            if(!currentUser->m_isWatching){
                //向服务器发送退出信息
                QString str = "6_" + currentUser->m_account + "_" + QString::number(currentUser->roomid) + "_*$E$*";
                currentUser->socket->write(str.toUtf8());
            }

            maininterfaceToGamecenter();
        }
    });

    connect(maininterface->defBtn,&QPushButton::clicked,[=](){
        if(currentUser->m_isGaming && currentUser->m_isRed){
            QString str = "5_black_" + QString::number(currentUser->roomid) +  "_*$E$*";
            socket->write(str.toUtf8());
        }
        else if(currentUser->m_isGaming && !currentUser->m_isRed){
            QString str = "5_red_" + QString::number(currentUser->roomid) +  "_*$E$*";
            socket->write(str.toUtf8());
        }
    });
}

//重写鼠标事件
void MainWindow::mousePressEvent(QMouseEvent *e){

    //正在游戏，且是我自己的回合，才判断,且不是观战状态
    if(currentUser != nullptr &&currentUser->m_isGaming && currentUser->m_isMyTurn && !currentUser->m_isWatching){
        //通过遍历每一个点周围一定的区域，判断是否有选择到某个区域
        static int click_num = 0;//第一次使用时，初始化为0
        int xPos = -1, yPos = -1;
        static int ox = -1 ,oy = -1;

        qDebug() << "(" << e->x() << "," << e->y() << ")" << endl;
        for(int i=0; i<10; i++)
        {
            int tx = e->x() - 682;
            if(i * CHESSBROAD_SIDE - 30 < tx && tx < i * CHESSBROAD_SIDE + 30)
            {
                xPos = i;
                break;
            }
        }

        for(int j=0; j<11; j++)
        {
            int ty = e->y() - 226;
            if(j * CHESSBROAD_SIDE - 30 < ty && ty < j * CHESSBROAD_SIDE + 30)
            {
                yPos = j;
                break;
            }
        }

        qDebug() << "(" << xPos << "," << yPos << ")" << endl;

        if(xPos != -1 && yPos != -1){
            if(click_num == 0){//第一次点击，需要选到自己方的棋子，否则无效点击
                maininterface->m_chessBroad.choosenId1 = maininterface->m_chessBroad.getChessID(xPos,yPos);

                ox = xPos,oy = yPos;
                qDebug() << "ox:" << ox << " oy:" << oy;
                if(maininterface->m_chessBroad.choosenId1 == -1 ||
                        maininterface->m_chessBroad.myChess[maininterface->m_chessBroad.choosenId1].m_isRed != currentUser->m_isRed)//判断是否是自家棋子,是否是空白
                {
                    qDebug() << "无效选择";
                    return;
                }
                else{
                    qDebug() << "第一次选择";
                    click_num++;
                }
            }
            else if(click_num == 1){//第二次点击
                qDebug() << "第二次选择";
                maininterface->m_chessBroad.choosenId2 = maininterface->m_chessBroad.getChessID(xPos,yPos);
                click_num++;
                qDebug() << "ox:" << ox << " oy:" << oy;
            }
        }
        else{
            qDebug() << "棋盘之外" << endl;
        }

        qDebug() << "ox:" << ox << " oy:" << oy;

        //判断是否能走棋
        if(click_num == 2){
            qDebug() << "ox:" << ox << " oy:" << oy;

            click_num = 0;

            if(maininterface->m_chessBroad.canMove(maininterface->m_chessBroad.choosenId1, maininterface->m_chessBroad.choosenId2,
                                                   xPos, yPos)){//如果可以走这一步棋
                maininterface->m_chessBroad.moveChess(maininterface->m_chessBroad.choosenId1, maininterface->m_chessBroad.choosenId2,
                                                      xPos, yPos);
                //检查游戏是否结束
                QString winner = maininterface->m_chessBroad.checkWinner();
                if(winner == "red"){
                    //发送自身输赢信息
                    QString str = "5_red_" + QString::number(currentUser->roomid) +  "_*$E$*";
                    currentUser->socket->write(str.toUtf8());
                }
                else if((winner == "black")){
                    QString str = "5_black_" + QString::number(currentUser->roomid) +  "_*$E$*";;
                    currentUser->socket->write(str.toUtf8());
                }
                else{
                    //交换下棋权

                    qDebug() << "ox:" << ox << " oy:" << oy;

                    QString str = "3_" + QString::number(ox) + "_"
                            + QString::number(oy) + "_"
                            + QString::number(xPos) + "_" +QString::number(yPos) + "_" + currentUser->m_account + "_" + QString::number(currentUser->roomid)
                            + "_*$E$*";

                    currentUser->socket->write(str.toUtf8());

                    //自身状态改变
                    currentUser->m_isMyTurn = false;

                    //信息改变
                    maininterface->turnInfo->setText("对手回合");
                }
            }

        }
        else{
            return;
        }
    }

    //返回其他不符合要求的点击事件
    return QMainWindow::mousePressEvent(e);
}


void MainWindow::gamecenterToMaininterface(){

    gamecenter->hide();
    this->setFixedSize(GAME_WIDTH,GAME_HEIGHT);
    maininterface->show();
}

void MainWindow::maininterfaceToGamecenter(){
    maininterface->hide();
    this->setFixedSize(600,400);
    gamecenter->show();
}

//初始化房间信息
void MainWindow::initGamingRoom(){
    QString str1 = "姓名:" + currentUser->m_nickname + "\n"
            + "胜场:" + QString::number(currentUser->m_winNum) ;
    QString str2 = "等待对手加入...";
    maininterface->myInfo->setText(str1);
    maininterface->opponentInfo->setText(str2);
}

void MainWindow::cleanGameroom(){
    //打扫信息栏
    maininterface->textroom->setText("");
    maininterface->sendroom->setText("");
    maininterface->opponentInfo->setText("");
    maininterface->myInfo->setText("");
    maininterface->turnInfo->setText("");

    //打扫棋盘
    maininterface = new MainInterface(this);

    //发送信息按钮
    connect(maininterface->btn,&QPushButton::clicked,[=](){
        //在我的窗口上加上我发的信息
        QString message = currentUser->m_nickname + ":\n" + maininterface->sendroom->toPlainText() + "\n";
        maininterface->textroom->append(message);
        //将我的信息传给服务器 t:4_message_account_roomid
        QString str = "4_" + message + "_" + currentUser->m_account + "_" + QString::number(currentUser->roomid) + "_*$E$*";
        currentUser->socket->write(str.toUtf8());
        //清空信息栏
        maininterface->sendroom->setText("");
    });

    //返回大厅
    connect(maininterface->backBtn,&QPushButton::clicked,[=](){
        if(currentUser->m_isGaming){
            QMessageBox::information(this,"提示","无法退出，正在游戏中！");
        }
        else{
            if(!currentUser->m_isWatching){
                //向服务器发送退出信息
                QString str = "6_" + currentUser->m_account + "_" + QString::number(currentUser->roomid) + "_*$E$*";
                currentUser->socket->write(str.toUtf8());
            }

            maininterfaceToGamecenter();
        }
    });

    connect(maininterface->defBtn,&QPushButton::clicked,[=](){
        if(currentUser->m_isGaming && currentUser->m_isRed){
            QString str = "5_black_" + QString::number(currentUser->roomid) +  "_*$E$*";
            socket->write(str.toUtf8());
        }
        else if(currentUser->m_isGaming && !currentUser->m_isRed){
            QString str = "5_red_" + QString::number(currentUser->roomid) +  "_*$E$*";
            socket->write(str.toUtf8());
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
