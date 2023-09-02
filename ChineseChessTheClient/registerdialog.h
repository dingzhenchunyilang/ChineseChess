#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QWidget>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QWidget
{
    Q_OBJECT


    friend class MainWindow;

public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();


private:
    Ui::RegisterDialog *ui;



signals:




};

#endif // REGISTERDIALOG_H

