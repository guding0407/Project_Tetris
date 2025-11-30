#include "TetrisBoard.h"
#include "ConsoleHelper.h"

TetrisBoard::TetrisBoard() {
    ab_x = 4;
    ab_y = 2;
    initBoard();
}

void TetrisBoard::setBasePosition(int x, int y) {
    ab_x = x;
    ab_y = y;
}

void TetrisBoard::initBoard() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 14; j++) {
            if (j == 0 || j == 13) total_block[i][j] = WALL_BLOCK;
            else total_block[i][j] = EMPTY_BLOCK;
        }
    }
    for (int j = 0; j < 14; j++) total_block[20][j] = WALL_BLOCK;
    waterHeight = 0;
}

void TetrisBoard::drawBoard(int level) {
    // 상단 벽 (기존 유지, 필요 시 여기도 루프 안에서 좌표 지정 방식으로 변경 가능)
    /*ConsoleHelper::setColor(DARK_GRAY);
    ConsoleHelper::setCursorPosition(ab_x, ab_y - 1);
    for (int i = 0; i < 14; i++) printf("■");*/

    // [수정된 부분] 보드 내부 그리기
    for (int i = 0; i < 21; i++) {
        // 기존: 줄 시작할 때 한 번만 커서 이동
        // ConsoleHelper::setCursorPosition(ab_x, ab_y + i); 

        for (int j = 0; j < 14; j++) {
            // 변경: 모든 칸을 그릴 때마다 명확한 좌표를 찍어줍니다.
            // 이렇게 하면 떨어지는 블록(showCurBlock)과 좌표 계산식이 완전히 동일해져서 밀림이 사라집니다.
            ConsoleHelper::setCursorPosition(ab_x + (j * 2), ab_y + i);

            int blockVal = total_block[i][j];

            if (blockVal == WALL_BLOCK) {
                if (j == 0 || j == 13 || i == 20)
                    ConsoleHelper::setColor((level % 6) + 1);
                else
                    ConsoleHelper::setColor(DARK_GRAY);
                printf("■");
            }
            else if (blockVal == WATER_BLOCK) {
                ConsoleHelper::setColor(WATER_COLOR);
                printf("■");
            }
            else if (blockVal == 1) { // 굳은 블록
                ConsoleHelper::setColor(GRAY);
                printf("■");
            }
            else {
                ConsoleHelper::setColor(BLACK);
                printf("  ");
            }
        }
    }

    // 하단 벽 (기존 유지)
    /*ConsoleHelper::setCursorPosition(ab_x, ab_y + 21);
    ConsoleHelper::setColor(DARK_GRAY);
    for (int i = 0; i < 14; i++) printf("■");*/

    ConsoleHelper::setColor(BLACK);
}

void TetrisBoard::raiseWaterLevel() {
    if (waterHeight >= 19) return;
    waterHeight++;

    int targetRow = 20 - waterHeight;

    for (int j = 1; j < 13; j++) {
        if (total_block[targetRow][j] == EMPTY_BLOCK) {
            total_block[targetRow][j] = WATER_BLOCK;
        }
    }

    for (int j = 1; j < 13; j++) {
        ConsoleHelper::setCursorPosition((j * 2) + ab_x, targetRow + ab_y);

        if (total_block[targetRow][j] == WATER_BLOCK) {
            ConsoleHelper::setColor(WATER_COLOR);
            printf("■");
        }
        else if (total_block[targetRow][j] == 1) {
            ConsoleHelper::setColor(WATER_COLOR); // 물 속 블럭
            printf("■");
        }
    }
    ConsoleHelper::setColor(BLACK);
}

void TetrisBoard::lowerWaterLevel() {
    if (waterHeight <= 0) return;
    int targetRow = 20 - waterHeight;

    for (int j = 1; j < 13; j++) {
        if (total_block[targetRow][j] == WATER_BLOCK) {
            total_block[targetRow][j] = EMPTY_BLOCK;
        }
    }

    for (int j = 1; j < 13; j++) {
        ConsoleHelper::setCursorPosition((j * 2) + ab_x, targetRow + ab_y);
        if (total_block[targetRow][j] == 1) {
            ConsoleHelper::setColor(GRAY);
            printf("■");
        }
        else {
            ConsoleHelper::setColor(BLACK);
            printf("  ");
        }
    }
    waterHeight--;
    ConsoleHelper::setColor(BLACK);
}

int TetrisBoard::getBlock(int y, int x) const {
    if (y < 0 || y > 20 || x < 0 || x > 13) return WALL_BLOCK;
    return total_block[y][x];
}

void TetrisBoard::setBlock(int y, int x, int val) {
    if (y >= 0 && y <= 20 && x >= 0 && x <= 13)
        total_block[y][x] = val;
}

int TetrisBoard::deleteFullLines(int ab_x, int ab_y) {
    int linesCleared = 0;
    int checkLimit = 20 - waterHeight;

    for (int i = 0; i < checkLimit; i++) {
        int j;
        for (j = 1; j < 13; j++) {
            if (total_block[i][j] == EMPTY_BLOCK) break;
            if (total_block[i][j] == WATER_BLOCK) break;
        }

        if (j == 13) {
            linesCleared++;
            ConsoleHelper::setColor(SKY_BLUE);
            ConsoleHelper::setCursorPosition(1 * 2 + ab_x, i + ab_y);
            for (int k = 1; k < 13; k++) { printf("=="); Sleep(10); }

            for (int k = i; k > 0; k--) {
                for (int col = 1; col < 13; col++)
                    total_block[k][col] = total_block[k - 1][col];
            }
            for (int col = 1; col < 13; col++) total_block[0][col] = EMPTY_BLOCK;
        }
    }
    return linesCleared;
}