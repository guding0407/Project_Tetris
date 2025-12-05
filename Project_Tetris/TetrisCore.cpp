#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "TetrisCore.h"
#include "ConsoleHelper.h"

TetrisCore::TetrisCore(int x, int y, bool waterMode, bool isBattle) {
    ab_x = x;
    ab_y = y;
    isWaterMode = waterMode;
    board.setBasePosition(ab_x, ab_y);

    if (isBattle) {
        stage_data[0] = { 40, 20, 5, 300 };
        stage_data[1] = { 38, 18, 5, 280 };
        stage_data[2] = { 35, 18, 5, 250 };
        stage_data[3] = { 30, 17, 5, 220 };
        stage_data[4] = { 25, 16, 5, 200 };
        stage_data[5] = { 20, 14, 5, 180 };
        stage_data[6] = { 15, 14, 5, 160 };
        stage_data[7] = { 10, 13, 5, 140 };
        stage_data[8] = { 6, 12, 5, 120 };
        stage_data[9] = { 4, 11, 99999, 80 };
    }
    else {
        stage_data[0] = { 40, 20, 20, 300 };
        stage_data[1] = { 38, 18, 20, 280 };
        stage_data[2] = { 35, 18, 20, 250 };
        stage_data[3] = { 30, 17, 20, 220 };
        stage_data[4] = { 25, 16, 20, 200 };
        stage_data[5] = { 20, 14, 20, 180 };
        stage_data[6] = { 15, 14, 20, 160 };
        stage_data[7] = { 10, 13, 20, 140 };
        stage_data[8] = { 6, 12, 20, 120 };
        stage_data[9] = { 4, 11, 99999, 80 };
    }
}

void TetrisCore::initGame(int startLevel) {
    board.initBoard();
    level = startLevel;
    lines = 0;
    score = 0;
    water_tick_count = 0;
    speed_counter = 0;
    is_gameover = false;
    clearedLineCount = 0;

    block_shape = makeNewBlock();
    next_block_shape = makeNewBlock();

    block_x = 5;
    block_y = -3;
    block_angle = 0;

    draw();
}

int TetrisCore::makeNewBlock() {
    int i = rand() % 100;
    if (i <= stage_data[level].stick_rate) return 0;
    return (rand() % 6) + 1;
}

void TetrisCore::draw() {
    board.drawBoard(level);
    showGameStat();
    showNextBlock(next_block_shape);
    showCurBlock(block_shape, block_angle, block_x, block_y);
}

void TetrisCore::updateLogic() {
    if (is_gameover) return;

    if (isWaterMode) {
        water_tick_count++;
        if (water_tick_count >= stage_data[level].water_speed) {
            water_tick_count = 0;
            if (board.getWaterHeight() >= 18) {
                is_gameover = true;
            }
            else if (board.getWaterHeight() < 10) {
                board.raiseWaterLevel();
                draw();
            }
        }
    }

    speed_counter++;
    if (speed_counter >= stage_data[level].speed) {
        speed_counter = 0;
        // [Rule 1] 참조자 사용 (주소 연산자 & 제거)
        int result = moveBlock(block_shape, block_angle, block_x, block_y, next_block_shape);
        if (result == 1) is_gameover = true;
        showCurBlock(block_shape, block_angle, block_x, block_y);
    }
}

bool TetrisCore::handleInput(int key) {
    if (is_gameover) return false;

    bool acted = false;
    switch (key) {
    case KEY_UP:
        if (strikeCheck(block_shape, (block_angle + 1) % 4, block_x, block_y) == 0) {
            eraseCurBlock(block_shape, block_angle, block_x, block_y);
            block_angle = (block_angle + 1) % 4;
            acted = true;
        }
        else if (strikeCheck(block_shape, (block_angle + 1) % 4, block_x - 1, block_y) == 0) {
            eraseCurBlock(block_shape, block_angle, block_x, block_y);
            block_angle = (block_angle + 1) % 4;
            block_x--;
            acted = true;
        }
        else if (strikeCheck(block_shape, (block_angle + 1) % 4, block_x + 1, block_y) == 0) {
            eraseCurBlock(block_shape, block_angle, block_x, block_y);
            block_angle = (block_angle + 1) % 4;
            block_x++;
            acted = true;
        }
        else if (strikeCheck(block_shape, (block_angle + 1) % 4, block_x - 2, block_y) == 0) {
            eraseCurBlock(block_shape, block_angle, block_x, block_y);
            block_angle = (block_angle + 1) % 4;
            block_x -= 2;
            acted = true;
        }
        else if (strikeCheck(block_shape, (block_angle + 1) % 4, block_x + 2, block_y) == 0) {
            eraseCurBlock(block_shape, block_angle, block_x, block_y);
            block_angle = (block_angle + 1) % 4;
            block_x += 2;
            acted = true;
        }
        break;
    case KEY_LEFT:
        if (strikeCheck(block_shape, block_angle, block_x - 1, block_y) == 0) {
            eraseCurBlock(block_shape, block_angle, block_x, block_y);
            block_x--;
            acted = true;
        }
        break;
    case KEY_RIGHT:
        if (strikeCheck(block_shape, block_angle, block_x + 1, block_y) == 0) {
            eraseCurBlock(block_shape, block_angle, block_x, block_y);
            block_x++;
            acted = true;
        }
        break;
    case KEY_DOWN:
    {
        // [Rule 1] 참조자 사용 (주소 연산자 & 제거)
        int result = moveBlock(block_shape, block_angle, block_x, block_y, next_block_shape);
        if (result == 1) is_gameover = true;
        acted = true;
        break;
    }
    case KEY_SPACE:
        // [Rule 1] 참조자 사용 (주소 연산자 & 제거)
        while (moveBlock(block_shape, block_angle, block_x, block_y, next_block_shape) == 0);
        acted = true;
        break;
    }

    if (acted) showCurBlock(block_shape, block_angle, block_x, block_y);
    return acted;
}

