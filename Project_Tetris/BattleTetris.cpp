#include "BattleTetris.h"
#include "TetrisCore.h"
#include "ConsoleHelper.h"

// 2인용 키 설정
#define P1_UP    'w'
#define P1_DOWN  's'
#define P1_LEFT  'a'
#define P1_RIGHT 'd'
#define P1_DROP  KEY_SPACE  // 스페이스바

#define P2_UP    KEY_UP
#define P2_DOWN  KEY_DOWN
#define P2_LEFT  KEY_LEFT
#define P2_RIGHT KEY_RIGHT
#define P2_DROP  KEY_ENTER  // 엔터키

// BattleTetris.cpp

void BattleTetris::run(bool waterMode) {
    system("cls");
    ConsoleHelper::setCursorVisible(false);

    TetrisCore player1(4, 2, waterMode);
    TetrisCore player2(50, 2, waterMode);

    player1.initGame(0);
    player2.initGame(0);

    ConsoleHelper::setCursorPosition(35, 10);
    ConsoleHelper::setColor(YELLOW);
    printf("VS");

    // [추가] 키가 계속 눌려있는지 확인하는 변수들 (중복 입력 방지용)
    bool p1_rotate_pressed = false; // P1 회전(W)
    bool p1_drop_pressed = false;   // P1 하드드롭(Space)
    bool p2_rotate_pressed = false; // P2 회전(Up)
    bool p2_drop_pressed = false;   // P2 하드드롭(Enter)

    while (true) {
        // 1. 게임 오버 체크
        if (player1.isGameOver()) {
            ConsoleHelper::setCursorPosition(35, 12); printf("P2 WIN!"); break;
        }
        if (player2.isGameOver()) {
            ConsoleHelper::setCursorPosition(35, 12); printf("P1 WIN!"); break;
        }

        // ======================= [플레이어 1 입력 처리] =======================

        // 1) 회전 (W) - 한 번만 눌리게 처리
        if (GetAsyncKeyState(0x57) & 0x8000) { // W키 누름
            if (!p1_rotate_pressed) {
                player1.handleInput(KEY_UP);
                p1_rotate_pressed = true; // 눌림 상태로 잠금
            }
        }
        else {
            p1_rotate_pressed = false; // 키를 떼면 잠금 해제
        }

        // 2) 하드 드롭 (Space) - 한 번만 눌리게 처리
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (!p1_drop_pressed) {
                player1.handleInput(KEY_SPACE);
                p1_drop_pressed = true;
            }
        }
        else {
            p1_drop_pressed = false;
        }

        // 3) 이동 (A, S, D) - 꾹 누르면 계속 이동해도 됨 (단, 너무 빠르면 여기도 딜레이 필요)
        if (GetAsyncKeyState(0x53) & 0x8000) player1.handleInput(KEY_DOWN);  // S
        if (GetAsyncKeyState(0x41) & 0x8000) player1.handleInput(KEY_LEFT);  // A
        if (GetAsyncKeyState(0x44) & 0x8000) player1.handleInput(KEY_RIGHT); // D


        // ======================= [플레이어 2 입력 처리] =======================

        // 1) 회전 (UP) - 한 번만 눌리게 처리
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (!p2_rotate_pressed) {
                player2.handleInput(KEY_UP);
                p2_rotate_pressed = true;
            }
        }
        else {
            p2_rotate_pressed = false;
        }

        // 2) 하드 드롭 (Enter) - 한 번만 눌리게 처리
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            if (!p2_drop_pressed) {
                player2.handleInput(KEY_SPACE); // Enter를 누르면 Space 기능(하드드롭) 실행
                p2_drop_pressed = true;
            }
        }
        else {
            p2_drop_pressed = false;
        }

        // 3) 이동 (Left, Right, Down)
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)  player2.handleInput(KEY_DOWN);
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)  player2.handleInput(KEY_LEFT);
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) player2.handleInput(KEY_RIGHT);

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break; // ESC 종료


        // 3. 게임 로직 업데이트
        player1.updateLogic();
        player2.updateLogic();

        // 4. 공격 시스템
        int p1_attack = player1.getLinesCleared();
        int p2_attack = player2.getLinesCleared();

        if (p1_attack >= 2) player2.addGarbageLines(p1_attack - 1);
        if (p2_attack >= 2) player1.addGarbageLines(p2_attack - 1);

        // 5. 속도 조절
        // Sleep이 너무 짧으면 이동이 너무 빠르고, 길면 반응이 답답할 수 있습니다.
        // 33ms는 약 30프레임입니다.
        Sleep(10);
    }

    _getch();
}