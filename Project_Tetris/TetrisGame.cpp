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
    // 1. 화면 지우기 (새 창 느낌)
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

    // 키 입력 대기
    while (_kbhit()) _getch();
    _getch();

    system("cls");
}

void TetrisGame::inputData() {
    // 이 함수는 printf를 써도 무방 (게임 루프 밖이라서)
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
    // 1. 로고 보여주기
    showLogo();

    while (1) {
        // 2. 레벨 입력 받기
        inputData();

        // 3. 더블 버퍼링 초기화
        ConsoleHelper::init();

        // 4. 싱글 플레이어 게임 객체 생성
        // (x좌표=4, y좌표=2, 해수면모드, 전투모드=false)
        TetrisCore game(4, 2, isWaterMode, false);
        game.initGame(startLevel);

        // 5. 게임 루프
        while (!game.isGameOver()) {
            // [Step 1] 버퍼 비우기
            ConsoleHelper::clearBuffer();

            // [Step 2] 키 입력 처리 (기존 싱글모드 방식 유지)
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
                    return; // ESC 누르면 종료
                }
            }

            // [Step 3] 로직 업데이트 및 그리기(Write)
            game.updateLogic();
            game.draw(); // 메모리에 그리기

            // [Step 4] 최종 렌더링
            ConsoleHelper::render();

            Sleep(20);
        }

        // 6. 게임 오버 처리
        ConsoleHelper::write(20, 10, "GAME OVER - Press Any Key", RED);
        ConsoleHelper::render(); // 마지막 화면 갱신

        while (_kbhit()) _getch(); // 버퍼 비우기
        _getch();
    }
}