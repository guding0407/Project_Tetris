#include "Tetris.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

// 정적 멤버 변수 초기화 (블럭 모양 데이터)
char Tetris::block_model[7][4][4][4] = {
    // 막대
    { {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}, {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0}, {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}, {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0} },
    // 네모
    { {1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0}, {1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0}, {1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0}, {1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0} },
    // ㅓ
    { {0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0}, {1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0}, {1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0}, {0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0} },
    // ㄱ
    { {1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0}, {1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0}, {1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0}, {0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0} },
    // ㄴ
    { {1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0}, {1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0}, {0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0}, {1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0} },
    // Z
    { {1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0}, {0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0}, {1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0}, {0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0} },
    // S
    { {0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0}, {1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0}, {0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0}, {1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0} }
};

// 생성자: 게임 데이터 초기화
Tetris::Tetris() {
    ab_x = 5;
    ab_y = 1;
    level = 0;
    score = 0;
    lines = 0;
    hud_drawn = 0;

    // 스테이지 데이터 설정
    stage_data[0] = { 40, 20, 1 };
    stage_data[1] = { 38, 18, 1 };
    stage_data[2] = { 35, 18, 20 };
    stage_data[3] = { 30, 17, 20 };
    stage_data[4] = { 25, 16, 20 };
    stage_data[5] = { 20, 14, 20 };
    stage_data[6] = { 15, 14, 20 };
    stage_data[7] = { 10, 13, 20 };
    stage_data[8] = { 6, 12, 20 };
    stage_data[9] = { 4, 11, 99999 };

    srand((unsigned)time(NULL));
    initGame();
}

// 화면 좌표 이동 헬퍼
void Tetris::gotoxy(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.Y = y;
    pos.X = x;
    SetConsoleCursorPosition(hConsole, pos);
}

// 색상 변경 헬퍼
void Tetris::setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 게임판 초기화
void Tetris::initGame() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 14; j++) {
            if (j == 0 || j == 13) total_block[i][j] = 1;
            else total_block[i][j] = 0;
        }
    }
    for (int j = 0; j < 14; j++) total_block[20][j] = 1;

    // 점수 및 상태 리셋 (레벨은 유지)
    lines = 0;
    score = 0;
    hud_drawn = 0;
}

// 현재 블럭 그리기
void Tetris::showCurrentBlock(int shape, int angle, int x, int y) {
    int colorCode;
    switch (shape) {
    case 0: colorCode = RED; break;
    case 1: colorCode = BLUE; break;
    case 2: colorCode = SKY_BLUE; break;
    case 3: colorCode = WHITE; break;
    case 4: colorCode = YELLOW; break;
    case 5: colorCode = VOILET; break;
    case 6: colorCode = GREEN; break;
    default: colorCode = WHITE; break;
    }
    setColor(colorCode);

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (block_model[shape][angle][r][c] == 0) continue;
            if (y + r < 0) continue;
            gotoxy((c + x) * 2 + ab_x, y + r + ab_y);
            printf("■");
        }
    }
    setColor(BLACK);
    gotoxy(77, 23);
}

// 현재 블럭 지우기
void Tetris::eraseCurrentBlock(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (block_model[shape][angle][r][c] == 0) continue;
            if (y + r < 0) continue;
            gotoxy((c + x) * 2 + ab_x, y + r + ab_y);
            printf("  ");
        }
    }
}

// 전체 맵 그리기
void Tetris::showTotalBlock() {
    setColor(DARK_GRAY);
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 14; j++) {
            if (j == 0 || j == 13 || i == 20)
                setColor((level % 6) + 1);
            else
                setColor(DARK_GRAY);

            gotoxy((j * 2) + ab_x, i + ab_y);
            if (total_block[i][j] == 1) printf("■");
            else printf("  ");
        }
    }
    setColor(BLACK);
    gotoxy(77, 23);
}

// 다음 블럭 보여주기
void Tetris::showNextBlock(int shape) {
    setColor((level) % 6 + 1);
    for (int i = 1; i < 7; i++) {
        gotoxy(33, i);
        for (int j = 0; j < 6; j++) {
            if (i == 1 || i == 6 || j == 0 || j == 5) printf("■");
            else printf("  ");
        }
    }
    // 다음 블럭은 항상 0도 회전 상태로 미리보기
    int temp_x = 15; // 미리보기 좌표 (임의 조정)
    int temp_y = 1;

    // showCurrentBlock을 재활용하되 좌표를 미리보기 창으로
    // 원래 로직에서는 미리보기 좌표 계산이 포함되어 있었음
    // 여기서는 간단히 구현
    int colorCode;
    switch (shape) {
    case 0: colorCode = RED; break;
    case 1: colorCode = BLUE; break;
    case 2: colorCode = SKY_BLUE; break;
    case 3: colorCode = WHITE; break;
    case 4: colorCode = YELLOW; break;
    case 5: colorCode = VOILET; break;
    case 6: colorCode = GREEN; break;
    }
    setColor(colorCode);

    // 미리보기 내부 그리기
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (block_model[shape][0][r][c]) {
                gotoxy((c * 2) + 36, r + 2); // 좌표 하드코딩 (원본 로직 참조)
                printf("■");
            }
        }
    }
    setColor(BLACK);
}

