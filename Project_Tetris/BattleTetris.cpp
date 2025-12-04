#include "BattleTetris.h"
#include "TetrisCore.h"
#include "ConsoleHelper.h"
#include <iostream>

constexpr int VK_W = 0x57;
constexpr int VK_A = 0x41;
constexpr int VK_S = 0x53;
constexpr int VK_D = 0x44;

// [수정] 생성자 구현: waterMode를 멤버 변수로 저장
BattleTetris::BattleTetris(bool waterMode) : isWaterMode(waterMode) {}

void BattleTetris::showControls() {
    system("cls");
    ConsoleHelper::setCursorVisible(false);

    ConsoleHelper::setColor(YELLOW);
    ConsoleHelper::setCursorPosition(12, 3);
    std::cout << "┏━━━━━━━━━━━━━━━━ BATTLE CONTROLS ━━━━━━━━━━━━━━━━┓";

    // P1
    int p1_x = 15;
    int y = 6;
    ConsoleHelper::setColor(SKY_BLUE);
    ConsoleHelper::setCursorPosition(p1_x, y++); std::cout << "[ PLAYER 1 ]";
    ConsoleHelper::setColor(GRAY);
    ConsoleHelper::setCursorPosition(p1_x, y++); std::cout << "  (WASD)    ";
    y++;
    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(p1_x, y++); std::cout << "Rotate :  W ";
    ConsoleHelper::setCursorPosition(p1_x, y++); std::cout << "Left   :  A ";
    ConsoleHelper::setCursorPosition(p1_x, y++); std::cout << "Right  :  D ";
    ConsoleHelper::setCursorPosition(p1_x, y++); std::cout << "Down   :  S ";
    ConsoleHelper::setCursorPosition(p1_x, y++); std::cout << "Drop   : SPACE";

    // 구분선
    for (int i = 6; i < 15; i++) {
        ConsoleHelper::setColor(DARK_GRAY);
        ConsoleHelper::setCursorPosition(37, i); std::cout << "┃";
    }

    // P2
    int p2_x = 47;
    y = 6;
    ConsoleHelper::setColor(GREEN);
    ConsoleHelper::setCursorPosition(p2_x, y++); std::cout << "[ PLAYER 2 ]";
    ConsoleHelper::setColor(GRAY);
    ConsoleHelper::setCursorPosition(p2_x, y++); std::cout << " (ARROW KEY)";
    y++;
    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(p2_x, y++); std::cout << "Rotate :  ↑ ";
    ConsoleHelper::setCursorPosition(p2_x, y++); std::cout << "Left   :  ← ";
    ConsoleHelper::setCursorPosition(p2_x, y++); std::cout << "Right  :  → ";
    ConsoleHelper::setCursorPosition(p2_x, y++); std::cout << "Down   :  ↓ ";
    ConsoleHelper::setCursorPosition(p2_x, y++); std::cout << "Drop   : ENTER";

    ConsoleHelper::setColor(YELLOW);
    ConsoleHelper::setCursorPosition(12, 17);
    std::cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛";

    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(27, 20);
    std::cout << "Press Any Key to FIGHT!";

    while (_kbhit()) _getch();
    _getch();

    system("cls");
}

