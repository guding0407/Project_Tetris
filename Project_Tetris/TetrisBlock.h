#pragma once

class TetrisBlock {
private:
    // [변경] 4x4x4 char 배열 -> 16비트 정수(unsigned short) 배열로 압축
    // 예: 0010 0010 0010 0010 (2진수) -> 0x2222 (16진수)
    static unsigned short shapes[7][4];

public:
    // (r, c) 위치에 블록이 존재하는지 비트 연산으로 확인
    static int getShape(int shape, int angle, int r, int c);

    // [추가] 성능 최적화를 위해 원본 비트마스크 값을 직접 가져오는 함수
    static unsigned short getShapeMask(int shape, int angle);

    static int getColor(int shape);
};