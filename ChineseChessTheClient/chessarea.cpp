#include "chessarea.h"
#include "ui_chessarea.h"

ChessArea::ChessArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessArea)
{
    ui->setupUi(this);
}

void ChessArea::initChessBroad(){
    m_image.load(CHESSBROAD_PATH);

    m_sidelenth = CHESSBROAD_SIDE;

    choosenId1 = 100;
    choosenId2 = 100;
}

void ChessArea::getPicname(){
    pic.append(CHESS_REDCHE);
    pic.append(CHESS_REDMA);
    pic.append(CHESS_REDXIANG);
    pic.append(CHESS_REDSHI);
    pic.append(CHESS_REDJIANG);
    pic.append(CHESS_REDPAO);
    pic.append(CHESS_REDBING);

    pic.append(CHESS_BLACKCHE);
    pic.append(CHESS_BLACKMA);
    pic.append(CHESS_BLACKXIANG);
    pic.append(CHESS_BLACKSHI);
    pic.append(CHESS_BLACKJIANG);
    pic.append(CHESS_BLACKPAO);
    pic.append(CHESS_BLACKBING);



}

void ChessArea::initChesses(){
    QString name;
    getPicname();

    for(int i=0; i<32; i++)
        {
            // 根据象棋棋子摆放位置摆放棋子
            if (i <= 4) {
                myChess[i].initChess(i);
                name = pic.at(i);
            } else if(i>4 && i<9) {
                myChess[i].initChess(i);
                name = pic.at(8-i);
            } else if(i==9 || i==10) {
                myChess[i].initChess(i);
                name = pic.at(5);
            } else if(i >= 11 && i<=15) {
                myChess[i].initChess(i);
                name = pic.at(6);
            } else if(i > 15 && i <= 20) {
                myChess[i].initChess(i);
                name = pic.at(i-9);
            } else if(i > 20 && i < 25) {
                myChess[i].initChess(i);
                name = pic.at(31-i);
            } else if(i == 25 || i == 26){
                myChess[i].initChess(i);
                name = pic.at(12);
            } else if(i>=27) {
                 myChess[i].initChess(i);
                 name = pic.at(13);
            }

            //绘制图片到label上面
            label[i] = new QLabel(this);

            label[i]->resize(CHESSBROAD_SIDE + 10,CHESSBROAD_SIDE + 10);
            label[i]->setPixmap(name);
            label[i]->setFixedSize(CHESSBROAD_SIDE + 10,CHESSBROAD_SIDE + 10);
            label[i]->setScaledContents(true);//自动缩放

            //根据坐标绘制
            label[i]->setGeometry(myChess[i].m_row * CHESSBROAD_SIDE,
                                         myChess[i].m_col * CHESSBROAD_SIDE,
                                          CHESSBROAD_SIDE, CHESSBROAD_SIDE);

            update();
       }

}

//棋子移动判断的方法

//判断目标落点是否存在棋子
bool ChessArea::existChess(int x, int y){
    for(int i =0;i < 32;i++){
        if(myChess[i].m_row == x && myChess[i].m_col == y){
            return true;
        }
    }
    return false;
}

//计算目标子与落点之间存在多少棋子（前提是在同一直线上，否则没有意义）
int ChessArea::middleChessNum(int moveID, int x, int y){
    int sum = 0;
    if(myChess[moveID].m_row == x)//在同一列
    {
        if(myChess[moveID].m_col < y)//确定计算的方向
        {
            for(int i = myChess[moveID].m_col + 1;i < y;i++)
            {
                if(existChess(x,i)) sum++;
            }
        }
        else{
            for(int i = myChess[moveID].m_col -1 ;i > y;i--)
            {
                if(existChess(x,i)) sum++;
            }
        }

        return sum;
    }
    else if(myChess[moveID].m_col == y) //在同一行
    {
        if(myChess[moveID].m_row < x){
            for(int i = myChess[moveID].m_row + 1;i < x;i++){
                if(existChess(i,y)) sum++;
            }
        }
        else{
            for(int i = myChess[moveID].m_row - 1;i > x;i--){
                if(existChess(i,y)) sum++;
            }
        }
            return sum;
        }
    //不在同一直线上，没有意义，返回-1
     return -1;
}

