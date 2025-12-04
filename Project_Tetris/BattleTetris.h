#pragma once
#include "GameMode.h" // [중요] 부모 헤더 포함
#include "Common.h"

// [상속] GameMode를 상속받음 -> 다형성 구현 가능
class BattleTetris : public GameMode {
private:
    bool isWaterMode; // 모드 저장용 변수
    void showControls();

public:
    // [수정] 생성자에서 모드 설정(waterMode)을 받음
    BattleTetris(bool waterMode);

    // [수정] 인자 없는 run() (부모와 형태 통일)
    void run() override;
};