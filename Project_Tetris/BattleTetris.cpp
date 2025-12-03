#include "BattleTetris.h"
#include "TetrisCore.h"
#include "ConsoleHelper.h"

// 키보드 가상 키 코드
constexpr int VK_W = 0x57;
constexpr int VK_A = 0x41;
constexpr int VK_S = 0x53;
constexpr int VK_D = 0x44;

void BattleTetris::run(bool waterMode) {
    // 1. 초기화
    ConsoleHelper::init();

    // 2. 로고 화면
    system("cls");
    ConsoleHelper::setCursorVisible(false);

    ConsoleHelper::setColor(SKY_BLUE);
    ConsoleHelper::setCursorPosition(54, 5);
    std::cout << "ECO - TETRIS";

    if (waterMode) {
        ConsoleHelper::setCursorPosition(46, 7);
        std::cout << "2-PLAYER BATTLE (SEA LEVEL)";
    }
    else {
        ConsoleHelper::setCursorPosition(47, 7);
        std::cout << "2-PLAYER BATTLE (CLASSIC)";
    }

    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(47, 10);
    std::cout << "[ Press Any Key to Start ]";

    while (_kbhit()) _getch();
    _getch();

    // 3. 게임 화면 준비
    system("cls");
    ConsoleHelper::setCursorVisible(false);

    // 4. 플레이어 생성
    TetrisCore player1(4, 2, waterMode, true);
    TetrisCore player2(50, 2, waterMode, true);

    player1.initGame(0);
    player2.initGame(0);

    Sleep(500);
    GetAsyncKeyState(VK_RETURN);

    // ==========================================
    // [변수 선언]
    // ==========================================
    bool p1_rotate_pressed = false;
    bool p1_drop_pressed = false;
    bool p2_rotate_pressed = false;
    bool p2_drop_pressed = false;

    int p1_move_timer = 0;
    int p2_move_timer = 0;

    constexpr int MOVE_SPEED = 3;
    constexpr int WINNING_SCORE = 1000;

    while (true) {
        // [Step 1] 버퍼 비우기
        ConsoleHelper::clearBuffer();

        // ==========================================
        // [Step 2] 승패 판정
        // ==========================================
        int winner = 0;

        if (player1.isGameOver()) winner = 2;
        else if (player2.isGameOver()) winner = 1;
        else if (player1.getScore() >= WINNING_SCORE) winner = 1;
        else if (player2.getScore() >= WINNING_SCORE) winner = 2;

        if (winner != 0) {
            int boxX = 42;
            int boxY = 12;

            ConsoleHelper::write(boxX, boxY++, "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓", YELLOW);
            ConsoleHelper::write(boxX, boxY++, "┃                             ┃", YELLOW);

            if (winner == 1) ConsoleHelper::write(boxX, boxY, "┃       PLAYER 1 WIN!       ┃", YELLOW);
            else             ConsoleHelper::write(boxX, boxY, "┃       PLAYER 2 WIN!       ┃", YELLOW);
            boxY++;

            ConsoleHelper::write(boxX, boxY++, "┃                             ┃", YELLOW);
            ConsoleHelper::write(boxX, boxY++, "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛", YELLOW);

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

        // ==========================================
        // [Step 3] 키 입력 처리
        // ==========================================
        if (p1_move_timer > 0) p1_move_timer--;
        if (p2_move_timer > 0) p2_move_timer--;

        // --- [Player 1] ---
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

        // --- [Player 2] ---
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

        // ==========================================
        // [Step 4] 로직 업데이트 & 그리기
        // ==========================================
        player1.updateLogic();
        player1.draw();

        player2.updateLogic();
        player2.draw();

        // [삭제됨] 공격 시스템 로직 제거
        /*
        int p1_attack = player1.getLinesCleared();
        int p2_attack = player2.getLinesCleared();
        if (p1_attack >= 2) player2.addGarbageLines(p1_attack - 1);
        if (p2_attack >= 2) player1.addGarbageLines(p2_attack - 1);
        */

        // ==========================================
        // [Step 5] 최종 렌더링
        // ==========================================
        ConsoleHelper::render();

        Sleep(20);
    }
}