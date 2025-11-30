#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

// 키보드 값 상수
#define KEY_LEFT    0x4b
#define KEY_RIGHT   0x4d
#define KEY_UP      0x48
#define KEY_DOWN    0x50
#define KEY_SPACE   32
#define KEY_ENTER   13
#define KEY_ESC     27

// 색상 열거형
enum Color {
    BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE,
    DARK_RED, DARK_VOILET, DARK_YELLOW, GRAY,
    DARK_GRAY, BLUE, GREEN, SKY_BLUE,
    RED, VOILET, YELLOW, WHITE,

    // [Design] 물 색상 (파랑)
    WATER_COLOR = BLUE
};

// 블럭 상태 상수
#define EMPTY_BLOCK 0
#define WALL_BLOCK 1
#define WATER_BLOCK 9 

// 난이도 데이터
struct Stage {
    int speed;
    int stick_rate;
    int clear_line;
    int water_speed;
};