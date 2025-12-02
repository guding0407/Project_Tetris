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
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 14; j++) {
            // 좌표를 명확하게 지정하여 밀림 방지
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
            ConsoleHelper::setColor(WATER_COLOR);
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

// [핵심 수정] 이펙트 잔상 제거 로직 추가
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

            // 1. 라인 삭제 이펙트 (파란색 ==)
            ConsoleHelper::setColor(SKY_BLUE);
            ConsoleHelper::setCursorPosition(1 * 2 + ab_x, i + ab_y);
            for (int k = 1; k < 13; k++) { printf("=="); Sleep(10); }

            // 2. [추가] 이펙트를 즉시 지움 (검은색 공백)
            // 이렇게 해야 데이터가 내려오기 전에 화면이 깨끗해져서 잔상이 남지 않습니다.
            ConsoleHelper::setColor(BLACK);
            ConsoleHelper::setCursorPosition(1 * 2 + ab_x, i + ab_y);
            for (int k = 1; k < 13; k++) { printf("  "); }

            // 3. 데이터 이동 (윗 줄을 아래로 내림)
            for (int k = i; k > 0; k--) {
                for (int col = 1; col < 13; col++)
                    total_block[k][col] = total_block[k - 1][col];
            }
            // 최상단 줄은 비움
            for (int col = 1; col < 13; col++) total_block[0][col] = EMPTY_BLOCK;
        }
    }
    return linesCleared;
}