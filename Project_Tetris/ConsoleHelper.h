#pragma once
#include "Common.h"
#include <string>

// [Rule 2] constexpr 적용
constexpr int SCREEN_WIDTH = 120;
constexpr int SCREEN_HEIGHT = 40;

struct Pixel {
    std::string content;
    int color;
    bool assigned;
};

class ConsoleHelper {
private:
    static Pixel frontBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
    static Pixel backBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

public:
    static void init();
    static void clearBuffer();
    static void write(int x, int y, std::string str, int color = WHITE);
    static void writeInt(int x, int y, int num, int color = WHITE);

    // [Rule 7] printf 대신 cout을 사용하는 렌더링 함수
    static void render();

    static void setCursorVisible(bool visible);
    static void setCursorPosition(int x, int y);
    static void setColor(int color);
    static void recover();
};