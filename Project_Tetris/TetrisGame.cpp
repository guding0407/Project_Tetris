#include "TetrisGame.h"
#include "ConsoleHelper.h"
#include <conio.h>
#include <iostream>

using namespace std;

TetrisGame::TetrisGame(bool waterMode) {
    isWaterMode = waterMode;
    startLevel = 0;
}

void TetrisGame::showLogo() {
    system("cls");
    ConsoleHelper::setCursorVisible(false);
    ConsoleHelper::setColor(SKY_BLUE);
    ConsoleHelper::setCursorPosition(10, 3);
    printf("        ECO - TETRIS        \n");

    ConsoleHelper::setCursorPosition(10, 4);
    if (isWaterMode) printf("  SINGLE PLAYER (SEA LEVEL)");
    else            printf("   SINGLE PLAYER (CLASSIC)");

    ConsoleHelper::setCursorPosition(10, 6);
    ConsoleHelper::setColor(WHITE);
    printf("  [ Press Any Key to Start ]");

    while (_kbhit()) _getch();
    _getch();

    system("cls");
}

void TetrisGame::inputData() {
    ConsoleHelper::setColor(GRAY);
    int x = 10, y = 7;
    ConsoleHelper::setCursorPosition(x, y++); printf("┏━━━━━━━━━━<GAME KEY>━━━━━━━━┓");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ UP   : Rotate Block        ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ DOWN : Move One-Step Down  ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ SPACE: Hard Drop           ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ LEFT : Move Left           ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ RIGHT: Move Right          ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    while (1) {
        ConsoleHelper::setCursorPosition(10, 3);
        ConsoleHelper::setColor(WHITE);
        printf("Select Start level[1-8]:          ");
        ConsoleHelper::setCursorPosition(35, 3);

        char buf[10];
        cin >> buf;
        int sel = atoi(buf);
        if (sel >= 1 && sel <= 8) {
            startLevel = sel - 1;
            break;
        }
    }
    system("cls");
}

void TetrisGame::run() {
    
    showLogo();

    while (1) {
        // [수정 1] 게임 재시작 시 화면을 깨끗하게 지움 (겹침 방지)
        system("cls");

        inputData();

        ConsoleHelper::init();

        TetrisCore game(4, 2, isWaterMode, false);
        game.initGame(startLevel);

        while (!game.isGameOver()) {
            ConsoleHelper::clearBuffer();

            if (_kbhit()) {
                int key = _getch();
                if (key == 0 || key == 0xE0) {
                    key = _getch();
                    if (key == KEY_UP) game.handleInput(KEY_UP);
                    else if (key == KEY_DOWN) game.handleInput(KEY_DOWN);
                    else if (key == KEY_LEFT) game.handleInput(KEY_LEFT);
                    else if (key == KEY_RIGHT) game.handleInput(KEY_RIGHT);
                }
                else if (key == KEY_SPACE) {
                    game.handleInput(KEY_SPACE);
                }
                else if (key == KEY_ESC) {
                    return;
                }
            }

            game.updateLogic();
            game.draw();

            ConsoleHelper::render();

            Sleep(20);
        }

        // [수정 2] 게임 오버 시 깔끔한 박스 출력
        int boxX = 15;
        int boxY = 10;
        ConsoleHelper::write(boxX, boxY++, "┏━━━━━━━━━━━━━━━┓", RED);
        ConsoleHelper::write(boxX, boxY++, "┃                              ┃", RED);
        ConsoleHelper::write(boxX, boxY++, "┃          GAME OVER           ┃", RED);
        ConsoleHelper::write(boxX, boxY++, "┃                              ┃", RED);
        ConsoleHelper::write(boxX, boxY++, "┃      Press Any Key...        ┃", RED);
        ConsoleHelper::write(boxX, boxY++, "┃                              ┃", RED);
        ConsoleHelper::write(boxX, boxY++, "┗━━━━━━━━━━━━━━━┛", RED);

        ConsoleHelper::render(); // 박스 출력

        while (_kbhit()) _getch(); // 키 버퍼 비우기
        _getch(); // 키 입력 대기
    }
}