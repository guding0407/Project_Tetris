#pragma once
#include "ConsoleHelper.h"
#include "TetrisBlock.h"

class MenuEffect {
public:
    static void drawBlockAt(int x, int y, int shape, int angle);
    static void clearBlockArea(int x, int y);
    static char waitForInputWithAnimation();
};