#include "TetrisBlock.h"
#include "Common.h"

// [핵심 변경] 블록 모양을 16비트 정수로 정의 (비트마스킹)
// 16개의 비트가 4x4 그리드를 표현합니다. (상단 행부터 순서대로)
// 예: 
// 0100 (4)
// 1110 (E)
// 0000 (0)
// 0000 (0) -> 0x4E00
unsigned short TetrisBlock::shapes[7][4] = {
    // 0: 막대 (I)
    { 0x4444, 0x0F00, 0x4444, 0x0F00 },
    // 1: 네모 (O)
    { 0xCC00, 0xCC00, 0xCC00, 0xCC00 },
    // 2: ㅓ (T)
    { 0x4E00, 0x4640, 0x0E40, 0x4C40 },
    // 3: ㄱ (J) - 원본 데이터 기준 변환
    { 0xC440, 0x2E00, 0x4460, 0x0E80 },
    // 4: ㄴ (L)
    { 0x44C0, 0x8E00, 0x6440, 0x0E20 },
    // 5: Z (Key_Z)
    { 0xC600, 0x4C80, 0xC600, 0x4C80 },
    // 6: S (Key_S)
    { 0x6C00, 0x8C40, 0x6C00, 0x8C40 }
};

int TetrisBlock::getShape(int shape, int angle, int r, int c) {
    // 비트마스크에서 해당 위치(r, c)의 비트가 1인지 확인
    // 4x4 행렬에서 (r, c)는 (r * 4 + c)번째 비트에 해당합니다.
    // 16비트 정수에서 최상위 비트(MSB)부터 0행0열이므로 shift 계산이 필요합니다.

    unsigned short mask = shapes[shape][angle];
    int shift = 15 - (r * 4 + c); // 왼쪽으로 밀어서 해당 비트 위치 찾기

    if (shift < 0) return 0; // 안전장치
    return (mask >> shift) & 1;
}

unsigned short TetrisBlock::getShapeMask(int shape, int angle) {
    return shapes[shape][angle];
}

int TetrisBlock::getColor(int shape) {
    // (기존 코드와 동일)
    switch (shape) {
    case 0: return RED;
    case 1: return BLUE;
    case 2: return SKY_BLUE;
    case 3: return WHITE;
    case 4: return YELLOW;
    case 5: return VOILET;
    case 6: return GREEN;
    default: return WHITE;
    }
}