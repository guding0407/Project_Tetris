#pragma once
#include "Common.h"
#include "TetrisBoard.h"
#include "TetrisBlock.h"

class TetrisGame {
private:
    TetrisBoard board;
    Stage stage_data[10];

    // 게임 상태 변수
    int level;
    int score;
    int lines;
    int hud_drawn;
    int ab_x, ab_y;

    // 블럭 변수
    int block_shape;
    int block_angle;
    int block_x;
    int block_y;
    int next_block_shape;

    // 내부 함수들 (원본 함수명과 매핑됨)
    void initGame();
    int makeNewBlock();
    void showCurBlock(int shape, int angle, int x, int y);
    void eraseCurBlock(int shape, int angle, int x, int y);
    void showNextBlock(int shape);
    void showGameStat();
    void showGameOver();
    void showLogo();
    void inputData(); // 시작 레벨 입력

    // 로직 함수
    int strikeCheck(int shape, int angle, int x, int y);
    void mergeBlock(int shape, int angle, int x, int y);
    int moveBlock(int* shape, int* angle, int* x, int* y, int* next_shape);
    void checkFullLine();

public:
    TetrisGame();
    void run(); // 게임 메인 루프
};