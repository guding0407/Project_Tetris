#pragma once

// [상속/다형성] 부모 클래스 (추상 클래스)
class GameMode {
public:
    virtual ~GameMode() {} // 가상 소멸자 (필수)

    // 순수 가상 함수: 자식들이 반드시 구현해야 함
    virtual void run() = 0;
};