// 게임 상태 표시 (점수, 레벨)
void Tetris::showGameStat() {
    setColor(GRAY);
    if (!hud_drawn) {
        gotoxy(35, 7); printf("STAGE");
        gotoxy(35, 9); printf("SCORE");
        gotoxy(35, 12); printf("LINES");
        hud_drawn = 1;
    }
    gotoxy(41, 7); printf("%d", level + 1);
    gotoxy(35, 10); printf("%10d", score);
    gotoxy(35, 13); printf("%10d", stage_data[level].clear_line - lines);
}

// 새 블럭 생성
int Tetris::makeNewBlock() {
    int i = rand() % 100;
    if (i <= stage_data[level].stick_rate) return 0; // 막대
    return (rand() % 6) + 1;
}

// 충돌 체크
int Tetris::checkCollision(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (block_model[shape][angle][r][c] == 0) continue;
            int gy = y + r;
            int gx = x + c;

            if (gx <= 0 || gx >= 13) return 1; // 벽
            if (gy < 0) continue;
            if (gy >= 21) return 1; // 바닥
            if (total_block[gy][gx]) return 1; // 다른 블럭
        }
    }
    return 0;
}

// 블럭 고정
void Tetris::mergeBlock(int shape, int angle, int x, int y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (block_model[shape][angle][r][c]) {
                int gy = y + r;
                int gx = x + c;
                if (gy >= 0 && gy <= 20 && gx >= 0 && gx <= 13)
                    total_block[gy][gx] = 1;
            }
        }
    }
    checkFullLine();
    showTotalBlock();
}

// 줄 삭제 체크
void Tetris::checkFullLine() {
    for (int i = 0; i < 20; i++) {
        int j;
        for (j = 1; j < 13; j++) {
            if (total_block[i][j] == 0) break;
        }
        if (j == 13) { // 한 줄 완성
            lines++;
            // 라인 클리어 애니메이션
            setColor(BLUE);
            gotoxy(1 * 2 + ab_x, i + ab_y);
            for (int k = 1; k < 13; k++) { printf("□"); Sleep(10); }

            // 라인 삭제 및 내리기
            for (int k = i; k > 0; k--) {
                for (int col = 1; col < 13; col++)
                    total_block[k][col] = total_block[k - 1][col];
            }
            for (int col = 1; col < 13; col++) total_block[0][col] = 0;

            score += 100 + (level * 10) + (rand() % 10);
            showGameStat();
        }
    }
}

// 게임 오버 처리
void Tetris::showGameOver() {
    setColor(RED);
    gotoxy(15, 8);  printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(15, 9);  printf("┃                          ┃");
    gotoxy(15, 10); printf("┃        GAME OVER         ┃");
    gotoxy(15, 11); printf("┃                          ┃");
    gotoxy(15, 12); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    Sleep(1000);
    _getch();
    system("cls");
}

// 로고 화면
void Tetris::showLogo() {
    gotoxy(13, 3); printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    Sleep(50); gotoxy(13, 4); printf("┃◆◆◆  ◆◆◆  ◆◆◆   ◆◆     ◆   ◆◆◆  ┃");
    Sleep(50); gotoxy(13, 5); printf("┃ ◆    ◆     ◆    ◆ ◆    ◆   ◆    ┃");
    Sleep(50); gotoxy(13, 6); printf("┃ ◆    ◆◆◆   ◆    ◆◆     ◆     ◆  ┃");
    Sleep(50); gotoxy(13, 7); printf("┃ ◆    ◆     ◆    ◆ ◆    ◆     ◆  ┃");
    Sleep(50); gotoxy(13, 8); printf("┃ ◆    ◆◆◆   ◆    ◆  ◆   ◆    ◆◆◆  ┃");
    Sleep(50); gotoxy(13, 9); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    gotoxy(28, 20); printf("Press Any Key to Start!");
    _getch();
    system("cls");
}

// 시작 레벨 입력
void Tetris::inputStartLevel() {
    setColor(GRAY);
    gotoxy(10, 7);  printf("┏━━━━━━━━━━<GAME KEY>━━━━━━━━┓");
    gotoxy(10, 8);  printf("┃ UP   : Rotate Block        ┃");
    gotoxy(10, 9);  printf("┃ DOWN : Move One-Step Down  ┃");
    gotoxy(10, 10); printf("┃ SPACE: Hard Drop           ┃");
    gotoxy(10, 11); printf("┃ LEFT : Move Left           ┃");
    gotoxy(10, 12); printf("┃ RIGHT: Move Right          ┃");
    gotoxy(10, 13); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    while (1) {
        gotoxy(10, 3);
        printf("Select Start level[1-8]:          ");
        gotoxy(35, 3);

        char buf[10];
        cin.getline(buf, 10);
        int sel = atoi(buf);

        if (sel >= 1 && sel <= 8) {
            level = sel - 1;
            break;
        }
    }
    system("cls");
}