void TetrisCore::addGarbageLines(int count) {
    for (int i = 0; i < count; i++) board.raiseWaterLevel();
    draw();
}

void TetrisCore::checkFullLine() {
    int cleared = board.deleteFullLines(ab_x, ab_y);
    if (cleared > 0) {
        lines += cleared;

        // [수정] 콤보 보너스 제거 -> 단순 비례 계산으로 변경
        
        // 최종 점수: 기본 점수 * 지운 줄 수
        int unitScore = 100;
        score += (unitScore * cleared);

        clearedLineCount += cleared;

        if (isWaterMode) {
            for (int k = 0; k < cleared; k++) board.lowerWaterLevel();
        }

        if (lines >= stage_data[level].clear_line) {
            lines = 0;
            level++;
            if (level > 9) level = 9;
        }
        draw();
    }
}
void TetrisCore::showGameStat() {
    int uiX = ab_x + 30;
    int uiY = ab_y + 2;

    ConsoleHelper::write(uiX, uiY, "NEXT", WHITE);
    ConsoleHelper::write(uiX, uiY + 5, "LV: ", WHITE);
    ConsoleHelper::writeInt(uiX + 4, uiY + 5, level + 1, WHITE);

    ConsoleHelper::write(uiX, uiY + 7, "SCORE: ", WHITE);
    ConsoleHelper::writeInt(uiX + 7, uiY + 7, score, WHITE);

    int remainLines = stage_data[level].clear_line - lines;
    if (remainLines < 0) remainLines = 0;
    ConsoleHelper::write(uiX, uiY + 9, "GOAL: ", WHITE);
    ConsoleHelper::writeInt(uiX + 6, uiY + 9, remainLines, WHITE);
}

void TetrisCore::showNextBlock(int shape) {
    int uiX = ab_x + 30;
    int uiY = ab_y + 1;

    for (int i = 0; i < 4; i++) ConsoleHelper::write(uiX, uiY + i, "        ", BLACK);

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (TetrisBlock::getShape(shape, 0, r, c)) {
                ConsoleHelper::write((c * 2) + uiX, r + uiY, "■", TetrisBlock::getColor(shape));
            }
        }
    }
}

void TetrisCore::showCurBlock(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            if (y + r < 0) continue;

            ConsoleHelper::write((c + x) * 2 + ab_x, y + r + ab_y, "■", TetrisBlock::getColor(shape));
        }
    }
}

void TetrisCore::eraseCurBlock(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            if (y + r < 0) continue;

            int gy = y + r;
            int waterLine = 20 - board.getWaterHeight();

            if (isWaterMode && gy < 20 && gy >= waterLine && (c + x) > 0 && (c + x) < 13) {
                ConsoleHelper::write((c + x) * 2 + ab_x, gy + ab_y, "■", WATER_COLOR);
            }
            else {
                ConsoleHelper::write((c + x) * 2 + ab_x, gy + ab_y, "  ", BLACK);
            }
        }
    }
}

int TetrisCore::strikeCheck(int shape, int angle, int x, int y) {
    // getShapeMask 대신 기존 getShape 사용
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            int gy = y + r;
            int gx = x + c;

            if (gx <= 0 || gx >= 13) return 1;
            if (gy < 0) continue;
            if (gy >= 21) return 1;

            if (board.getBlock(gy, gx) != EMPTY_BLOCK) return 1;
        }
    }
    return 0;
}

void TetrisCore::mergeBlock(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (TetrisBlock::getShape(shape, angle, r, c)) {
                board.setBlock(y + r, x + c, 1);
            }
        }
    }
    checkFullLine();
    board.drawBoard(level);
}

// [Rule 1] 인자 타입 int&로 변경
int TetrisCore::moveBlock(int& shape, int& angle, int& x, int& y, int& next_shape) {
    // [Rule 1] 포인터 역참조(*) 제거
    eraseCurBlock(shape, angle, x, y);
    y++;

    if (strikeCheck(shape, angle, x, y)) {
        y--;
        if (y < 0) return 1;

        mergeBlock(shape, angle, x, y);
        shape = next_shape;
        next_shape = makeNewBlock();

        x = 5;
        y = -3;
        angle = 0;

        showNextBlock(next_shape);
        return 2;
    }
    return 0;
}