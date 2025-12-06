#pragma once
#include "GameMode.h" // 부모 헤더 포함
#include "TetrisCore.h"

// GameMode를 상속받음
class TetrisGame : public GameMode {
private:
    bool isWaterMode;
    int startLevel;

    void showLogo();
    void inputData();

public:
    TetrisGame(bool waterMode);

    // 부모의 가상 함수 오버라이딩(다형성)
    void run() override;
};