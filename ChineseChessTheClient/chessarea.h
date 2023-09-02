#ifndef CHESSAREA_H
#define CHESSAREA_H

#include <QWidget>
#include <config.h>
#include<QPaintEvent>
#include<QPixmap>
#include<chess.h>
#include<QLabel>
#include<QDebug>
#include<QPoint>

namespace Ui {
class ChessArea;
}

class ChessArea : public QWidget
{
    Q_OBJECT

public:
    explicit ChessArea(QWidget *parent = 0);
    ~ChessArea();

    void initChessBroad();

    void getPicname();

    void initChesses();

    QPixmap m_image;

    int m_sidelenth;
    int m_posX;
    int m_posY;

    //点击时间所记录的id
    int choosenId1;
    int choosenId2;

    //画棋子所需
    QLabel  *label[32];     // 显示棋子图片标签
    Chess myChess[32];      // 初始化32个棋子
    QVector<QString> pic;   // 保存棋子图片名字

    //-----------  判断各种类型棋子能不能移动 ------------
      bool canMove(     int moveID, int killID, int x, int y);
      bool canMoveSHI(  int moveID, int killID, int x, int y);
      bool canMoveBING( int moveID, int killID, int x, int y);
      bool canMoveXIANG(int moveID, int killID, int x, int y);
      bool canMoveMA(   int moveID, int killID, int x, int y);
      bool canMoveCHE(  int moveID, int killID, int x, int y);
      bool canMovePAO(  int moveID, int killID, int x, int y);
      bool canMoveJIANG(int moveID, int killID, int x, int y);


      void getPicName();                   // 获取棋子图片名字
      bool existChess(int x, int y);       // 判断坐标上有没有棋子
      int  getChessID(int x, int y);       // 获取棋子的ID
      void showDeadChess(int  chessID);    // 显示阵亡棋子数目

      // 计算即将行走的棋子与某一坐标之间有几颗棋子
      int middleChessNum(int moveID, int x, int y);

      // 判断两个棋子是否同一方
      bool sameColor(int firstChessID, int secondChessID);

      // 棋子移动函数
      void moveChess(int moveID, int killID, int x, int y);

      //检查谁的将死了
      QString checkWinner();

private:
    Ui::ChessArea *ui;
};

#endif // CHESSAREA_H

