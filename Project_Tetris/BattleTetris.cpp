#include "BattleTetris.h"
#include "TetrisCore.h"
#include "ConsoleHelper.h"

// 키보드 가상 키 코드 (플레이어 1 - WASD)
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

void BattleTetris::run(bool waterMode) {
    system("cls");
    ConsoleHelper::setCursorVisible(false);

    // 1. 플레이어 생성
    // (x좌표, y좌표, 해수면모드, 전투모드=true)
    TetrisCore player1(4, 2, waterMode, true);
    TetrisCore player2(50, 2, waterMode, true);

    // 2. 게임 초기화 (레벨 0부터 시작)
    player1.initGame(0);
    player2.initGame(0);

    // 3. UI 장식
    ConsoleHelper::setCursorPosition(35, 10);
    ConsoleHelper::setColor(YELLOW);
    printf("VS");

    // ==========================================
    // [변수 선언] 입력 제어용
    // ==========================================

    // 회전/하드드롭 중복 입력 방지 (누르고 있어도 1번만 동작)
    bool p1_rotate_pressed = false;
    bool p1_drop_pressed = false;
    bool p2_rotate_pressed = false;
    bool p2_drop_pressed = false;

    // 이동 속도 조절용 타이머 (너무 빠른 이동 방지)
    int p1_move_timer = 0;
    int p2_move_timer = 0;

    // ★ 이동 감도 조절 (숫자가 클수록 이동이 느려짐. 2~5 추천)
    const int MOVE_SPEED = 3;

    while (true) {
        // ==========================================
        // 1. 승패 판정 및 결과 출력
        // ==========================================
        int winner = 0; // 0:진행중, 1:P1승리, 2:P2승리

        if (player1.isGameOver()) {
            winner = 2; // P1 사망 -> P2 승리
        }
        else if (player2.isGameOver()) {
            winner = 1; // P2 사망 -> P1 승리
        }

        if (winner != 0) {
            // 결과 박스 그리기
            int boxX = 32;
            int boxY = 10;

            ConsoleHelper::setColor(YELLOW);
            ConsoleHelper::setCursorPosition(boxX, boxY++); printf("┏━━━━━━━━━━━━━━━━━━┓");
            ConsoleHelper::setCursorPosition(boxX, boxY++); printf("┃                  ┃");

            ConsoleHelper::setCursorPosition(boxX, boxY);
            if (winner == 1) printf("┃   PLAYER 1 WIN!  ┃");
            else             printf("┃   PLAYER 2 WIN!  ┃");
            boxY++;

            ConsoleHelper::setCursorPosition(boxX, boxY++); printf("┃                  ┃");
            ConsoleHelper::setCursorPosition(boxX, boxY++); printf("┗━━━━━━━━━━━━━━━━━━┛");

            ConsoleHelper::setColor(WHITE);
            ConsoleHelper::setCursorPosition(boxX + 2, boxY + 1);
            printf("Press Any Key...");

            // 키 입력 대기 후 종료
            while (_kbhit()) _getch();
            _getch();
            break; // 메인 화면으로 복귀
        }

        // ==========================================
        // 2. 키 입력 처리
        // ==========================================

        // 타이머 쿨타임 감소
        if (p1_move_timer > 0) p1_move_timer--;
        if (p2_move_timer > 0) p2_move_timer--;

        // --- [Player 1 : WASD + Space] ---

        // 회전 (W) : 한 번만 눌림
        if (GetAsyncKeyState(VK_W) & 0x8000) {
            if (!p1_rotate_pressed) {
                player1.handleInput(KEY_UP);
                p1_rotate_pressed = true;
            }
        }
        else {
            p1_rotate_pressed = false;
        }

        // 하드 드롭 (Space) : 한 번만 눌림
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (!p1_drop_pressed) {
                player1.handleInput(KEY_SPACE);
                p1_drop_pressed = true;
            }
        }
        else {
            p1_drop_pressed = false;
        }

        // 이동 (A, S, D) : 타이머로 속도 조절
        if (p1_move_timer == 0) {
            bool moved = false;
            if (GetAsyncKeyState(VK_S) & 0x8000) { player1.handleInput(KEY_DOWN); moved = true; }
            else if (GetAsyncKeyState(VK_A) & 0x8000) { player1.handleInput(KEY_LEFT); moved = true; }
            else if (GetAsyncKeyState(VK_D) & 0x8000) { player1.handleInput(KEY_RIGHT); moved = true; }

            if (moved) p1_move_timer = MOVE_SPEED;
        }


        // --- [Player 2 : 방향키 + Enter] ---

        // 회전 (UP)
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (!p2_rotate_pressed) {
                player2.handleInput(KEY_UP);
                p2_rotate_pressed = true;
            }
        }
        else {
            p2_rotate_pressed = false;
        }

        // 하드 드롭 (Enter)
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            if (!p2_drop_pressed) {
                player2.handleInput(KEY_SPACE); // Enter키를 Space(하드드롭) 기능으로 맵핑
                p2_drop_pressed = true;
            }
        }
        else {
            p2_drop_pressed = false;
        }

        // 이동 (Left, Right, Down)
        if (p2_move_timer == 0) {
            bool moved = false;
            if (GetAsyncKeyState(VK_DOWN) & 0x8000) { player2.handleInput(KEY_DOWN); moved = true; }
            else if (GetAsyncKeyState(VK_LEFT) & 0x8000) { player2.handleInput(KEY_LEFT); moved = true; }
            else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { player2.handleInput(KEY_RIGHT); moved = true; }

            if (moved) p2_move_timer = MOVE_SPEED;
        }

        // 게임 강제 종료 (ESC)
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;


        // ==========================================
        // 3. 게임 로직 업데이트
        // ==========================================
        player1.updateLogic();
        player2.updateLogic();


        // ==========================================
        // 4. 공격 시스템 (2줄 이상 지우면 상대 공격)
        // ==========================================
        int p1_attack = player1.getLinesCleared();
        int p2_attack = player2.getLinesCleared();

        if (p1_attack >= 2) player2.addGarbageLines(p1_attack - 1);
        if (p2_attack >= 2) player1.addGarbageLines(p2_attack - 1);


        // ==========================================
        // 5. 전체 속도 조절
        // ==========================================
        Sleep(20);
    }

    _getch();
}