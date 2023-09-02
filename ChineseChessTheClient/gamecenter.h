#ifndef GAMECENTER_H
#define GAMECENTER_H

#include <QWidget>
#include<config.h>
#include<QPainter>

namespace Ui {
class GameCenter;
}

class GameCenter : public QWidget
{
    Q_OBJECT

    friend class MainWindow;

public:
    explicit GameCenter(QWidget *parent = 0);
    ~GameCenter();

    void paintEvent(QPaintEvent * e);

private:
    Ui::GameCenter *ui;
};

#endif // GAMECENTER_H

