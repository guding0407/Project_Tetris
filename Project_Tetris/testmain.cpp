#include "TetrisGame.h"
#include "BattleTetris.h"
#include "ConsoleHelper.h"
#include "TetrisBlock.h" 
#include "Common.h"
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <iostream> // cout 사용

// 메인 화면 애니메이션 효과 클래스
class MenuEffect {
public:
    static void drawBlockAt(int x, int y, int shape, int angle) {
        ConsoleHelper::setColor(TetrisBlock::getColor(shape));
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (TetrisBlock::getShape(shape, angle, r, c)) {
                    // [변경] printf -> cout
                    ConsoleHelper::setCursorPosition((x + c) * 2, y + r);
                    std::cout << "■";
                }
            }
        }
    }

    static void clearBlockArea(int x, int y) {
        ConsoleHelper::setColor(BLACK);
        for (int r = 0; r < 4; r++) {
            ConsoleHelper::setCursorPosition(x * 2, y + r);
            std::cout << "        "; // 공백 8칸
        }
    }

    static char waitForInputWithAnimation() {
        int timer = 0;
        int animX[4] = { 8, 16, 24, 32 };
        int animY = 22;

        while (_kbhit()) _getch();

        while (true) {
            if (_kbhit()) {
                return _getch();
            }

            if (timer % 60 == 0) {
                for (int i = 0; i < 4; i++) {
                    clearBlockArea(animX[i], animY);
                    int shape = rand() % 7;
                    int angle = rand() % 4;
                    drawBlockAt(animX[i], animY, shape, angle);
                }

                if ((timer / 30) % 2 == 0) {
                    ConsoleHelper::setColor(WHITE);
                    ConsoleHelper::setCursorPosition(25, 17); std::cout << ">> SELECT OPTION: ";
                }
                else {
                    ConsoleHelper::setColor(GRAY);
                    ConsoleHelper::setCursorPosition(25, 17); std::cout << ">> SELECT OPTION: ";
                }

                // [중요] cout 버퍼 비우기 (안하면 출력 밀림)
                std::cout << std::flush;
            }

            Sleep(10);
            timer++;
        }
    }
};

int main() {
    system("mode con:cols=120 lines=40");
    system("title Tetris Project");

    // 초기화
    ConsoleHelper::init(); // 여기서 sync_with_stdio(false)가 호출됨
    ConsoleHelper::setCursorVisible(false);
    srand((unsigned)time(NULL));

    while (true) {
        system("cls");

        // --- 로고 그리기 (cout 사용) ---
        ConsoleHelper::setColor(SKY_BLUE);

        ConsoleHelper::setCursorPosition(25, 5);  printf("====================================");
        ConsoleHelper::setCursorPosition(25, 6);  printf("       TETRIS PROJECT LAUNCHER      ");
        ConsoleHelper::setCursorPosition(25, 7);  printf("====================================");


        // --- 메뉴 그리기 ---
        ConsoleHelper::setColor(WHITE);
        ConsoleHelper::setCursorPosition(25, 10); std::cout << "[1] SINGLE PLAYER (CLASSIC)";
        ConsoleHelper::setCursorPosition(25, 11); std::cout << "[2] SINGLE PLAYER (SEA LEVEL MODE)";
        ConsoleHelper::setCursorPosition(25, 12); std::cout << "[3] 2-PLAYER BATTLE (CLASSIC)";
        ConsoleHelper::setCursorPosition(25, 13); std::cout << "[4] 2-PLAYER BATTLE (SEA LEVEL MODE)";
        ConsoleHelper::setCursorPosition(25, 14); std::cout << "[5] EXIT";

        ConsoleHelper::setColor(GRAY);
        ConsoleHelper::setCursorPosition(25, 27); std::cout << "Use Number Keys (1-5) to Select.";

        // --- 애니메이션 재생 ---
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
            std::cout << "Exiting Game...";
            Sleep(1000);
            break;
        }
    }

    return 0;
}