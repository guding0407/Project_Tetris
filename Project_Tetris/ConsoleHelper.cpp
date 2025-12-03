#include "ConsoleHelper.h"
#include <cstdio> // sprintf

Pixel ConsoleHelper::frontBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
Pixel ConsoleHelper::backBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void ConsoleHelper::init() {
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
            // [중요] 반드시 true로 설정하여 공백으로 덮어쓰게 함
            backBuffer[y][x] = { " ", BLACK, true };
        }
    }
}

// [핵심] 2바이트 문자("■")가 잘리지 않도록 처리하는 로직 추가
void ConsoleHelper::write(int x, int y, std::string str, int color) {
    if (y < 0 || y >= SCREEN_HEIGHT) return;

    int currentX = x;
    for (size_t i = 0; i < str.length(); ++i) {
        if (currentX >= SCREEN_WIDTH) break;

        // 특수문자/한글 확인 (상위 비트가 1이면 멀티바이트 문자)
        if (str[i] & 0x80) {
            if (i + 1 < str.length()) {
                std::string mbChar = str.substr(i, 2);

                // 현재 칸에 기록
                backBuffer[y][currentX].content = mbChar;
                backBuffer[y][currentX].color = color;
                backBuffer[y][currentX].assigned = true;

                // [중요] 특수문자는 2칸을 차지하므로, 다음 칸은 '건너뜀(__SKIP__)' 처리
                if (currentX + 1 < SCREEN_WIDTH) {
                    backBuffer[y][currentX + 1].content = "__SKIP__";
                    backBuffer[y][currentX + 1].color = color;
                    backBuffer[y][currentX + 1].assigned = true;
                }

                i++; // 2바이트 썼으니 인덱스 추가 증가
                currentX += 2; // 커서 2칸 이동
            }
        }
        else {
            // 일반 문자 (숫자, 알파벳, 공백)
            backBuffer[y][currentX].content = std::string(1, str[i]);
            backBuffer[y][currentX].color = color;
            backBuffer[y][currentX].assigned = true;
            currentX++;
        }
    }
}

void ConsoleHelper::writeInt(int x, int y, int num, int color) {
    char buf[32];
    sprintf_s(buf, "%d", num);
    write(x, y, buf, color);
}

void ConsoleHelper::render() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // [수정] 높이 루프를 SCREEN_HEIGHT - 1 까지만 돌려서
    // 맨 마지막 줄에 글자를 써서 발생하는 자동 스크롤을 원천 차단합니다.
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
                COORD pos = { (SHORT)x, (SHORT)y };
                SetConsoleCursorPosition(hConsole, pos);
                SetConsoleTextAttribute(hConsole, backBuffer[y][x].color);
                printf("%s", backBuffer[y][x].content.c_str());

                frontBuffer[y][x] = backBuffer[y][x];
            }
        }
    }
}

// 호환성 함수들
void ConsoleHelper::setCursorVisible(bool visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void ConsoleHelper::setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos; pos.Y = y; pos.X = x;
    SetConsoleCursorPosition(hConsole, pos);
}
void ConsoleHelper::setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}