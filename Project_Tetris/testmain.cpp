#include "TetrisGame.h"
#include "BattleTetris.h"
#include "ConsoleHelper.h"
#include "TetrisBlock.h" 
#include "Common.h"      // [중요] KEY_ESC, 구체적인 키 코드, 표준 라이브러리를 위해 필수
#include <conio.h>       // _kbhit, _getch
#include <windows.h>     // Sleep
#include <time.h>        // time
#include <stdlib.h>      // rand, srand
#include <stdio.h>       // printf

// [추가] 메인 화면 애니메이션 효과를 담당하는 클래스
class MenuEffect {
public:
    // 특정 위치에 블록 하나를 그리는 함수
    static void drawBlockAt(int x, int y, int shape, int angle) {
        ConsoleHelper::setColor(TetrisBlock::getColor(shape));
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (TetrisBlock::getShape(shape, angle, r, c)) {
                    // x좌표에 *2를 하여 가로 비율 맞춤
                    ConsoleHelper::setCursorPosition((x + c) * 2, y + r);
                    printf("■");
                }
            }
        }
    }

    // 블록이 그려질 영역을 지우는 함수
    static void clearBlockArea(int x, int y) {
        ConsoleHelper::setColor(BLACK);
        for (int r = 0; r < 4; r++) {
            ConsoleHelper::setCursorPosition(x * 2, y + r);
            printf("        "); // 공백 8칸
        }
    }

    // 애니메이션을 보여주며 키 입력을 기다리는 함수
    static char waitForInputWithAnimation() {
        int timer = 0;

        // 애니메이션 좌표 (화면 하단)
        int animX[4] = { 8, 16, 24, 32 }; // 블록 4개의 X 좌표 (Grid 기준)
        int animY = 22;                    // 블록 Y 좌표

        // 키 버퍼 비우기 (입력 밀림 방지)
        while (_kbhit()) _getch();

        while (true) {
            // 1. 키 입력 확인 (입력이 있으면 즉시 리턴)
            if (_kbhit()) {
                return _getch();
            }

            // 2. 애니메이션 업데이트 (일정 시간마다)
            if (timer % 60 == 0) { // 속도 조절
                for (int i = 0; i < 4; i++) {
                    // 기존 블록 지우기
                    clearBlockArea(animX[i], animY);

                    // 랜덤 모양 및 각도
                    int shape = rand() % 7;
                    int angle = rand() % 4;

                    // 새 블록 그리기
                    drawBlockAt(animX[i], animY, shape, angle);
                }

                // 안내 문구 깜빡임 효과
                if ((timer / 30) % 2 == 0) {
                    ConsoleHelper::setColor(WHITE);
                    ConsoleHelper::setCursorPosition(25, 17); cout << ">> SELECT OPTION: " << endl;
                }
                else {
                    ConsoleHelper::setColor(GRAY);
                    ConsoleHelper::setCursorPosition(25, 17); cout << ">> SELECT OPTION: " << endl;
                }
            }

            Sleep(10); // CPU 과부하 방지
            timer++;
        }
    }
};

int main() {

    // 콘솔 창 크기 설정
    system("mode con:cols=100 lines=35");


    system("mode con:cols=120 lines=40");

    system("title Tetris Project");

    // 커서 숨기기
    ConsoleHelper::setCursorVisible(false);

    // 랜덤 시드 초기화
    srand((unsigned)time(NULL));

    while (true) {

        system("cls");

        // --- 로고 그리기 ---
        ConsoleHelper::setColor(SKY_BLUE);
        ConsoleHelper::setCursorPosition(20, 5);  printf("===================================");
        ConsoleHelper::setCursorPosition(20, 6);  printf("       TETRIS PROJECT LAUNCHER      ");
        ConsoleHelper::setCursorPosition(20, 7);  printf("===================================");

        // --- 메뉴 그리기 ---
        ConsoleHelper::setColor(WHITE);
        ConsoleHelper::setCursorPosition(25, 10); printf("[1] SINGLE PLAYER (CLASSIC)");
        ConsoleHelper::setCursorPosition(25, 11); printf("[2] SINGLE PLAYER (SEA LEVEL MODE)");
        ConsoleHelper::setCursorPosition(25, 12); printf("[3] 2-PLAYER BATTLE (CLASSIC)");
        ConsoleHelper::setCursorPosition(25, 13); printf("[4] 2-PLAYER BATTLE (SEA LEVEL MODE)");
        ConsoleHelper::setCursorPosition(25, 14); printf("[5] EXIT");

        ConsoleHelper::setColor(GRAY);
        ConsoleHelper::setCursorPosition(25, 27); printf("Use Number Keys (1-5) to Select.");

        // --- 애니메이션 재생 및 입력 대기 ---
        char choice = MenuEffect::waitForInputWithAnimation();

        // --- 선택 처리 ---
        if (choice == '1') {
            TetrisGame game(false);
            game.run();
        }
        else if (choice == '2') {
            TetrisGame game(true);
            game.run();
        }
        else if (choice == '3') {
            BattleTetris battle;
            battle.run(false);
        }
        else if (choice == '4') {
            BattleTetris battle;
            battle.run(true);
        }
        else if (choice == '5' || choice == KEY_ESC) {
            ConsoleHelper::setCursorPosition(25, 19);
            ConsoleHelper::setColor(RED);
            printf("Exiting Game...");
            Sleep(1000);
            break;
        }
    }

    return 0;
}