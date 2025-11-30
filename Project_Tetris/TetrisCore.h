#pragma once
#include "Common.h"
#include "TetrisBoard.h"
#include "TetrisBlock.h"

// 테트리스 게임의 한 '판'을 담당하는 핵심 클래스
class TetrisCore {
protected:
    TetrisBoard board;
    Stage stage_data[10];

    int level;
    int score;
    int lines; // 현재까지 지운 라인 수
    int clearedLineCount; // 이번 턴에 지운 라인 수 (공격용)

    int ab_x, ab_y; // 보드 기준 좌표
    int water_tick_count;
    bool isWaterMode;

    int block_shape;
    int block_angle;
    int block_x;
    int block_y;
    int next_block_shape;
    int speed_counter; // 속도 조절용

    bool is_gameover;

public:
    TetrisCore(int x, int y, bool waterMode);

    void setLevel(int l) { level = l; }

    void initGame();
    void draw(); // 전체 그리기
    void updateLogic(); // 게임 로직 진행 (자동 낙하 등)
    bool handleInput(int key); // 키 입력 처리 (성공시 true)

    // 공격받았을 때 줄 추가 (2인용 전용)
    void addGarbageLines(int count);

    // 상태 확인용 Getter
    int getLinesCleared() { int t = clearedLineCount; clearedLineCount = 0; return t; }
    bool isGameOver() const { return is_gameover; }
    int getScore() const { return score; }

protected:
    int makeNewBlock();
    void showCurBlock(int shape, int angle, int x, int y);
    void eraseCurBlock(int shape, int angle, int x, int y);
    void showNextBlock(int shape);
    void showGameStat();

    int strikeCheck(int shape, int angle, int x, int y);
    void mergeBlock(int shape, int angle, int x, int y);
    int moveBlock(int* shape, int* angle, int* x, int* y, int* next_shape);
    void checkFullLine();
};