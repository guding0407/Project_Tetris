#pragma once
#include "GameMode.h" // 부모 헤더 포함
#include "Common.h"

// GameMode를 상속받음 -> 다형성 구현
class BattleTetris : public GameMode {
private:
    bool isWaterMode;   // 모드 저장용 변수
    void showControls();

public:
    // 생성자에서 모드 설정(waterMode)을 받음
    BattleTetris(bool waterMode);   
    // 부모의 가상 함수 오버라이딩(다형성)
    void run() override;            
};