// [수정] 인자 없는 run() 구현
void BattleTetris::run() {
    ConsoleHelper::init();

    system("cls");
    ConsoleHelper::setCursorVisible(false);

    ConsoleHelper::setColor(SKY_BLUE);
    ConsoleHelper::setCursorPosition(10, 3);

    std::cout << "     UPGRADE TETRIS        ";
    
    ConsoleHelper::setCursorPosition(10, 4);
    if (isWaterMode) {
        std::cout << "2-PLAYER BATTLE (SEA LEVEL)";
    }
    else {
        std::cout << "2-PLAYER BATTLE (CLASSIC)";
    }


    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(10, 6);
    std::cout << "[ Press Any Key to Start ]";


    while (_kbhit()) _getch();
    _getch();

    showControls();

    system("cls");
    ConsoleHelper::setCursorVisible(false);

    // [수정] 멤버 변수 isWaterMode 사용
    TetrisCore player1(4, 2, isWaterMode, true);
    TetrisCore player2(50, 2, isWaterMode, true);

    player1.initGame(0);
    player2.initGame(0);

    Sleep(500);
    GetAsyncKeyState(VK_RETURN);

    // 점수 기반 공격 플래그 (300 / 600 / 900)
    static bool p1Attack[3] = { false, false, false };
    static bool p2Attack[3] = { false, false, false };


    bool p1_rotate_pressed = false;
    bool p1_drop_pressed = false;
    bool p2_rotate_pressed = false;
    bool p2_drop_pressed = false;

    int p1_move_timer = 0;
    int p2_move_timer = 0;
    constexpr int MOVE_SPEED = 3;
    constexpr int WINNING_SCORE = 1000;

    while (true) {
        ConsoleHelper::clearBuffer();

        int winner = 0;

        if (player1.isGameOver()) winner = 2;
        else if (player2.isGameOver()) winner = 1;
        else if (player1.getScore() >= WINNING_SCORE) winner = 1;
        else if (player2.getScore() >= WINNING_SCORE) winner = 2;

        if (winner != 0) {
            int boxX = 42;
            int boxY = 12;

            ConsoleHelper::write(boxX, boxY++, "┏━━━━━━━━━━━━━━━┓", YELLOW);
            ConsoleHelper::write(boxX, boxY++, "┃                              ┃", YELLOW);

            if (winner == 1) ConsoleHelper::write(boxX, boxY, "┃       PLAYER 1 WIN!          ┃", YELLOW);
            else             ConsoleHelper::write(boxX, boxY, "┃       PLAYER 2 WIN!          ┃", YELLOW);
            boxY++;

            ConsoleHelper::write(boxX, boxY++, "┃                              ┃", YELLOW);
            ConsoleHelper::write(boxX, boxY++, "┗━━━━━━━━━━━━━━━┛", YELLOW);

            if (player1.getScore() >= WINNING_SCORE || player2.getScore() >= WINNING_SCORE) {
                ConsoleHelper::write(boxX + 2, boxY + 1, "       SCORE REACHED!      ", GREEN);
                ConsoleHelper::write(boxX + 2, boxY + 2, "      Press Any Key...     ", WHITE);
            }
            else {
                ConsoleHelper::write(boxX + 2, boxY + 1, "      Press Any Key...     ", WHITE);
            }

            ConsoleHelper::render();

            while (_kbhit()) _getch();
            _getch();
            break;
        }

        // P1 → P2 공격
        int p1Score = player1.getScore();
        if (!p1Attack[0] && p1Score >= 300) {
            player2.addGarbageLines(1);
            p1Attack[0] = true;
        }
        if (!p1Attack[1] && p1Score >= 600) {
            player2.addGarbageLines(1);
            p1Attack[1] = true;
        }
        if (!p1Attack[2] && p1Score >= 900) {
            player2.addGarbageLines(1);
            p1Attack[2] = true;
        }

        // P2 → P1 공격
        int p2Score = player2.getScore();
        if (!p2Attack[0] && p2Score >= 300) {
            player1.addGarbageLines(1);
            p2Attack[0] = true;
        }
        if (!p2Attack[1] && p2Score >= 600) {
            player1.addGarbageLines(1);
            p2Attack[1] = true;
        }
        if (!p2Attack[2] && p2Score >= 900) {
            player1.addGarbageLines(1);
            p2Attack[2] = true;
        }


        if (p1_move_timer > 0) p1_move_timer--;
        if (p2_move_timer > 0) p2_move_timer--;

        // P1 Input
        if (GetAsyncKeyState(VK_W) & 0x8000) {
            if (!p1_rotate_pressed) { player1.handleInput(KEY_UP); p1_rotate_pressed = true; }
        }
        else p1_rotate_pressed = false;

        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (!p1_drop_pressed) { player1.handleInput(KEY_SPACE); p1_drop_pressed = true; }
        }
        else p1_drop_pressed = false;

        if (p1_move_timer == 0) {
            bool moved = false;
            if (GetAsyncKeyState(VK_S) & 0x8000) { player1.handleInput(KEY_DOWN); moved = true; }
            else if (GetAsyncKeyState(VK_A) & 0x8000) { player1.handleInput(KEY_LEFT); moved = true; }
            else if (GetAsyncKeyState(VK_D) & 0x8000) { player1.handleInput(KEY_RIGHT); moved = true; }
            if (moved) p1_move_timer = MOVE_SPEED;
        }

        // P2 Input
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (!p2_rotate_pressed) { player2.handleInput(KEY_UP); p2_rotate_pressed = true; }
        }
        else p2_rotate_pressed = false;

        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            if (!p2_drop_pressed) { player2.handleInput(KEY_SPACE); p2_drop_pressed = true; }
        }
        else p2_drop_pressed = false;

        if (p2_move_timer == 0) {
            bool moved = false;
            if (GetAsyncKeyState(VK_DOWN) & 0x8000) { player2.handleInput(KEY_DOWN); moved = true; }
            else if (GetAsyncKeyState(VK_LEFT) & 0x8000) { player2.handleInput(KEY_LEFT); moved = true; }
            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { player2.handleInput(KEY_RIGHT); moved = true; }
            if (moved) p2_move_timer = MOVE_SPEED;
        }

        if (GetAsyncKeyState(KEY_ESC) & 0x8000) break;

        player1.updateLogic();
        player1.draw();

        player2.updateLogic();
        player2.draw();

        ConsoleHelper::render();
        Sleep(20);
    }
}