#include "ConsoleHelper.h"

Pixel ConsoleHelper::frontBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
Pixel ConsoleHelper::backBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void ConsoleHelper::init() {
    // C++ 입출력 속도 향상 (printf와 동기화 끔)
    std::ios::sync_with_stdio(false);
    std::cout.tie(nullptr);

    setCursorVisible(false);
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            frontBuffer[y][x] = { " ", 7, false };
            backBuffer[y][x] = { " ", 7, false };
        }
    }
}

void ConsoleHelper::clearBuffer() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            backBuffer[y][x] = { " ", BLACK, true };
        }
    }
}

void ConsoleHelper::write(int x, int y, std::string str, int color) {
    if (y < 0 || y >= SCREEN_HEIGHT) return;

    int currentX = x;
    for (size_t i = 0; i < str.length(); ++i) {
        if (currentX >= SCREEN_WIDTH) break;

        if (str[i] & 0x80) {
            if (i + 1 < str.length()) {
                std::string mbChar = str.substr(i, 2);
                backBuffer[y][currentX].content = mbChar;
                backBuffer[y][currentX].color = color;
                backBuffer[y][currentX].assigned = true;

                if (currentX + 1 < SCREEN_WIDTH) {
                    backBuffer[y][currentX + 1].content = "__SKIP__";
                    backBuffer[y][currentX + 1].color = color;
                    backBuffer[y][currentX + 1].assigned = true;
                }
                i++;
                currentX += 2;
            }
        }
        else {
            backBuffer[y][currentX].content = std::string(1, str[i]);
            backBuffer[y][currentX].color = color;
            backBuffer[y][currentX].assigned = true;
            currentX++;
        }
    }
}

void ConsoleHelper::writeInt(int x, int y, int num, int color) {
    // sprintf 대신 std::to_string 사용
    write(x, y, std::to_string(num), color);
}

void ConsoleHelper::render() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int y = 0; y < SCREEN_HEIGHT - 1; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (backBuffer[y][x].content == "__SKIP__") {
                frontBuffer[y][x] = backBuffer[y][x];
                continue;
            }

            if (backBuffer[y][x].assigned &&
                (backBuffer[y][x].content != frontBuffer[y][x].content ||
                    backBuffer[y][x].color != frontBuffer[y][x].color))
            {
                COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };   // static_cast
                SetConsoleCursorPosition(hConsole, pos);
                SetConsoleTextAttribute(hConsole, backBuffer[y][x].color);

                std::cout << backBuffer[y][x].content;

                frontBuffer[y][x] = backBuffer[y][x];
            }
        }
    }
    // 버퍼 강제 비우기 (cout은 버퍼링되므로 필수)
    std::cout << std::flush;
}

void ConsoleHelper::setCursorVisible(bool visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void ConsoleHelper::setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };   // static_cast
    SetConsoleCursorPosition(hConsole, pos);
}

void ConsoleHelper::setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ConsoleHelper::recover() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            backBuffer[y][x] = frontBuffer[y][x];
            backBuffer[y][x].assigned = true;
        }
    }
}