//killID，如果为-1，说明是空闲的位置
bool ChessArea::canMoveCHE(int moveID,int killID,int x,int y){
    if(killID > 31)
        return false;
    if(middleChessNum(moveID,x,y) == 0)//车要吃，去的路上不能有棋子
    {
        return true;
    }

    return false;
}

bool ChessArea::canMovePAO(int moveID, int killID, int x, int y){
    if(killID > 31)
        return false;
    if(middleChessNum(moveID,x,y) == 1 && killID != -1)//炮要吃的路上要有一个棋子
    {
        return true;
    }
    if(middleChessNum(moveID,x,y) == 0 && killID == -1){//去的路上不能有棋子
        return true;
    }

    return false;
}

bool ChessArea::canMoveSHI(int moveID, int killID, int x, int y){
    //只能斜着走,且不能出营
    if(killID > 31) return false;

    //不能出营
    if(myChess[moveID].m_isRed){
            if(y<7 || x<3 || x>5) return false;
        }
        else {
            if(y>2 || x<3 || x>5) return false;
        }

    //斜着走
    if(abs(x - myChess[moveID].m_row) == 1 && abs(y - myChess[moveID].m_col) == 1){
        return true;
    }

    return false;
}

bool ChessArea::canMoveXIANG(int moveID, int killID, int x, int y){
    //走田字格，不能过河，田字中间不能有阻隔
    if(killID > 31) return false;
    //象不能过河
    if(myChess[moveID].m_isRed){
        if(y<5) return false;
    } else{
        if(y>4) return false;
    }

    if(abs(myChess[moveID].m_row - x) == 2 && abs(myChess[moveID].m_col - y) == 2){
        //判断是否别脚
        if(!existChess(((myChess[moveID].m_row + x) / 2),((myChess[moveID].m_col + y) / 2)))
        {
            return true;
        }
    }

    return false;

}

bool ChessArea::canMoveMA(int moveID, int killID, int x, int y){
    //马走日，不能撇腿
    if(killID > 31) return false;

    //走日 2/1  或者1/2
    if(abs(myChess[moveID].m_row - x) == 1 && abs(myChess[moveID].m_col - y) == 2){  //1/2
        //判断撇腿
        if(!existChess(myChess[moveID].m_row,((myChess[moveID].m_col + y) / 2))){
            return true;
        }
    }
    else if(abs(myChess[moveID].m_row - x) == 2 && abs(myChess[moveID].m_col - y) == 1){
        if(!existChess(((myChess[moveID].m_row + x) / 2),myChess[moveID].m_col)){
            return true;
        }
    }

    return false;
}

bool ChessArea::canMoveJIANG(int moveID, int killID, int x, int y){
    //不能出营，不能与敌将碰面,走一步
    if(killID > 31) return false;

    if(myChess[moveID].m_isRed){
        if(killID == 20 && myChess[moveID].m_row == myChess[20].m_row)//吃的目标是对面的将
        {
            if(middleChessNum(moveID,x,y) == 0)//中间没有阻隔
            {
                return true;
            }
        }

        //目标不是对面的将，不能出营
        if(x<3 || x>5 || y > 9 || y < 7) return false;
        //检查步长
        if((abs(myChess[moveID].m_row - x)==1 && abs(myChess[moveID].m_col - y)==0) ||
            (abs(myChess[moveID].m_row - x==0 && abs(myChess[moveID].m_col - y)==1)))
        {
            return true;
        }
    }
    else{
        if(killID == 4 && myChess[moveID].m_row == myChess[20].m_row)//吃的目标是对面的将
        {
            if(middleChessNum(moveID,x,y) == 0)//中间没有阻隔
            {
                return true;
            }
        }

        //目标不是对面的将，不能出营
        if(x<3 || x>5 || y < 0 || y > 2) return false;
        //检查步长
        if((abs(myChess[moveID].m_row - x)==1 && abs(myChess[moveID].m_col - y)==0) ||
            (abs(myChess[moveID].m_row - x==0 && abs(myChess[moveID].m_col - y)==1)))
        {
            return true;
        }
    }

    return false;
}

