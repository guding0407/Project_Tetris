#include "TetrisCore.h"
#include "ConsoleHelper.h"

TetrisCore::TetrisCore(int x, int y, bool waterMode) {
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

void TetrisCore::initGame() {
    board.initBoard();
    level = 0;
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

    draw(); // 초기 화면 그리기
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

// 1프레임 로직 (루프 안에서 호출됨)
void TetrisCore::updateLogic() {
    if (is_gameover) return;

    // 해수면 모드 로직
    if (isWaterMode) {
        water_tick_count++;
        if (water_tick_count >= stage_data[level].water_speed) {
            water_tick_count = 0;
            if (board.getWaterHeight() >= 18) {
                is_gameover = true;
            }
            else {
                board.raiseWaterLevel();
                draw(); // 변화가 있을 때 다시 그리기
            }
        }
    }

    // 블록 자동 낙하 로직
    speed_counter++;
    if (speed_counter >= stage_data[level].speed) {
        speed_counter = 0;
        int result = moveBlock(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
        if (result == 1) is_gameover = true;
        showCurBlock(block_shape, block_angle, block_x, block_y);
    }
}

// 키 입력 처리
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
            block_angle = (block_angle + 1) % 4; block_x--; acted = true;
        }
        else if (strikeCheck(block_shape, (block_angle + 1) % 4, block_x + 1, block_y) == 0) {
            eraseCurBlock(block_shape, block_angle, block_x, block_y);
            block_angle = (block_angle + 1) % 4; block_x++; acted = true;
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
    // 2인용 공격 받기: 바닥에서 회색 블록이 올라옴
    // 구현 단순화를 위해 물 높이를 올리는 것으로 대체하거나, 
    // TetrisBoard에 바닥 올리기 기능을 추가해야 함.
    // 여기서는 간단히 '물 높이 상승' 함수를 재사용하여 공격 효과를 냄
    for (int i = 0; i < count; i++) board.raiseWaterLevel();
    draw();
}

// --- 아래는 기존 TetrisGame의 내부 함수들을 그대로 가져옴 (수정 없음) ---
// (moveBlock, checkFullLine, strikeCheck, showCurBlock 등등...)
// 단, checkFullLine에서 clearedLineCount를 업데이트 해야 함.

void TetrisCore::checkFullLine() {
    int cleared = board.deleteFullLines(ab_x, ab_y);
    if (cleared > 0) {
        lines += cleared;
        score += 100 + (level * 10);
        clearedLineCount += cleared; // 공격용 카운트

        if (isWaterMode) {
            for (int k = 0; k < cleared; k++) board.lowerWaterLevel();
        }
        draw();
    }
}

// 나머지 함수들(moveBlock, strikeCheck 등)은 기존 TetrisGame.cpp와 동일하게 복사하되,
// 클래스명만 TetrisCore로 변경. 
// drawUIFrame 등 위치 좌표는 ab_x, ab_y를 기준으로 상대 좌표로 수정 필요.

void TetrisCore::showGameStat() {
    // UI 위치를 보드 옆으로 상대적으로 배치
    int uiX = ab_x + 30;
    int uiY = ab_y + 2;

    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(uiX, uiY);      printf("NEXT");
    ConsoleHelper::setCursorPosition(uiX, uiY + 5);  printf("LV: %d", level + 1);
    ConsoleHelper::setCursorPosition(uiX, uiY + 7);  printf("SC: %d", score);
}

void TetrisCore::showNextBlock(int shape) {
    int uiX = ab_x + 30;
    int uiY = ab_y + 1;

    // 기존 블록 지우기
    for (int i = 0; i < 4; i++) {
        ConsoleHelper::setCursorPosition(uiX, uiY + i); printf("        ");
    }

    ConsoleHelper::setColor(TetrisBlock::getColor(shape));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (TetrisBlock::getShape(shape, 0, r, c)) {
                ConsoleHelper::setCursorPosition((c * 2) + uiX, r + uiY);
                printf("■");
            }
        }
    }
}
// moveBlock, strikeCheck, mergeBlock, eraseCurBlock 등은 기존 코드 그대로 사용
// 단, eraseCurBlock과 drawBoard는 앞서 수정한 '밀림 방지' 코드가 적용된 상태여야 함.

void TetrisCore::showCurBlock(int shape, int angle, int x, int y) {
    ConsoleHelper::setColor(TetrisBlock::getColor(shape));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            if (y + r < 0) continue;
            // ab_x, ab_y를 더해 상대 좌표로 출력
            ConsoleHelper::setCursorPosition((c + x) * 2 + ab_x, y + r + ab_y);
            printf("■");
        }
    }
    ConsoleHelper::setColor(BLACK);
    // 커서 숨기기용 (위치는 크게 중요하지 않음)
    ConsoleHelper::setCursorPosition(ab_x + 20, ab_y + 20);
}

void TetrisCore::eraseCurBlock(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            if (y + r < 0) continue;

            int gy = y + r;
            int waterLine = 20 - board.getWaterHeight();

            // ab_x, ab_y를 더해 상대 좌표로 이동
            ConsoleHelper::setCursorPosition((c + x) * 2 + ab_x, gy + ab_y);

            // 물이 있는 곳인지 확인 (밀림 방지 로직 포함)
            if (isWaterMode && gy < 20 && gy >= waterLine && (c + x) > 0 && (c + x) < 13) {
                ConsoleHelper::setColor(WATER_COLOR);
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

int TetrisCore::strikeCheck(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            int gy = y + r;
            int gx = x + c;

            if (gx <= 0 || gx >= 13) return 1; // 벽 충돌
            if (gy < 0) continue;
            if (gy >= 21) return 1; // 바닥 충돌

            // 보드판의 블록과 충돌 확인
            int blockType = board.getBlock(gy, gx);
            if (blockType != EMPTY_BLOCK) return 1;
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

int TetrisCore::moveBlock(int* shape, int* angle, int* x, int* y, int* next_shape) {
    eraseCurBlock(*shape, *angle, *x, *y);
    (*y)++;

    if (strikeCheck(*shape, *angle, *x, *y)) {
        (*y)--;
        if (*y < 0) return 1; // 게임 오버 (천장에 닿음)

        mergeBlock(*shape, *angle, *x, *y);
        *shape = *next_shape;
        *next_shape = makeNewBlock();

        *x = 5;
        *y = -3;
        *angle = 0;

        showNextBlock(*next_shape);

        // 블록이 바닥에 닿았으므로 즉시 리턴하되, 게임오버는 아님(2는 착지 의미)
        return 2;
    }
    return 0; // 정상 이동
}