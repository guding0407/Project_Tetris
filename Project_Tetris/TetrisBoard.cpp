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
    // 내부 공간 초기화
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 14; j++) {
            if (j == 0 || j == 13) total_block[i][j] = WALL_BLOCK;
            else total_block[i][j] = EMPTY_BLOCK;
        }
    }
    // [중요] 바닥 벽 설정 (이게 없으면 뚫림)
    for (int j = 0; j < 14; j++) total_block[20][j] = WALL_BLOCK;
    waterHeight = 0;
}

void TetrisBoard::drawBoard(int level) {
    // 상단 벽
   /* for (int i = 0; i < 14; i++) {
        ConsoleHelper::write(ab_x + (i * 2), ab_y - 1, "■", DARK_GRAY);
    }*/

    // 보드 내부
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 14; j++) {
            int blockVal = total_block[i][j];
            int color = BLACK;
            std::string shape = "  ";

            if (blockVal == WALL_BLOCK) {
                if (j == 0 || j == 13 || i == 20) color = (level % 6) + 1;
                else color = DARK_GRAY;
                shape = "■";
            }
            else if (blockVal == WATER_BLOCK) {
                color = WATER_COLOR;
                shape = "■";
            }
            else if (blockVal == 1) { // 굳은 블록
                color = GRAY;
                shape = "■";
            }
            // 그 외에는 "  " (공백)

            ConsoleHelper::write(ab_x + (j * 2), ab_y + i, shape, color);
        }
    }
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
    // 그리기 업데이트는 drawBoard에서 수행됨
}

void TetrisBoard::lowerWaterLevel() {
    if (waterHeight <= 0) return;

    // 현재 물의 가장 윗부분 (이제 블록이 채워질 공간)
    int targetRow = 20 - waterHeight;

    // [핵심 변경] 단순히 물만 지우는 게 아니라, 
    // 물 위에 있던 모든 블록들을 아래로 한 칸씩 '이사' 시킵니다.
    for (int i = targetRow; i > 0; i--) {
        for (int j = 1; j < 13; j++) {
            total_block[i][j] = total_block[i - 1][j];
        }
    }

    // 맨 윗줄은 비워줍니다 (천장이 내려오진 않으니까요)
    for (int j = 1; j < 13; j++) {
        total_block[0][j] = EMPTY_BLOCK;
    }

    waterHeight--;

    // 데이터만 수정하면, 이후 메인 루프의 draw()에서 
    // 내려앉은 전체 화면을 예쁘게 그려줄 것입니다.
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
        if (i >= 20) continue;

        int j;
        for (j = 1; j < 13; j++) {
            if (total_block[i][j] == EMPTY_BLOCK) break;
            if (total_block[i][j] == WATER_BLOCK) break;
        }

        if (j == 13) {
            linesCleared++;

            // [핵심 수정 1] 애니메이션 시작 전에 배경(현재 화면)을 복구!
            // 이걸 안 하면 빈 화면에서 파란 블록만 깜빡거립니다.
            ConsoleHelper::recover();

            // 1. 라인 삭제 이펙트 (파란색)
            for (int k = 1; k < 13; k++) {
                ConsoleHelper::write((k * 2) + ab_x, i + ab_y, "■", SKY_BLUE);
                ConsoleHelper::render(); // 이제 배경이 있는 상태에서 이펙트만 그려짐
                Sleep(10);
            }

            // [핵심 수정 2] 이펙트 지우기 
            // write로 지우고 바로 render하지 말고, 
            // recover()로 다시 배경을 불러온 뒤 해당 줄만 지우는 것이 더 안전합니다.
            // 하지만 간단하게 공백으로 덮고 render해도 됩니다.
            for (int k = 1; k < 13; k++) {
                ConsoleHelper::write((k * 2) + ab_x, i + ab_y, "  ", BLACK);
            }
            ConsoleHelper::render();

            // 3. 데이터 이동 (기존 로직 유지)
            for (int k = i; k > 0; k--) {
                for (int col = 1; col < 13; col++)
                    total_block[k][col] = total_block[k - 1][col];
            }
            for (int col = 1; col < 13; col++) total_block[0][col] = EMPTY_BLOCK;
        }
    }
    return linesCleared;
}