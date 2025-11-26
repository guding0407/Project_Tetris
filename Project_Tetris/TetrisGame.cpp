#include "TetrisGame.h"
#include "ConsoleHelper.h"

TetrisGame::TetrisGame() {
    ab_x = 5;
    ab_y = 1;
    board.setBasePosition(ab_x, ab_y);

    // 스테이지 난이도 설정
    stage_data[0] = { 40, 20, 1 };
    stage_data[1] = { 38, 18, 1 };
    stage_data[2] = { 35, 18, 20 };
    stage_data[3] = { 30, 17, 20 };
    stage_data[4] = { 25, 16, 20 };
    stage_data[5] = { 20, 14, 20 };
    stage_data[6] = { 15, 14, 20 };
    stage_data[7] = { 10, 13, 20 };
    stage_data[8] = { 6, 12, 20 };
    stage_data[9] = { 4, 11, 99999 };

    srand((unsigned)time(NULL));
}

void TetrisGame::initGame() {
    board.initBoard();
    level = 0;
    lines = 0;
    score = 0;
    hud_drawn = 0;
}

int TetrisGame::makeNewBlock() {
    int i = rand() % 100;
    if (i <= stage_data[level].stick_rate) return 0; // 막대 확률
    return (rand() % 6) + 1;
}

void TetrisGame::showCurBlock(int shape, int angle, int x, int y) {
    ConsoleHelper::setColor(TetrisBlock::getColor(shape));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            if (y + r < 0) continue;
            ConsoleHelper::setCursorPosition((c + x) * 2 + ab_x, y + r + ab_y);
            printf("■");
        }
    }
    ConsoleHelper::setColor(BLACK);
    ConsoleHelper::setCursorPosition(77, 23);
}

void TetrisGame::eraseCurBlock(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            if (y + r < 0) continue;
            ConsoleHelper::setCursorPosition((c + x) * 2 + ab_x, y + r + ab_y);
            printf("  ");
        }
    }
}

void TetrisGame::showNextBlock(int shape) {
    ConsoleHelper::setColor((level) % 6 + 1);
    for (int i = 1; i < 7; i++) {
        ConsoleHelper::setCursorPosition(33, i);
        for (int j = 0; j < 6; j++) {
            if (i == 1 || i == 6 || j == 0 || j == 5) printf("■");
            else printf("  ");
        }
    }
    // 사이드 패널에 다음 블럭 표시 (좌표 35, 3)
    showCurBlock(shape, 0, 15, 1);
}

void TetrisGame::showGameStat() {
    ConsoleHelper::setColor(GRAY);
    if (!hud_drawn) {
        ConsoleHelper::setCursorPosition(35, 7); printf("STAGE");
        ConsoleHelper::setCursorPosition(35, 9); printf("SCORE");
        ConsoleHelper::setCursorPosition(35, 12); printf("LINES");
        hud_drawn = 1;
    }
    ConsoleHelper::setCursorPosition(41, 7); printf("%d", level + 1);
    ConsoleHelper::setCursorPosition(35, 10); printf("%10d", score);
    ConsoleHelper::setCursorPosition(35, 13); printf("%10d", stage_data[level].clear_line - lines);
}

int TetrisGame::strikeCheck(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!TetrisBlock::getShape(shape, angle, r, c)) continue;
            int gy = y + r;
            int gx = x + c;

            if (gx <= 0 || gx >= 13) return 1; // 벽
            if (gy < 0) continue;
            if (gy >= 21) return 1; // 바닥
            if (board.getBlock(gy, gx)) return 1; // 블럭 충돌
        }
    }
    return 0;
}

