#include "BattleTetris.h"
#include "TetrisCore.h"
#include "ConsoleHelper.h"

// 키보드 가상 키 코드 (플레이어 1 - WASD)
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

void BattleTetris::run(bool waterMode) {
    // 1. 초기화 (더블 버퍼링용)
    ConsoleHelper::init();

    // 2. 로고 화면 출력 (printf 사용해도 무관한 구간)
    system("cls");
    ConsoleHelper::setColor(SKY_BLUE);
    printf("\n\n");
    printf("        ECO - TETRIS        \n");

    // 모드에 따라 부제목 다르게 출력
    if (waterMode) printf(" 2-PLAYER BATTLE (SEA LEVEL)\n");
    else           printf("  2-PLAYER BATTLE (CLASSIC) \n");

    printf("\n");
    ConsoleHelper::setColor(WHITE);
    printf("  [ Press Any Key to Start ]\n");

    // 키 입력 대기
    while (_kbhit()) _getch();
    _getch();

    // 3. 게임 화면 전환
    system("cls");
    ConsoleHelper::setCursorVisible(false);

    // 4. 플레이어 생성 (x좌표, y좌표, 해수면모드, 전투모드=true)
    TetrisCore player1(4, 2, waterMode, true);
    TetrisCore player2(50, 2, waterMode, true);

    // 5. 게임 초기화 (레벨 0부터 시작)
    player1.initGame(0);
    player2.initGame(0);

    // ==========================================
    // [변수 선언] 입력 제어용
    // ==========================================

    // 회전/하드드롭 중복 입력 방지 (토글 방식)
    bool p1_rotate_pressed = false;
    bool p1_drop_pressed = false;
    bool p2_rotate_pressed = false;
    bool p2_drop_pressed = false;

    // 이동 속도 조절용 타이머
    int p1_move_timer = 0;
    int p2_move_timer = 0;

    // ★ 이동 감도 조절 (숫자가 클수록 이동이 느려짐. 3~5 추천)
    const int MOVE_SPEED = 3;

    while (true) {
        // [Step 1] 버퍼 비우기 (매 프레임 시작)
        ConsoleHelper::clearBuffer();

        // ==========================================
        // [Step 2] 승패 판정 및 결과 출력
        // ==========================================
        int winner = 0; // 0:진행중, 1:P1승리, 2:P2승리

        if (player1.isGameOver()) {
            winner = 2; // P1 사망 -> P2 승리
        }
        else if (player2.isGameOver()) {
            winner = 1; // P2 사망 -> P1 승리
        }

        if (winner != 0) {
            // 결과 박스 그리기 (write 사용)
            int boxX = 32;
            int boxY = 10;

            ConsoleHelper::write(boxX, boxY++, "┏━━━━━━━━━━━━━━━━━━┓", YELLOW);
            ConsoleHelper::write(boxX, boxY++, "┃                  ┃", YELLOW);

            if (winner == 1) ConsoleHelper::write(boxX, boxY, "┃   PLAYER 1 WIN!  ┃", YELLOW);
            else             ConsoleHelper::write(boxX, boxY, "┃   PLAYER 2 WIN!  ┃", YELLOW);
            boxY++;

            ConsoleHelper::write(boxX, boxY++, "┃                  ┃", YELLOW);
            ConsoleHelper::write(boxX, boxY++, "┗━━━━━━━━━━━━━━━━━━┛", YELLOW);

            ConsoleHelper::write(boxX + 2, boxY + 1, "Press Any Key...", WHITE);

            // 결과를 화면에 렌더링하고 종료 대기
            ConsoleHelper::render();

            while (_kbhit()) _getch();
            _getch();
            break; // 메인 화면으로 복귀
        }

        // ==========================================
        // [Step 3] 키 입력 처리
        // ==========================================
        if (p1_move_timer > 0) p1_move_timer--;
        if (p2_move_timer > 0) p2_move_timer--;

        // --- [Player 1 : WASD + Space] ---
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

        // --- [Player 2 : 방향키 + Enter] ---
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
        // [Step 4] 로직 업데이트 & 그리기(Write)
        // ==========================================
        player1.updateLogic();
        player1.draw(); // 메모리에 그림

        player2.updateLogic();
        player2.draw(); // 메모리에 그림

        // 공격 시스템 적용
        int p1_attack = player1.getLinesCleared();
        int p2_attack = player2.getLinesCleared();
        if (p1_attack >= 2) player2.addGarbageLines(p1_attack - 1);
        if (p2_attack >= 2) player1.addGarbageLines(p2_attack - 1);

        // VS 로고 출력
        ConsoleHelper::write(35, 10, "VS", YELLOW);

        // ==========================================
        // [Step 5] 최종 렌더링 (화면 출력)
        // ==========================================
        ConsoleHelper::render();

        Sleep(20);
    }
}