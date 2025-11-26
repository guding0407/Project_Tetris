#pragma once
#include "Common.h"

class TetrisBoard {
private:
    char total_block[21][14]; // 보드 상태 배열
    int ab_x, ab_y;           // 화면 출력 기준 좌표

public:
    TetrisBoard();
    void setBasePosition(int x, int y);
    void initBoard(); // 보드 초기화

    void drawBoard(int level); // 전체 보드 그리기 (show_total_block)

    // 좌표값 확인 및 설정
    int getBlock(int y, int x) const;
    void setBlock(int y, int x, int val);

    // 라인 삭제 (check_full_line 로직 일부)
    int deleteFullLines(int ab_x, int ab_y);
};