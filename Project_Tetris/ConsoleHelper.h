#pragma once
#include "Common.h"
#include <string>

// 화면 크기 정의 (충분히 크게 잡음)
#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 40

struct Pixel {
    std::string content; // "■", "  ", "A" 등
    int color;
    bool assigned; // 해당 위치에 값이 쓰였는지 여부
};

class ConsoleHelper {
private:
    // 앞면(실제 화면), 뒷면(메모리) 버퍼
    static Pixel frontBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
    static Pixel backBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

public:
    // 초기화
    static void init();

    // 화면 지우기 (버퍼만 비움)
    static void clearBuffer();

    // 메모리에 그리기 (x, y는 2칸 단위가 아닌 1칸 단위 좌표)
    static void write(int x, int y, std::string str, int color = WHITE);
    static void writeInt(int x, int y, int num, int color = WHITE);

    // 실제 콘솔에 출력 (변경된 부분만)
    static void render();

    // 기존 호환성 유지용 (커서 제어)
    static void setCursorVisible(bool visible);
    static void setCursorPosition(int x, int y);
    static void setColor(int color);
};