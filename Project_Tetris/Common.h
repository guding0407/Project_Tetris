#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

// [Rule 2] #define -> constexpr (컴파일 타임 상수)
constexpr int KEY_LEFT = 0x4b;
constexpr int KEY_RIGHT = 0x4d;
constexpr int KEY_UP = 0x48;
constexpr int KEY_DOWN = 0x50;
constexpr int KEY_SPACE = 32;
constexpr int KEY_ENTER = 13;
constexpr int KEY_ESC = 27;

// 블럭 상태 상수
constexpr int EMPTY_BLOCK = 0;
constexpr int WALL_BLOCK = 1;
constexpr int WATER_BLOCK = 9;

// 색상 열거형
enum Color {
    BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE,
    DARK_RED, DARK_VOILET, DARK_YELLOW, GRAY,
    DARK_GRAY, BLUE, GREEN, SKY_BLUE,
    RED, VOILET, YELLOW, WHITE,

    WATER_COLOR = BLUE
};

// 난이도 데이터
struct Stage {
    int speed;
    int stick_rate;
    int clear_line;
    int water_speed;
};