void TetrisGame::mergeBlock(int shape, int angle, int x, int y) {
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

int TetrisGame::moveBlock(int* shape, int* angle, int* x, int* y, int* next_shape) {
    eraseCurBlock(*shape, *angle, *x, *y);
    (*y)++;

    if (strikeCheck(*shape, *angle, *x, *y)) {
        (*y)--;
        if (*y < 0) return 1; // 게임 오버

        mergeBlock(*shape, *angle, *x, *y);
        *shape = *next_shape;
        *next_shape = makeNewBlock();

        *x = 5;
        *y = -3;
        *angle = 0;

        showNextBlock(*next_shape);
        return 2; // 착지 성공
    }
    return 0;
}

void TetrisGame::checkFullLine() {
    int cleared = board.deleteFullLines(ab_x, ab_y);
    if (cleared > 0) {
        lines += cleared;
        score += 100 + (level * 10) + (rand() % 10);
        showGameStat();
        board.drawBoard(level);
    }
}

void TetrisGame::showGameOver() {
    ConsoleHelper::setColor(RED);
    ConsoleHelper::setCursorPosition(15, 8); printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    ConsoleHelper::setCursorPosition(15, 9); printf("┃**************************┃");
    ConsoleHelper::setCursorPosition(15, 10); printf("┃* GAME OVER       *┃");
    ConsoleHelper::setCursorPosition(15, 11); printf("┃**************************┃");
    ConsoleHelper::setCursorPosition(15, 12); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    Sleep(1000);
    _getch();
    system("cls");
}

void TetrisGame::showLogo() {
    ConsoleHelper::setCursorPosition(13, 3); printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    Sleep(50); ConsoleHelper::setCursorPosition(13, 4); printf("┃◆◆◆  ◆◆◆  ◆◆◆   ◆◆     ◆   ◆◆◆  ┃");
    Sleep(50); ConsoleHelper::setCursorPosition(13, 5); printf("┃ ◆    ◆     ◆    ◆ ◆    ◆   ◆    ┃");
    Sleep(50); ConsoleHelper::setCursorPosition(13, 6); printf("┃ ◆    ◆◆◆   ◆    ◆◆     ◆     ◆  ┃");
    Sleep(50); ConsoleHelper::setCursorPosition(13, 7); printf("┃ ◆    ◆     ◆    ◆ ◆    ◆     ◆  ┃");
    Sleep(50); ConsoleHelper::setCursorPosition(13, 8); printf("┃ ◆    ◆◆◆   ◆    ◆  ◆   ◆    ◆◆◆  ┃");
    Sleep(50); ConsoleHelper::setCursorPosition(13, 9); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    ConsoleHelper::setCursorPosition(28, 20); printf("Please Press Any Key~!");
    _getch();
    system("cls");
}

void TetrisGame::inputData() {
    ConsoleHelper::setColor(GRAY);
    ConsoleHelper::setCursorPosition(10, 7); printf("┏━━━━━━━━━━<GAME KEY>━━━━━━━━┓");
    ConsoleHelper::setCursorPosition(10, 8); printf("┃ UP   : Rotate Block        ┃");
    ConsoleHelper::setCursorPosition(10, 9); printf("┃ DOWN : Move One-Step Down  ┃");
    ConsoleHelper::setCursorPosition(10, 10); printf("┃ SPACE: Hard Drop           ┃");
    ConsoleHelper::setCursorPosition(10, 11); printf("┃ LEFT : Move Left           ┃");
    ConsoleHelper::setCursorPosition(10, 12); printf("┃ RIGHT: Move Right          ┃");
    ConsoleHelper::setCursorPosition(10, 13); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    while (1) {
        ConsoleHelper::setCursorPosition(10, 3);
        printf("Select Start level[1-8]:          ");
        ConsoleHelper::setCursorPosition(35, 3);

        char buf[10];
        cin >> buf;
        int sel = atoi(buf);
        if (sel >= 1 && sel <= 8) {
            level = sel - 1;
            break;
        }
    }
    system("cls");
}

// *** 원본의 main 함수 로직과 100% 동일한 구조 ***
void TetrisGame::run() {
    initGame();
    showLogo();

    while (1) {
        int is_gameover = 0;
        inputData();
        board.drawBoard(level);

        block_shape = makeNewBlock();
        next_block_shape = makeNewBlock();
        showNextBlock(next_block_shape);

        block_x = 5;
        block_y = -3;
        block_angle = 0;

        showGameStat();

        for (int i = 1; 1; i++) {
            if (_kbhit()) {
                int key = _getch();
                if (key == 0 || key == 0xE0) {
                    key = _getch();
                    switch (key) {
                    case KEY_UP:
                        if (strikeCheck(block_shape, (block_angle + 1) % 4, block_x, block_y) == 0) {
                            eraseCurBlock(block_shape, block_angle, block_x, block_y);
                            block_angle = (block_angle + 1) % 4;
                            showCurBlock(block_shape, block_angle, block_x, block_y);
                        }
                        break;
                    case KEY_LEFT:
                        if (strikeCheck(block_shape, block_angle, block_x - 1, block_y) == 0) {
                            eraseCurBlock(block_shape, block_angle, block_x, block_y);
                            block_x--;
                            showCurBlock(block_shape, block_angle, block_x, block_y);
                        }
                        break;
                    case KEY_RIGHT:
                        if (strikeCheck(block_shape, block_angle, block_x + 1, block_y) == 0) {
                            eraseCurBlock(block_shape, block_angle, block_x, block_y);
                            block_x++;
                            showCurBlock(block_shape, block_angle, block_x, block_y);
                        }
                        break;
                    case KEY_DOWN:
                        is_gameover = moveBlock(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
                        showCurBlock(block_shape, block_angle, block_x, block_y);
                        break;
                    }
                }
                else if (key == KEY_SPACE) {
                    while (is_gameover == 0) {
                        is_gameover = moveBlock(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
                    }
                    showCurBlock(block_shape, block_angle, block_x, block_y);
                }
            }

            if (i % stage_data[level].speed == 0) {
                is_gameover = moveBlock(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
                showCurBlock(block_shape, block_angle, block_x, block_y);
            }

            if (stage_data[level].clear_line <= lines) {
                level++;
                lines = 0;
                showGameStat();
                board.drawBoard(level);
                showNextBlock(next_block_shape);
            }

            if (is_gameover == 1) {
                showGameOver();
                break;
            }

            ConsoleHelper::setCursorPosition(77, 23);
            Sleep(15);
        }
        initGame();
    }
}