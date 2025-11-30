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

void BattleTetris::run(bool waterMode) {
    system("cls");
    ConsoleHelper::setCursorVisible(false);

    // [수정] 생성자에 waterMode 변수 전달
    // Player 1: (4, 2) 위치
    TetrisCore player1(4, 2, waterMode);
    // Player 2: (50, 2) 위치
    TetrisCore player2(50, 2, waterMode);

    player1.initGame();
    player2.initGame();

    // 중간 장식
    ConsoleHelper::setCursorPosition(35, 10);
    ConsoleHelper::setColor(YELLOW);
    printf("VS");

    while (true) {
        // 1. 게임 오버 체크
        if (player1.isGameOver()) {
            ConsoleHelper::setCursorPosition(35, 12);
            printf("P2 WIN!");
            break;
        }
        if (player2.isGameOver()) {
            ConsoleHelper::setCursorPosition(35, 12);
            printf("P1 WIN!");
            break;
        }

        // 2. 키 입력 처리 (동시 입력을 위해 GetAsyncKeyState 사용)
        // Player 1 (WASD)
        if (GetAsyncKeyState(0x57) & 0x8000) player1.handleInput(KEY_UP);    // W
        if (GetAsyncKeyState(0x53) & 0x8000) player1.handleInput(KEY_DOWN);  // S
        if (GetAsyncKeyState(0x41) & 0x8000) player1.handleInput(KEY_LEFT);  // A
        if (GetAsyncKeyState(0x44) & 0x8000) player1.handleInput(KEY_RIGHT); // D
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) player1.handleInput(KEY_SPACE);

        // Player 2 (방향키)
        if (GetAsyncKeyState(VK_UP) & 0x8000)    player2.handleInput(KEY_UP);
        if (GetAsyncKeyState(VK_DOWN) & 0x8000)  player2.handleInput(KEY_DOWN);
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)  player2.handleInput(KEY_LEFT);
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) player2.handleInput(KEY_RIGHT);
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) player2.handleInput(KEY_SPACE); // 엔터키를 스페이스 기능(Hard Drop)으로

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break; // ESC 종료

        // 3. 게임 로직 업데이트 (낙하)
        player1.updateLogic();
        player2.updateLogic();

        // 4. 공격 시스템 (라인 지우면 상대방에게 공격)
        int p1_attack = player1.getLinesCleared();
        int p2_attack = player2.getLinesCleared();

        if (p1_attack >= 2) player2.addGarbageLines(p1_attack - 1);
        if (p2_attack >= 2) player1.addGarbageLines(p2_attack - 1);

        Sleep(33); // 약 30프레임 속도 조절

        // 키 중복 입력 방지용 딜레이 (필요시 조절)
        // Sleep을 너무 길게 주면 반응이 느려지므로 미세 조정 필요
    }

    _getch(); // 결과 확인 대기
}