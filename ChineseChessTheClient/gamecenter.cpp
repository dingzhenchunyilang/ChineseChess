#include "gamecenter.h"
#include "ui_gamecenter.h"

GameCenter::GameCenter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameCenter)
{
    ui->setupUi(this);
}

void GameCenter::paintEvent(QPaintEvent *e){
    QPainter painter(this);

    painter.drawPixmap(0,0,600,500,QPixmap(GAMECENTER_PATH));
}

GameCenter::~GameCenter()
{
    delete ui;
}
