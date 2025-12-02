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
    ConsoleHelper::setColor(SKY_BLUE);
    printf("\n\n");
    printf("        ECO - TETRIS        \n");
    if (isWaterMode) printf("  STOP THE RISING SEA LEVEL \n");
    else            printf("     CLASSIC SINGLE MODE    \n");
    printf("\n");
    ConsoleHelper::setColor(WHITE);
    printf("  [ Press Any Key to Start ]\n");
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
        inputData(); // 레벨 입력 받기

        // 1인용 게임이므로 (4, 2) 위치에 생성
        TetrisCore game(4, 2, isWaterMode);
        game.initGame(startLevel);


        // 게임 루프
        while (!game.isGameOver()) {
            if (_kbhit()) {
                int key = _getch();
                if (key == 0 || key == 0xE0) {
                    key = _getch();
                    // 방향키 처리는 TetrisCore에게 위임
                    if (key == KEY_UP) game.handleInput(KEY_UP);
                    else if (key == KEY_DOWN) game.handleInput(KEY_DOWN);
                    else if (key == KEY_LEFT) game.handleInput(KEY_LEFT);
                    else if (key == KEY_RIGHT) game.handleInput(KEY_RIGHT);
                }
                else if (key == KEY_SPACE) {
                    game.handleInput(KEY_SPACE);
                }
                else if (key == KEY_ESC) {
                    // ESC 누르면 게임 종료
                    return;
                }
            }

            // 게임 상태 업데이트 (블록 낙하 등)
            game.updateLogic();

            Sleep(10); // 게임 속도 조절
        }

        // 게임 오버 처리
        // 간단히 메시지 띄우고 다시 시작
        ConsoleHelper::setCursorPosition(20, 10);
        ConsoleHelper::setColor(RED);
        printf("GAME OVER - Press Any Key to Restart");
        _getch();
    }
}