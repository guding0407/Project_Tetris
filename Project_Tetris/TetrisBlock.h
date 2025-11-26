#pragma once

class TetrisBlock {
private:
    // 7종류, 4회전, 4x4 크기의 블럭 데이터
    static char shapes[7][4][4][4];

public:
    // 해당 shape, angle의 (r,c) 위치에 블럭이 있는지 반환 (1 or 0)
    static int getShape(int shape, int angle, int r, int c);
    
    // 블럭 종류별 색상 반환
    static int getColor(int shape);
};