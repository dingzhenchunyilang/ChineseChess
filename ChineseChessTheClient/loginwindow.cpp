#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);


}

void LoginWindow::paintEvent(QPaintEvent *e){
    QPainter painter(this);

    painter.drawPixmap(0,0,600,600,QPixmap(LOGINWINDOW_PATH));
}

LoginWindow::~LoginWindow()
{

    delete ui;
}
