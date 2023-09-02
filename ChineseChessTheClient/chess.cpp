#include "chess.h"
#include "ui_chess.h"

Chess::Chess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chess)
{
    ui->setupUi(this);


}

//初始化棋子状态
void Chess::initChess(const int id){
    //将场上所有的棋子都分配一个id，用于初始化，前16个棋子为红，后面的为黑
    if(id < 0 || id > 31)//越界
    {
        return;
    }
    else{
        m_id  = id;
        m_isDead = false;
    }

    if(id < 16){
        m_isRed = true;
    }
    else{
        m_isRed = false;
    }

    //初始化坐标，类型
    switch (m_id) {
                case 0:  { m_row = 0; m_col = 9; m_type = CHE;   } break; //red_che
                case 1:  { m_row = 1; m_col = 9; m_type = MA;    } break; //red_ma
                case 2:  { m_row = 2; m_col = 9; m_type = XIANG; } break; //red_xaing
                case 3:  { m_row = 3; m_col = 9; m_type = SHI;   } break; //red_shi
                case 4:  { m_row = 4; m_col = 9; m_type = JIANG; } break; //red_jiang
                case 5:  { m_row = 5; m_col = 9; m_type = SHI;   } break; //red_shi
                case 6:  { m_row = 6; m_col = 9; m_type = XIANG; } break; //red_xiang
                case 7:  { m_row = 7; m_col = 9; m_type = MA;    } break; //red_ma
                case 8:  { m_row = 8; m_col = 9; m_type = CHE;   } break; //red_che
                case 9:  { m_row = 1; m_col = 7; m_type = PAO;   } break; //red_pao
                case 10: { m_row = 7; m_col = 7; m_type = PAO;   } break; //red_pao
                case 11: { m_row = 0; m_col = 6; m_type = BING;  } break; //red_bing
                case 12: { m_row = 2; m_col = 6; m_type = BING;  } break; //red_bing
                case 13: { m_row = 4; m_col = 6; m_type = BING;  } break; //red_bing
                case 14: { m_row = 6; m_col = 6; m_type = BING;  } break; //red_bing
                case 15: { m_row = 8; m_col = 6; m_type = BING;  } break; //red_bing

                case 16: { m_row = 0; m_col = 0; m_type = CHE;   } break; //black_che
                case 17: { m_row = 1; m_col = 0; m_type = MA;    } break; //black_ma
                case 18: { m_row = 2; m_col = 0; m_type = XIANG; } break; //black_xiang
                case 19: { m_row = 3; m_col = 0; m_type = SHI;   } break; //black_shi
                case 20: { m_row = 4; m_col = 0; m_type = JIANG; } break; //black_jiang
                case 21: { m_row = 5; m_col = 0; m_type = SHI;   } break; //black_shi
                case 22: { m_row = 6; m_col = 0; m_type = XIANG; } break; //black_xiang
                case 23: { m_row = 7; m_col = 0; m_type = MA;    } break; //black_ma
                case 24: { m_row = 8; m_col = 0; m_type = CHE;   } break; //black_che
                case 25: { m_row = 1; m_col = 2; m_type = PAO;   } break; //black_pao
                case 26: { m_row = 7; m_col = 2; m_type = PAO;   } break; //black_pao
                case 27: { m_row = 0; m_col = 3; m_type = BING;  } break; //black_bing
                case 28: { m_row = 2; m_col = 3; m_type = BING;  } break; //black_bing
                case 29: { m_row = 4; m_col = 3; m_type = BING;  } break; //black_bing
                case 30: { m_row = 6; m_col = 3; m_type = BING;  } break; //black_bing
                case 31: { m_row = 8; m_col = 3; m_type = BING;  } break; //black_bing
    }


}

QString Chess::getChessType(){
    switch(m_type){
            case JIANG: return "JIANG"; break;
            case SHI:   return "SHI";   break;
            case XIANG: return "XIANG"; break;
            case MA:    return "MA";    break;
            case CHE:   return "CHE";   break;
            case PAO:   return "PAO";   break;
            case BING:  return "BING";  break;

    }
}

Chess::~Chess()
{
    delete ui;
}
