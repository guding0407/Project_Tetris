#pragma once
#include "TetrisCore.h" // TetrisCore를 포함해야 합니다.

class TetrisGame {
private:
    bool isWaterMode;
    int startLevel;

    void showLogo();
    void inputData();

public:
    TetrisGame(bool waterMode);
    void run();
};