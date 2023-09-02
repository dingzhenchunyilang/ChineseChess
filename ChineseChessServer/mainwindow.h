#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<usermanagement.h>
#include<server.h>
#include<QTimer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Server server;


private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
