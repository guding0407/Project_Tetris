#include "TetrisBoard.h"
#include "ConsoleHelper.h"

TetrisBoard::TetrisBoard() {
    ab_x = 5;
    ab_y = 1;
    initBoard();
}

void TetrisBoard::setBasePosition(int x, int y) {
    ab_x = x;
    ab_y = y;
}

void TetrisBoard::initBoard() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 14; j++) {
            if (j == 0 || j == 13) total_block[i][j] = 1; // 벽
            else total_block[i][j] = 0;
        }
    }
    for (int j = 0; j < 14; j++) total_block[20][j] = 1; // 바닥
}

void TetrisBoard::drawBoard(int level) {
    ConsoleHelper::setColor(DARK_GRAY);
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 14; j++) {
            if (j == 0 || j == 13 || i == 20)
                ConsoleHelper::setColor((level % 6) + 1); // 레벨별 벽 색상
            else
                ConsoleHelper::setColor(DARK_GRAY);

            ConsoleHelper::setCursorPosition((j * 2) + ab_x, i + ab_y);
            if (total_block[i][j] == 1) printf("■");
            else printf("  ");
        }
    }
    ConsoleHelper::setColor(BLACK);
    ConsoleHelper::setCursorPosition(77, 23);
}

int TetrisBoard::getBlock(int y, int x) const {
    if (y < 0 || y > 20 || x < 0 || x > 13) return 1; // 범위 밖은 벽
    return total_block[y][x];
}

void TetrisBoard::setBlock(int y, int x, int val) {
    if (y >= 0 && y <= 20 && x >= 0 && x <= 13)
        total_block[y][x] = val;
}

int TetrisBoard::deleteFullLines(int ab_x, int ab_y) {
    int linesCleared = 0;
    for (int i = 0; i < 20; i++) {
        int j;
        for (j = 1; j < 13; j++) {
            if (total_block[i][j] == 0) break;
        }
        if (j == 13) { // 한 줄 꽉 참
            linesCleared++;
            // 라인 삭제 효과
            ConsoleHelper::setColor(BLUE);
            ConsoleHelper::setCursorPosition(1 * 2 + ab_x, i + ab_y);
            for (int k = 1; k < 13; k++) { printf("□"); Sleep(10); }

            ConsoleHelper::setCursorPosition(1 * 2 + ab_x, i + ab_y);
            for (int k = 1; k < 13; k++) { printf("  "); Sleep(10); }

            // 줄 내리기
            for (int k = i; k > 0; k--) {
                for (int col = 1; col < 13; col++)
                    total_block[k][col] = total_block[k - 1][col];
            }
            for (int col = 1; col < 13; col++) total_block[0][col] = 0;
        }
    }
    return linesCleared;
}