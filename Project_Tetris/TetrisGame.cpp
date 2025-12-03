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
    printf("\n\n");
    printf("        ECO - TETRIS        \n");

    if (isWaterMode) printf("  SINGLE PLAYER (SEA LEVEL) \n");
    else            printf("   SINGLE PLAYER (CLASSIC)  \n");

    printf("\n");
    ConsoleHelper::setColor(WHITE);
    printf("  [ Press Any Key to Start ]\n");

    while (_kbhit()) _getch();
    _getch();

    system("cls");
}

void TetrisGame::inputData() {
    ConsoleHelper::setColor(GRAY);
    int x = 10, y = 7;
    ConsoleHelper::setCursorPosition(x, y++); printf("┏━━━━━━<GAME KEY>━━━━┓");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ UP   : Rotate Block          ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ DOWN : Move One-Step Down    ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ SPACE: Hard Drop             ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ LEFT : Move Left             ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┃ RIGHT: Move Right            ┃");
    ConsoleHelper::setCursorPosition(x, y++); printf("┗━━━━━━━━━━━━━━━┛");

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
    // 1. 로고 출력
    showLogo();

    // [수정] while(1) 무한 루프 제거
    // 게임을 한 판만 진행하고 함수를 종료해야 메인 메뉴(Launcher)로 돌아갑니다.

    // 2. 레벨 입력 및 초기화
    system("cls");
    inputData();

    ConsoleHelper::init();

    TetrisCore game(4, 2, isWaterMode, false);
    game.initGame(startLevel);

    // 3. 게임 루프
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
                return; // ESC 누르면 즉시 종료
            }
        }

        game.updateLogic();
        game.draw();

        ConsoleHelper::render();

        Sleep(20);
    }

    // 4. 게임 오버 화면 출력
    int boxX = 45;
    int boxY = 16;

    ConsoleHelper::write(boxX, boxY++, "┏━━━━━━━━━━━━━━━┓", RED);
    ConsoleHelper::write(boxX, boxY++, "┃                              ┃", RED);

    ConsoleHelper::write(boxX, boxY, "┃          GAME OVER           ┃", RED); boxY++;

    ConsoleHelper::write(boxX, boxY++, "┃                              ┃", RED);

    ConsoleHelper::write(boxX, boxY, "┃      Press Any Key...        ┃", WHITE); boxY++;

    ConsoleHelper::write(boxX, boxY++, "┃                              ┃", RED);
    ConsoleHelper::write(boxX, boxY++, "┗━━━━━━━━━━━━━━━┛", RED);

    ConsoleHelper::render();

    // 5. 키 입력 대기 후 함수 종료 -> 메인 메뉴로 복귀
    while (_kbhit()) _getch();
    _getch();
}