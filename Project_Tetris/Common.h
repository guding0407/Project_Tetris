#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

// 키보드 값 상수
#define KEY_LEFT    0x4b
#define KEY_RIGHT   0x4d
#define KEY_UP      0x48
#define KEY_DOWN    0x50
#define KEY_SPACE   32

// 색상 열거형
enum Color {
    BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE,
    DARK_RED, DARK_VOILET, DARK_YELLOW, GRAY,
    DARK_GRAY, BLUE, GREEN, SKY_BLUE,
    RED, VOILET, YELLOW, WHITE
};

// 난이도 데이터 구조체
struct Stage {
    int speed;      // 게임 속도
    int stick_rate; // 막대기 등장 확률
    int clear_line; // 클리어 목표 줄 수
};