#include "MenuEffect.h"


void MenuEffect::drawBlockAt(int x, int y, int shape, int angle) {
    ConsoleHelper::setColor(TetrisBlock::getColor(shape));
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (TetrisBlock::getShape(shape, angle, r, c)) {
                ConsoleHelper::setCursorPosition((x + c) * 2, y + r);
                std::cout << "бс";
            }
        }
    }
}

void MenuEffect::clearBlockArea(int x, int y) {
    ConsoleHelper::setColor(BLACK);
    for (int r = 0; r < 4; r++) {
        ConsoleHelper::setCursorPosition(x * 2, y + r);
        std::cout << "        ";
    }
}

char MenuEffect::waitForInputWithAnimation() {
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