bool ChessArea::canMoveBING(int moveID, int killID, int x, int y){
    //没过河，只能往前走，过了和，可以左右走，每次走一步
    if(killID > 31)
          return false;

      int dx=myChess[moveID].m_row - x;
      int dy=myChess[moveID].m_col - y;

      if(myChess[moveID].m_isRed){
          // 红方棋子，过河前的行走规则
          if(myChess[moveID].m_col>=5 && myChess[moveID].m_col<=6) {

              if(dy==1 && dx==0)  //竖着走,不回头
                  return true;
              else                //横着走,走不通
                  return false;
       } else { /* 过河后 */
          if((abs(dy)==1 && abs(dx)==0) ||(abs(dx)==1 && abs(dy)==0)){
              if(dy == -1)         //竖着走
                  return false;    //竖着走走了回头路就要返回错误
              else                 //横着走
                  return true;
          }else {
              return false;
          }
       }

      } else { /* 黑棋 */
          // 黑方棋子，过河前的行走规则
          if(myChess[moveID].m_col>= 3 && myChess[moveID].m_col<=4){
              if(dy == -1 && dx==0)   //竖着走,不回头
                  return true;
              else                    //横着走,走不通
                  return false;
          } else { /* 过河后 */
              if((abs(dx)==1&&abs(dy)==0)||(abs(dy)==1&&abs(dx)==0)){
                  if(dy == 1)         //竖着回头走,走不通
                      return false;
                  else                //横着走,一样走得通
                      return true;
              } else {
                  return false;
              }
          }
      }

      return false;

}

bool ChessArea::sameColor(int firstChessID, int secondChessID){
    return myChess[firstChessID].m_isRed == myChess[secondChessID].m_isRed;
}

bool ChessArea::canMove(int moveID, int killID, int x, int y)
{
    if(moveID < 0)
        return false;

    // killID 为 -1 表示该ID是没有棋子
    if(killID==-1 || !sameColor(moveID, killID))//是空白，或者选择与要吃的颜色不一样
    {
        switch(myChess[moveID].m_type)
        {
            case Chess::JIANG:
                return canMoveJIANG(moveID, killID, x, y);
                break;
            case Chess::SHI:
                return canMoveSHI(moveID, killID, x, y );
                break;
            case Chess::XIANG:
                return canMoveXIANG(moveID, killID, x, y);
                break;
            case Chess::CHE:
                return canMoveCHE(moveID, killID, x, y);
                break;
            case Chess::MA:
                return canMoveMA(moveID, killID, x, y);
                break;
            case Chess::PAO:
                return canMovePAO(moveID, killID, x, y);
                break;
            case Chess::BING:
                return canMoveBING(moveID, killID, x, y);
                break;
            default: break;
        }
    }

    return false;
}

int ChessArea::getChessID(int x, int y){
    for(int i = 0;i < 31;i++){
        if(myChess[i].m_row == x && myChess[i].m_col == y && !myChess[i].m_isDead){
            return i;
        }
    }

    //该点没有棋子，返回-1
    return -1;
}

void ChessArea::moveChess(int moveID, int killID, int x, int y){
    if(killID != -1)//该次移动为 吃
    {
        myChess[killID].m_isDead = true;
        myChess[killID].m_row = -1;
        myChess[killID].m_col = -1;
        label[killID]->hide();

    }

    //移动
    myChess[moveID].m_row = x;
    myChess[moveID].m_col = y;

    label[moveID]->move(x * CHESSBROAD_SIDE,y * CHESSBROAD_SIDE);

    //更新位置
    update();
}

QString ChessArea::checkWinner(){
    //4 为红将。20 为黑将
    if(myChess[4].m_isDead){
        return "black";
    }
    else if(myChess[20].m_isDead){
        return "red";
    }
    else{
        return "null";
    }
}

ChessArea::~ChessArea()
{
    delete ui;
}
