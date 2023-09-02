#ifndef CHESS_H
#define CHESS_H
#include<config.h>
#include <QWidget>

namespace Ui {
class Chess;
}

class Chess : public QWidget
{
    Q_OBJECT

public:
    explicit Chess(QWidget *parent = 0);
    ~Chess();

    // 枚举的 车=0 马=1 炮=2 兵=3 将=4 士=5 相=6
    enum   TYPE{ CHE, MA, PAO, BING, JIANG, SHI, XIANG };


//方法
    void initChess(const int id);

    QString getChessType();

//属性
    //坐标
    int m_row;//横坐标
    int m_col;//纵坐标
    //棋子id
    int m_id;
    bool m_isDead;
    bool m_isRed;
    TYPE m_type;


private:
    Ui::Chess *ui;
};

#endif // CHESS_H
