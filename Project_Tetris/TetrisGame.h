#pragma once
#include "GameMode.h" // [중요] 부모 헤더 포함
#include "TetrisCore.h"

// [상속] GameMode를 상속받음
class TetrisGame : public GameMode {
private:
    bool isWaterMode;
    int startLevel;

    void showLogo();
    void inputData();

public:
    TetrisGame(bool waterMode);

    // [다형성] 부모의 가상 함수 오버라이딩
    void run() override;
};