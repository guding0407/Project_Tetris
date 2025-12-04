#include "GameMode.h" // [중요] 부모 클래스 헤더 포함
#include "TetrisGame.h"
#include "BattleTetris.h"
#include "ConsoleHelper.h"
#include "TetrisBlock.h" 
#include "Common.h"
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

// 메인 화면 애니메이션 효과 클래스
class MenuEffect {
public:
    static void drawBlockAt(int x, int y, int shape, int angle) {
        ConsoleHelper::setColor(TetrisBlock::getColor(shape));
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (TetrisBlock::getShape(shape, angle, r, c)) {
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
            std::cout << "        ";
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

    ConsoleHelper::init();
    ConsoleHelper::setCursorVisible(false);
    srand((unsigned)time(NULL));

    while (true) {
        system("cls");

        // --- 로고 그리기 ---
        ConsoleHelper::setColor(SKY_BLUE);
        ConsoleHelper::setCursorPosition(25, 5);  std::cout << "====================================";
        ConsoleHelper::setCursorPosition(25, 6);  std::cout << "       TETRIS PROJECT LAUNCHER      ";
        ConsoleHelper::setCursorPosition(25, 7);  std::cout << "====================================";

        // --- 메뉴 그리기 ---
        ConsoleHelper::setColor(WHITE);
        ConsoleHelper::setCursorPosition(25, 10); std::cout << "[1] SINGLE PLAYER (CLASSIC)";
        ConsoleHelper::setCursorPosition(25, 11); std::cout << "[2] SINGLE PLAYER (SEA LEVEL MODE)";
        ConsoleHelper::setCursorPosition(25, 12); std::cout << "[3] 2-PLAYER BATTLE (CLASSIC)";
        ConsoleHelper::setCursorPosition(25, 13); std::cout << "[4] 2-PLAYER BATTLE (SEA LEVEL MODE)";
        ConsoleHelper::setCursorPosition(25, 14); std::cout << "[5] EXIT";

        ConsoleHelper::setColor(GRAY);
        ConsoleHelper::setCursorPosition(25, 27); std::cout << "Use Number Keys (1-5) to Select.";

        // --- 입력 대기 ---
        char choice = MenuEffect::waitForInputWithAnimation();

        // [핵심] 다형성(Polymorphism) 적용
        // 부모 클래스(GameMode) 포인터로 자식 객체를 가리킴
        GameMode* game = nullptr;

        if (choice == '1') {
            game = new TetrisGame(false);
        }
        else if (choice == '2') {
            game = new TetrisGame(true);
        }
        else if (choice == '3') {
            game = new BattleTetris(false);
        }
        else if (choice == '4') {
            game = new BattleTetris(true);
        }
        else if (choice == '5' || choice == KEY_ESC) {
            ConsoleHelper::setCursorPosition(25, 19);
            ConsoleHelper::setColor(RED);
            std::cout << "Exiting Game...";
            Sleep(1000);
            break;
        }

        // [핵심] 다형성을 통해 실행
        // game이 TetrisGame인지 BattleTetris인지 몰라도 run() 하나로 실행됨
        if (game != nullptr) {
            game->run();

            // 사용이 끝난 객체는 메모리 해제
            delete game;
            game = nullptr;
        }
    }

    return 0;
}