// 블럭 자동 낙하 (타이머에 의한 호출)
int Tetris::dropBlock() {
    return processBlockMove(KEY_DOWN);
}

// 키 입력 및 이동 처리
// 리턴값: 0=정상, 1=게임오버, 2=착지 후 새블럭 필요
int Tetris::processBlockMove(int key) {
    // 1. 현재 블럭 지우기
    eraseCurrentBlock(block_shape, block_angle, block_x, block_y);

    int ret = 0; // 상태 코드

    switch (key) {
    case KEY_UP: // 회전
        if (!checkCollision(block_shape, (block_angle + 1) % 4, block_x, block_y))
            block_angle = (block_angle + 1) % 4;
        break;
    case KEY_LEFT:
        if (!checkCollision(block_shape, block_angle, block_x - 1, block_y))
            block_x--;
        break;
    case KEY_RIGHT:
        if (!checkCollision(block_shape, block_angle, block_x + 1, block_y))
            block_x++;
        break;
    case KEY_DOWN:
        if (!checkCollision(block_shape, block_angle, block_x, block_y + 1)) {
            block_y++;
        }
        else {
            // 바닥에 닿음
            // 지웠던 자리에 다시 그리기엔 늦었으므로(좌표가 변하지 않음)
            // 바로 병합 프로세스로 넘어감
            // 다만 erase를 맨 위에서 했으므로, 충돌 직전 위치(현재위치)에 다시 그려주는 게 안전하지만
            // mergeBlock이 현재 x,y 기준으로 total_block에 1을 찍으므로 괜찮음.

            // 만약 y가 음수라면 게임오버
            if (block_y < 0) ret = 1;
            else ret = 2; // 착지
        }
        break;
    case KEY_SPACE: // 하드 드롭
        while (!checkCollision(block_shape, block_angle, block_x, block_y + 1)) {
            block_y++;
        }
        if (block_y < 0) ret = 1;
        else ret = 2;
        break;
    }

    // 착지(2)나 게임오버(1)가 아니면 다시 그림
    if (ret == 0) {
        showCurrentBlock(block_shape, block_angle, block_x, block_y);
    }
    else if (ret == 2) {
        // 착지 처리
        // erase된 상태이므로 다시 그려줄 필요 없이 바로 merge해도 되지만, 
        // 시각적으로 마지막 위치를 보여주고 병합
        showCurrentBlock(block_shape, block_angle, block_x, block_y);
        mergeBlock(block_shape, block_angle, block_x, block_y);
    }

    return ret;
}

// 메인 게임 루프
void Tetris::run() {
    showLogo();

    while (true) {
        // 1. 게임 시작 전 초기화 및 입력
        inputStartLevel();
        initGame();
        showTotalBlock();
        showGameStat();

        // 2. 첫 블럭 생성
        block_shape = makeNewBlock();
        next_block_shape = makeNewBlock();

        block_x = 5;
        block_y = -3; // 위에서 시작
        block_angle = 0;

        showNextBlock(next_block_shape);

        // 3. 게임 플레이 루프 (한 판)
        bool isGameOver = false;

        while (!isGameOver) {
            // 키 입력 처리 및 타이머 루프
            for (int i = 0; i < stage_data[level].speed; i++) {
                if (_kbhit()) {
                    int key = _getch();
                    if (key == 0 || key == 0xE0) {
                        key = _getch();
                        int result = processBlockMove(key);
                        if (result == 1) { isGameOver = true; break; }
                        if (result == 2) {
                            // 착지했으므로 새 블럭 생성 후 루프 탈출(타이머 리셋 효과)
                            goto NEW_BLOCK;
                        }
                    }
                    else if (key == KEY_SPACE) {
                        int result = processBlockMove(KEY_SPACE);
                        if (result == 1) { isGameOver = true; break; }
                        if (result == 2) goto NEW_BLOCK;
                    }
                }
                Sleep(20); // 프레임 딜레이
            }

            // 시간 경과로 인한 자동 낙하:
            // C2362 오류 방지를 위해 블록({ })으로 감싸 변수 result의 범위를 제한함
            {
                int result = dropBlock();
                if (result == 1) isGameOver = true;
                else if (result == 2) goto NEW_BLOCK;
            }

            continue;

        NEW_BLOCK:
            // 스테이지 클리어 체크
            if (lines >= stage_data[level].clear_line) {
                level++;
                lines = 0;
                initGame(); // 보드 초기화 및 다음 레벨 시작
                showTotalBlock();
                showGameStat();
            }

            // 새 블럭 교체
            block_shape = next_block_shape;
            next_block_shape = makeNewBlock();
            showNextBlock(next_block_shape);

            // 위치 리셋
            block_x = 5;
            block_y = -1; // 생성 위치 조정
            block_angle = 0;

            // 생성되자마자 죽는지 체크
            if (checkCollision(block_shape, block_angle, block_x, block_y)) {
                isGameOver = true;
            }
        }

        // 게임 오버 처리
        showGameOver();
        // while(true)에 의해 다시 레벨 입력으로 돌아감
    }
}