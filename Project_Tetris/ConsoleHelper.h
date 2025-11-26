#pragma once
#include "Common.h"

class ConsoleHelper {
public:
    static void setCursorPosition(int x, int y); // gotoxy 기능
    static void setColor(int color);             // SetColor 기능
    static void setCursorVisible(bool visible);  // 커서 숨기기용
};