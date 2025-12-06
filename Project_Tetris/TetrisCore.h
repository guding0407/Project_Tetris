#pragma once
#include "Common.h"
#include "TetrisBoard.h"
#include "TetrisBlock.h"

class TetrisCore { // TetrisBlock과 TetrisBoard를 조립하여 실제 게임 규칙을 돌리는 엔진 역할
protected:
    TetrisBoard board;
    Stage stage_data[10];

    int level;
    int score;
    int lines;
    int clearedLineCount;

    int ab_x, ab_y;
    int water_tick_count;
    bool isWaterMode;

    int block_shape;
    int block_angle;
    int block_x;
    int block_y;
    int next_block_shape;
    int speed_counter;

    bool is_gameover;

public:
    TetrisCore(int x, int y, bool waterMode, bool isBattle = false);

    void setLevel(int l) { level = l; }
    void initGame(int startLevel = 0);
    void draw();
    void updateLogic();
    bool handleInput(int key);
    void addGarbageLines(int count);

    int getLinesCleared() { int t = clearedLineCount; clearedLineCount = 0; return t; }
    bool isGameOver() const { return is_gameover; }
    int getScore() const { return score; }

protected:
    int makeNewBlock();
    void showCurBlock(int shape, int angle, int x, int y);
    void eraseCurBlock(int shape, int angle, int x, int y);
    void showNextBlock(int shape);
    void showGameStat();

    int strikeCheck(int shape, int angle, int x, int y);    // 블록이 벽이나 다른 블록에 부딪혔는지 검사
    void mergeBlock(int shape, int angle, int x, int y);

    int moveBlock(int& shape, int& angle, int& x, int& y, int& next_shape);

    void checkFullLine();
};