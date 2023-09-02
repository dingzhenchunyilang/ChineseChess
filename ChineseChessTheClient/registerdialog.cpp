#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    //注册功能
    connect(ui->pushButton,&QPushButton::clicked,[=](){

    });


}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}
