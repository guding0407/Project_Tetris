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
    for (int i = 0; i < 14; i++) {
        ConsoleHelper::write(ab_x + (i * 2), ab_y - 1, "■", DARK_GRAY);
    }

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
    int targetRow = 20 - waterHeight;

    for (int j = 1; j < 13; j++) {
        if (total_block[targetRow][j] == WATER_BLOCK) {
            total_block[targetRow][j] = EMPTY_BLOCK;
        }
    }
    waterHeight--;
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
        // 바닥(20)은 검사하지 않음
        if (i >= 20) continue;

        int j;
        for (j = 1; j < 13; j++) {
            if (total_block[i][j] == EMPTY_BLOCK) break;
            if (total_block[i][j] == WATER_BLOCK) break;
        }

        if (j == 13) {
            linesCleared++;

            // [수정 1] 이펙트를 "==" 대신 "■"로 변경하여 버퍼 구조 통일
            for (int k = 1; k < 13; k++) {
                ConsoleHelper::write((k * 2) + ab_x, i + ab_y, "■", SKY_BLUE);
                ConsoleHelper::render();
                Sleep(10);
            }

            // [수정 2] 이펙트 지우기 및 ★즉시 렌더링★ 추가
            // 화면을 깨끗한 검은색으로 밀어버려야 잔상이 남지 않습니다.
            for (int k = 1; k < 13; k++) {
                ConsoleHelper::write((k * 2) + ab_x, i + ab_y, "  ", BLACK);
            }
            ConsoleHelper::render(); // [중요] 지운 것을 화면에 즉시 반영

            // 3. 데이터 이동
            for (int k = i; k > 0; k--) {
                for (int col = 1; col < 13; col++)
                    total_block[k][col] = total_block[k - 1][col];
            }
            for (int col = 1; col < 13; col++) total_block[0][col] = EMPTY_BLOCK;
        }
    }
    return linesCleared;
}