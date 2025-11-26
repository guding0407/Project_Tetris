#pragma once
#include <windows.h>

// 키보드 값 상수 정의
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

class Tetris {
private:
    // --- 멤버 변수 (게임 상태 데이터) ---
    int level;
    int score;
    int lines;
    int hud_drawn;
    int ab_x, ab_y; // 보드 출력 절대 좌표

    int block_shape;
    int block_angle;
    int block_x;
    int block_y;
    int next_block_shape;

    char total_block[21][14];   // 게임 보드 전체 상태
    Stage stage_data[10];       // 스테이지 정보

    // 블럭 모양 데이터 (7종류, 4회전, 4x4)
    // static const로 선언하여 클래스 차원에서 공유
    static char block_model[7][4][4][4];

    // --- 내부 헬퍼 함수 (로직) ---
    void gotoxy(int x, int y);
    void setColor(int color);
    void initGame();

    void showCurrentBlock(int shape, int angle, int x, int y);
    void eraseCurrentBlock(int shape, int angle, int x, int y);
    void showTotalBlock();
    void showNextBlock(int shape);
    void showGameStat();
    void showGameOver();
    void showLogo();

    int makeNewBlock();
    int checkCollision(int shape, int angle, int x, int y); // strike_check 대체
    void mergeBlock(int shape, int angle, int x, int y);
    void checkFullLine();

    // 블럭 이동/회전 처리 (반환값: 0=진행, 1=게임오버, 2=착지)
    int processBlockMove(int key);
    int dropBlock(); // 한 칸 아래로 자동 이동

public:
    // --- 생성자 및 실행 함수 ---
    Tetris();   // 생성자 (초기화)
    void run(); // 게임 메인 루프 실행
    void inputStartLevel(); // 시작 레벨 입력
};