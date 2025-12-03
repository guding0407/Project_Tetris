#include "BattleTetris.h"
#include "TetrisCore.h"
#include "ConsoleHelper.h"


// Ű���� ���� Ű �ڵ�

// [Rule 2] constexpr 상수 사용

constexpr int VK_W = 0x57;
constexpr int VK_A = 0x41;
constexpr int VK_S = 0x53;
constexpr int VK_D = 0x44;

// [추가] 조작법 설명 화면 구현
void BattleTetris::showControls() {
    system("cls");
    ConsoleHelper::setCursorVisible(false);

    // --- 제목 ---
    ConsoleHelper::setColor(YELLOW);
    ConsoleHelper::setCursorPosition(12, 3);
    printf("┏━━━━━━━━━━━━━━━━ BATTLE CONTROLS ━━━━━━━━━━━━━━━━┓");

    // --- PLAYER 1 (Left) ---
    int p1_x = 15;
    int y = 6;
    ConsoleHelper::setColor(SKY_BLUE);
    ConsoleHelper::setCursorPosition(p1_x, y++); printf("[ PLAYER 1 ]");
    ConsoleHelper::setColor(GRAY);
    ConsoleHelper::setCursorPosition(p1_x, y++); printf("  (WASD)    ");
    y++; // 공백
    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(p1_x, y++); printf("Rotate :  W ");
    ConsoleHelper::setCursorPosition(p1_x, y++); printf("Left   :  A ");
    ConsoleHelper::setCursorPosition(p1_x, y++); printf("Right  :  D ");
    ConsoleHelper::setCursorPosition(p1_x, y++); printf("Down   :  S ");
    ConsoleHelper::setCursorPosition(p1_x, y++); printf("Drop   : SPACE");

    // --- 구분선 ---
    for (int i = 6; i < 15; i++) {
        ConsoleHelper::setColor(DARK_GRAY);
        ConsoleHelper::setCursorPosition(37, i); printf("┃");
    }

    // --- PLAYER 2 (Right) ---
    int p2_x = 47;
    y = 6;
    ConsoleHelper::setColor(GREEN);
    ConsoleHelper::setCursorPosition(p2_x, y++); printf("[ PLAYER 2 ]");
    ConsoleHelper::setColor(GRAY);
    ConsoleHelper::setCursorPosition(p2_x, y++); printf(" (ARROW KEY)");
    y++; // 공백
    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(p2_x, y++); printf("Rotate :  ↑ ");
    ConsoleHelper::setCursorPosition(p2_x, y++); printf("Left   :  ← ");
    ConsoleHelper::setCursorPosition(p2_x, y++); printf("Right  :  → ");
    ConsoleHelper::setCursorPosition(p2_x, y++); printf("Down   :  ↓ ");
    ConsoleHelper::setCursorPosition(p2_x, y++); printf("Drop   : ENTER");

    // --- 하단 안내 ---
    ConsoleHelper::setColor(YELLOW);
    ConsoleHelper::setCursorPosition(12, 17);
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(27, 20);
    printf("Press Any Key to FIGHT!");

    // 키 입력 대기
    while (_kbhit()) _getch();
    _getch();

    system("cls");
}

void BattleTetris::run(bool waterMode) {
    // 1. �ʱ�ȭ
    ConsoleHelper::init();

    // 2. �ΰ� ȭ��
    system("cls");
    ConsoleHelper::setCursorVisible(false);

    ConsoleHelper::setColor(SKY_BLUE);

    ConsoleHelper::setCursorPosition(10, 3);
    std::cout << "ECO - TETRIS";
    
  ConsoleHelper::setCursorPosition(10, 4);
    if (waterMode) {
        std::cout << "2-PLAYER BATTLE (SEA LEVEL)";
    }
    else {
        std::cout << "2-PLAYER BATTLE (CLASSIC)";
    }

    ConsoleHelper::setColor(WHITE);
    ConsoleHelper::setCursorPosition(10, 6);
    std::cout << "[ Press Any Key to Start ]";
  
    showControls();


    while (_kbhit()) _getch();
    _getch();

    // 3. ���� ȭ�� �غ�

    // 3. 게임 화면 준비

    system("cls");
    ConsoleHelper::setCursorVisible(false);

    // 4. �÷��̾� ����
    TetrisCore player1(4, 2, waterMode, true);
    TetrisCore player2(50, 2, waterMode, true);

    player1.initGame(0);
    player2.initGame(0);

    Sleep(500);
    GetAsyncKeyState(VK_RETURN);

    // ==========================================
    // [���� ����]
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
        // [Step 1] ���� ����
        ConsoleHelper::clearBuffer();

        // ==========================================
        // [Step 2] ���� ����
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
        // [Step 3] Ű �Է� ó��
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
        // [Step 4] ���� ������Ʈ & �׸���
        // ==========================================
        player1.updateLogic();
        player1.draw();

        player2.updateLogic();
        player2.draw();

        // [������] ���� �ý��� ���� ����
        /*
        int p1_attack = player1.getLinesCleared();
        int p2_attack = player2.getLinesCleared();
        if (p1_attack >= 2) player2.addGarbageLines(p1_attack - 1);
        if (p2_attack >= 2) player1.addGarbageLines(p2_attack - 1);
        */

        // ==========================================
        // [Step 5] ���� ������
        // ==========================================
        ConsoleHelper::render();

        Sleep(20);
    }
}