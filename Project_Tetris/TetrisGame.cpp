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
    std::cout << "         TETRIS GAME        ";
  
    ConsoleHelper::setCursorPosition(10, 4);
    if (isWaterMode) std::cout << "  SINGLE PLAYER (SEA LEVEL) ";
    else            std::cout << "   SINGLE PLAYER (CLASSIC)  ";

    ConsoleHelper::setCursorPosition(10, 6);
    ConsoleHelper::setColor(WHITE);
    std::cout << "  [ Press Any Key to Start ]\n";

    while (_kbhit()) _getch();
    _getch();

    system("cls");
}

void TetrisGame::inputData() {
    ConsoleHelper::setColor(GRAY);
    int x = 10, y = 7;
    ConsoleHelper::setCursorPosition(x, y++); std::cout << "┏━━━━━━━━━━<GAME KEY>━━━━━━━━┓";
    ConsoleHelper::setCursorPosition(x, y++); std::cout << "┃ UP   : Rotate Block        ┃";
    ConsoleHelper::setCursorPosition(x, y++); std::cout << "┃ DOWN : Move One-Step Down  ┃";
    ConsoleHelper::setCursorPosition(x, y++); std::cout << "┃ SPACE: Hard Drop           ┃";
    ConsoleHelper::setCursorPosition(x, y++); std::cout << "┃ LEFT : Move Left           ┃";
    ConsoleHelper::setCursorPosition(x, y++); std::cout << "┃ RIGHT: Move Right          ┃";
    ConsoleHelper::setCursorPosition(x, y++); std::cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

    while (1) {
        ConsoleHelper::setCursorPosition(10, 3);
        ConsoleHelper::setColor(WHITE);
        std::cout << "Select Start level[1-8]:          ";
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

        // 게임 오버 처리
        int boxX = 45;
        int boxY = 16;

        ConsoleHelper::write(boxX, boxY++, "┏━━━━━━━━━━━━━━━┓", RED);
        ConsoleHelper::write(boxX, boxY++, "┃                              ┃", RED);
        ConsoleHelper::write(boxX, boxY, "┃          GAME OVER           ┃", RED); boxY++;
        ConsoleHelper::write(boxX, boxY++, "┃                              ┃", RED);
        ConsoleHelper::write(boxX, boxY++, "┗━━━━━━━━━━━━━━━┛", RED); boxY++;

        ConsoleHelper::write(boxX, boxY, "         Press Any Key...        ", WHITE);

        ConsoleHelper::render();

        while (_kbhit()) _getch();
        _getch();
        break;
    }
}