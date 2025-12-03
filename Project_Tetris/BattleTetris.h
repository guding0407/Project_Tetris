#pragma once
#include "Common.h"

// 2인용 모드 클래스
class BattleTetris {
public:
    void showControls();
    // [수정 전] void run();
    // [수정 후] 인자(waterMode)를 받을 수 있도록 변경
    void run(bool waterMode = false);
};