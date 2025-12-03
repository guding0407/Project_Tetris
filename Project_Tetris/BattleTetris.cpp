#include "BattleTetris.h"
#include "TetrisCore.h"
#include "ConsoleHelper.h"

// 키보드 가상 키 코드 (플레이어 1 - WASD)
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

void BattleTetris::run(bool waterMode) {
    // 1. 초기화
    ConsoleHelper::init();

    // 2. 로고 화면
    system("cls");
    ConsoleHelper::setColor(SKY_BLUE);
    printf("\n\n");
    printf("        ECO - TETRIS        \n");

    if (waterMode) printf(" 2-PLAYER BATTLE (SEA LEVEL)\n");
    else           printf("  2-PLAYER BATTLE (CLASSIC) \n");

    printf("\n");
    ConsoleHelper::setColor(WHITE);
    printf("  [ Press Any Key to Start ]\n");

    // 키 입력 대기
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

    // P2 하드드롭 버그 수정용 대기
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
    const int MOVE_SPEED = 3;

    // [추가] 승리 점수 목표 설정
    const int WINNING_SCORE = 300;

    while (true) {
        // [Step 1] 버퍼 비우기
        ConsoleHelper::clearBuffer();

        // ==========================================
        // [Step 2] 승패 판정 (사망 또는 점수 달성)
        // ==========================================
        int winner = 0; // 0:진행중, 1:P1승리, 2:P2승리

        // 1. 상대방이 죽었을 때 승리
        if (player1.isGameOver()) winner = 2;
        else if (player2.isGameOver()) winner = 1;

        // 2. [추가됨] 목표 점수(1000점) 달성 시 승리
        else if (player1.getScore() >= WINNING_SCORE) winner = 1;
        else if (player2.getScore() >= WINNING_SCORE) winner = 2;

        if (winner != 0) {
            // 결과 박스 출력
            int boxX = 32;
            int boxY = 10;

            ConsoleHelper::write(boxX, boxY++, "┏━━━━━━━━━┓", YELLOW);
            ConsoleHelper::write(boxX, boxY++, "┃                  ┃", YELLOW);

            if (winner == 1) ConsoleHelper::write(boxX, boxY, "┃   PLAYER 1 WIN!  ┃", YELLOW);
            else             ConsoleHelper::write(boxX, boxY, "┃   PLAYER 2 WIN!  ┃", YELLOW);
            boxY++;

            ConsoleHelper::write(boxX, boxY++, "┃                  ┃", YELLOW);
            ConsoleHelper::write(boxX, boxY++, "┗━━━━━━━━━┛", YELLOW);

            // 승리 사유 출력 (선택 사항)
            if (player1.getScore() >= WINNING_SCORE || player2.getScore() >= WINNING_SCORE) {
                ConsoleHelper::write(boxX + 2, boxY + 1, "SCORE REACHED!", GREEN);
                ConsoleHelper::write(boxX + 2, boxY + 2, "Press Any Key...", WHITE);
            }
            else {
                ConsoleHelper::write(boxX + 2, boxY + 1, "Press Any Key...", WHITE);
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

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        // ==========================================
        // [Step 4] 로직 업데이트 & 그리기
        // ==========================================
        player1.updateLogic();
        player1.draw();

        player2.updateLogic();
        player2.draw();

        // 공격 시스템
        int p1_attack = player1.getLinesCleared();
        int p2_attack = player2.getLinesCleared();
        if (p1_attack >= 2) player2.addGarbageLines(p1_attack - 1);
        if (p2_attack >= 2) player1.addGarbageLines(p2_attack - 1);

        // ==========================================
        // [Step 5] 최종 렌더링
        // ==========================================
        ConsoleHelper::render();

        Sleep(20);
    }
}