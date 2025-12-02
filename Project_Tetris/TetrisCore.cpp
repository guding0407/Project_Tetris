#include "TetrisCore.h"
#include "ConsoleHelper.h"

TetrisCore::TetrisCore(int x, int y, bool waterMode, bool isBattle) {
    ab_x = x;
    ab_y = y;
    isWaterMode = waterMode;
    board.setBasePosition(ab_x, ab_y);

    // 난이도 데이터 (기존 유지)
    stage_data[0] = { 40, 20, 1, 300 };
    stage_data[1] = { 38, 18, 1, 280 };
    stage_data[2] = { 35, 18, 20, 250 };
    stage_data[3] = { 30, 17, 20, 220 };
    stage_data[4] = { 25, 16, 20, 200 };
    stage_data[5] = { 20, 14, 20, 180 };
    stage_data[6] = { 15, 14, 20, 160 };
    stage_data[7] = { 10, 13, 20, 140 };
    stage_data[8] = { 6, 12, 20, 120 };
    stage_data[9] = { 4, 11, 99999, 80 };
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
            else {
                board.raiseWaterLevel();
                draw();
            }
        }
    }

    speed_counter++;
    if (speed_counter >= stage_data[level].speed) {
        speed_counter = 0;
        int result = moveBlock(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
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
        int result = moveBlock(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
        if (result == 1) is_gameover = true;
        acted = true;
        break;
    }
    case KEY_SPACE:
        while (moveBlock(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape) == 0);
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
        score += 100 + (level * 10);
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

// [수정] showGameStat: printf -> write
void TetrisCore::showGameStat() {
    int uiX = ab_x + 30;
    int uiY = ab_y + 2;

    ConsoleHelper::write(uiX, uiY, "NEXT", WHITE);
    ConsoleHelper::write(uiX, uiY + 5, "LV: ", WHITE);
    ConsoleHelper::writeInt(uiX + 4, uiY + 5, level + 1, WHITE);
    ConsoleHelper::write(uiX, uiY + 7, "SC: ", WHITE);
    ConsoleHelper::writeInt(uiX + 4, uiY + 7, score, WHITE);

    int remainLines = stage_data[level].clear_line - lines;
    if (remainLines < 0) remainLines = 0;
    ConsoleHelper::write(uiX, uiY + 9, "GOAL: ", WHITE);
    ConsoleHelper::writeInt(uiX + 6, uiY + 9, remainLines, WHITE);
}

// [수정] showNextBlock: printf -> write
void TetrisCore::showNextBlock(int shape) {
    int uiX = ab_x + 30;
    int uiY = ab_y + 1;

    // 지우기
    for (int i = 0; i < 4; i++) ConsoleHelper::write(uiX, uiY + i, "        ", BLACK);

    // 그리기
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (TetrisBlock::getShape(shape, 0, r, c)) {
                // [중요] write 사용
                ConsoleHelper::write((c * 2) + uiX, r + uiY, "■", TetrisBlock::getColor(shape));
            }
        }
    }
}

// [수정] showCurBlock: printf -> write (잔상 해결 핵심)
void TetrisCore::showCurBlock(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            if (y + r < 0) continue;

            // [중요] setCursorPosition + printf 대신 write 사용
            ConsoleHelper::write((c + x) * 2 + ab_x, y + r + ab_y, "■", TetrisBlock::getColor(shape));
        }
    }
}

// [수정] eraseCurBlock: printf -> write
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
                // [중요] write 사용
                ConsoleHelper::write((c + x) * 2 + ab_x, gy + ab_y, "  ", BLACK);
            }
        }
    }
}

int TetrisCore::strikeCheck(int shape, int angle, int x, int y) {
    // [최적화] 함수 호출 반복 대신 비트마스크를 한 번 가져옴
    unsigned short mask = TetrisBlock::getShapeMask(shape, angle);
    unsigned short bitChecker = 0x8000; // 1000 0000 0000 0000 (최상위 비트)

    for (int i = 0; i < 16; i++) {
        // 해당 비트가 1인 경우에만 충돌 검사 수행 (비트가 0이면 루프 패스)
        if (mask & bitChecker) {
            int r = i / 4; // 행
            int c = i % 4; // 열

            int gy = y + r;
            int gx = x + c;


            if (gx <= 0 || gx >= 13) return 1;
            if (gy < 0) continue;
            if (gy >= 21) return 1;

            int blockType = board.getBlock(gy, gx);
            if (blockType != EMPTY_BLOCK) return 1;
=======
            // 1. 벽 충돌 (좌우)
            if (gx <= 0 || gx >= 13) return 1;

            // 2. 바닥 충돌
            if (gy >= 21) return 1;

            // 3. 보드판의 다른 블록과 충돌 (화면 위쪽(gy < 0)은 검사 제외)
            if (gy >= 0) {
                if (board.getBlock(gy, gx) != EMPTY_BLOCK) return 1;
            }
        }
        // 검사 비트를 오른쪽으로 한 칸 이동
        bitChecker >>= 1;
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

int TetrisCore::moveBlock(int* shape, int* angle, int* x, int* y, int* next_shape) {
    eraseCurBlock(*shape, *angle, *x, *y);
    (*y)++;

    if (strikeCheck(*shape, *angle, *x, *y)) {
        (*y)--;
        if (*y < 0) return 1;

        mergeBlock(*shape, *angle, *x, *y);
        *shape = *next_shape;
        *next_shape = makeNewBlock();

        *x = 5;
        *y = -3;
        *angle = 0;

        showNextBlock(*next_shape);
        return 2;
    }
    return 0;
}