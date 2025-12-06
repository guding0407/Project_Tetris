#pragma once
#include "Common.h"

class TetrisBoard {     // 게임이 진행되는 격자판(20행 14열)을 관리
private:
    char total_block[21][14];
    int ab_x, ab_y;
    int waterHeight;

public:
    TetrisBoard();
    void setBasePosition(int x, int y);
    void initBoard();

    void drawBoard(int level);
    void raiseWaterLevel(); // 해수면 상승
    void lowerWaterLevel(); // 해수면 하강
    int getWaterHeight() const { return waterHeight; }

    int getBlock(int y, int x) const;
    void setBlock(int y, int x, int val);
    int deleteFullLines(int ab_x, int ab_y);
};