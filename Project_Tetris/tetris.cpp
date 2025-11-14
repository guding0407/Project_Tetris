#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h> // strtol

//*********************************
// 상수 선언
//*********************************

// _getch() 확장키 prefix는 0 또는 0xE0
#define KEY_LEFT   0x4b
#define KEY_RIGHT  0x4d
#define KEY_UP     0x48
#define KEY_DOWN   0x50

//*********************************
// 구조체 선언
//*********************************
struct STAGE {        // 각 스테이지마다의 난이도 설정
    int speed;       // 숫자가 낮을수록 속도가 빠르다
    int stick_rate;  // 막대가 나오는 확률 0~99 , 99면 막대기만 나옴
    int clear_line;
};

enum {
    BLACK,        /*  0 : 까망 */
    DARK_BLUE,    /*  1 : 어두운 파랑 */
    DARK_GREEN,   /*  2 : 어두운 초록 */
    DARK_SKY_BLUE,/*  3 : 어두운 하늘 */
    DARK_RED,     /*  4 : 어두운 빨강 */
    DARK_VOILET,  /*  5 : 어두운 보라 */
    DARK_YELLOW,  /*  6 : 어두운 노랑 */
    GRAY,         /*  7 : 회색 */
    DARK_GRAY,    /*  8 : 어두운 회색 */
    BLUE,         /*  9 : 파랑 */
    GREEN,        /* 10 : 초록 */
    SKY_BLUE,     /* 11 : 하늘 */
    RED,          /* 12 : 빨강 */
    VOILET,       /* 13 : 보라 */
    YELLOW,       /* 14 : 노랑 */
    WHITE,        /* 15 : 하양 */
};

//*********************************
// 전역변수선언
//*********************************
int level;
int ab_x, ab_y;    // 화면 중 블럭의 절대 위치
int block_shape, block_angle, block_x, block_y;
int next_block_shape;
int score;
int lines;
int hud_drawn;    // HUD(STAGE/SCORE/LINES) 그렸는지 여부 (게임 재시작 시 초기화)
char total_block[21][14];        // 화면에 표시되는 블럭들
struct STAGE stage_data[10];

// block[shape][angle][row][col]
char block[7][4][4][4] = {
    //막대모양
    1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,  1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,  1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,  1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
    //네모모양
    1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,  1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,  1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,  1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
    //'ㅓ' 모양
    0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,  1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,  1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,  0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
    //'ㄱ'모양
    1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,  1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,  1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,  0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,
    //'ㄴ' 모양
    1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,  1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,  0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,  1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,
    //'Z' 모양
    1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,  0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,  1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,  0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,
    //'S' 모양
    0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,  1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,  0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,  1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0
};

//*********************************
// 함수 선언
//*********************************
int gotoxy(int x, int y);                     // 커서 옮기기
void SetColor(int color);                    // 색 표현
int init();                                  // 각종변수 초기화
int show_cur_block(int shape, int angle, int x, int y);
int erase_cur_block(int shape, int angle, int x, int y);
int show_total_block();
int show_next_block(int shape);
int make_new_block();                        // block 모양 번호 반환
int strike_check(int shape, int angle, int x, int y);
int merge_block(int shape, int angle, int x, int y);
int block_start(int shape, int* angle, int* x, int* y);
int move_block(int* shape, int* angle, int* x, int* y, int* next_shape);
int rotate_block(int shape, int* angle, int* x, int* y);
int show_gameover();
int show_gamestat();
int show_logo();
int input_data();
int check_full_line();

int main()
{
    int i;

    init();
    show_logo();

    while (1)
    {
        int is_gameover = 0;   // ★ 매 판 시작 시 리셋

        input_data();
        show_total_block();

        block_shape = make_new_block();
        next_block_shape = make_new_block();
        show_next_block(next_block_shape);
        block_start(block_shape, &block_angle, &block_x, &block_y);
        show_gamestat();

        for (i = 1; 1; i++)
        {
            if (_kbhit())
            {
                // 키 입력 처리: _getch(), int, prefix 0/0xE0
                int prefix = _getch();
                if (prefix == 0 || prefix == 0xE0) {
                    int key = _getch();
                    switch (key)
                    {
                    case KEY_UP:        // 회전
                        if (strike_check(block_shape, (block_angle + 1) % 4, block_x, block_y) == 0)
                        {
                            erase_cur_block(block_shape, block_angle, block_x, block_y);
                            block_angle = (block_angle + 1) % 4;
                            show_cur_block(block_shape, block_angle, block_x, block_y);
                        }
                        break;
                    case KEY_LEFT:      // 왼쪽 이동
                        erase_cur_block(block_shape, block_angle, block_x, block_y);
                        block_x--;
                        if (strike_check(block_shape, block_angle, block_x, block_y))
                            block_x++;
                        show_cur_block(block_shape, block_angle, block_x, block_y);
                        break;
                    case KEY_RIGHT:     // 오른쪽 이동
                        erase_cur_block(block_shape, block_angle, block_x, block_y);
                        block_x++;
                        if (strike_check(block_shape, block_angle, block_x, block_y))
                            block_x--;
                        show_cur_block(block_shape, block_angle, block_x, block_y);
                        break;
                    case KEY_DOWN:      // 아래 이동
                        is_gameover = move_block(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
                        show_cur_block(block_shape, block_angle, block_x, block_y);
                        break;
                    }
                }
                else if (prefix == 32) { // 스페이스바(하드드롭)
                    while (is_gameover == 0)
                        is_gameover = move_block(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
                    show_cur_block(block_shape, block_angle, block_x, block_y);
                }
            }

            if (i % stage_data[level].speed == 0)
            {
                is_gameover = move_block(&block_shape, &block_angle, &block_x, &block_y, &next_block_shape);
                show_cur_block(block_shape, block_angle, block_x, block_y);
            }

            if (stage_data[level].clear_line == lines)    // 스테이지 클리어
            {
                level++;
                lines = 0;
            }
            if (is_gameover == 1)
            {
                show_gameover();
                SetColor(GRAY);
                system("cls"); // ★ 혹시 남은 잔상까지 정리
                break;
            }

            gotoxy(77, 23);
            Sleep(15);
            gotoxy(77, 23);
        }
        init();
    }
    return 0;
}

int gotoxy(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.Y = y;
    pos.X = x;
    SetConsoleCursorPosition(hConsole, pos);
    return 0;
}

void SetColor(int color)
{
    static HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(std_output_handle, color);
}

int init()
{
    int i, j;

    srand((unsigned)time(NULL));

    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 14; j++)
        {
            if ((j == 0) || (j == 13))
                total_block[i][j] = 1;
            else
                total_block[i][j] = 0;
        }
    }

    for (j = 0; j < 14; j++)            // 화면의 제일 밑의 줄은 1로 채운다.
        total_block[20][j] = 1;

    // 전역변수 초기화
    level = 0;
    lines = 0;
    score = 0;        // 점수 초기화
    hud_drawn = 0;    // HUD 다시 그리기 허용
    ab_x = 5;
    ab_y = 1;

    stage_data[0].speed = 40;
    stage_data[0].stick_rate = 20;
    stage_data[0].clear_line = 20;
    stage_data[1].speed = 38;
    stage_data[1].stick_rate = 18;
    stage_data[1].clear_line = 20;
    stage_data[2].speed = 35;
    stage_data[2].stick_rate = 18;
    stage_data[2].clear_line = 20;
    stage_data[3].speed = 30;
    stage_data[3].stick_rate = 17;
    stage_data[3].clear_line = 20;
    stage_data[4].speed = 25;
    stage_data[4].stick_rate = 16;
    stage_data[4].clear_line = 20;
    stage_data[5].speed = 20;
    stage_data[5].stick_rate = 14;
    stage_data[5].clear_line = 20;
    stage_data[6].speed = 15;
    stage_data[6].stick_rate = 14;
    stage_data[6].clear_line = 20;
    stage_data[7].speed = 10;
    stage_data[7].stick_rate = 13;
    stage_data[7].clear_line = 20;
    stage_data[8].speed = 6;
    stage_data[8].stick_rate = 12;
    stage_data[8].clear_line = 20;
    stage_data[9].speed = 4;
    stage_data[9].stick_rate = 11;
    stage_data[9].clear_line = 99999;
    return 0;
}

// block[row][col] 인덱싱 통일 + y<0 가드
int show_cur_block(int shape, int angle, int x, int y)
{
    int r, c;

    switch (shape)
    {
    case 0: SetColor(RED); break;
    case 1: SetColor(BLUE); break;
    case 2: SetColor(SKY_BLUE); break;
    case 3: SetColor(WHITE); break;
    case 4: SetColor(YELLOW); break;
    case 5: SetColor(VOILET); break;
    case 6: SetColor(GREEN); break;
    }

    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            if (!block[shape][angle][r][c]) continue;
            if (y + r < 0) continue; // 화면 위는 그리지 않음

            gotoxy((c + x) * 2 + ab_x, y + r + ab_y);
            printf("■");
        }
    }
    SetColor(BLACK);
    gotoxy(77, 23);
    return 0;
}

int erase_cur_block(int shape, int angle, int x, int y)
{
    int r, c;
    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            if (!block[shape][angle][r][c]) continue;
            if (y + r < 0) continue; // 화면 위는 지우지 않음

            gotoxy((c + x) * 2 + ab_x, y + r + ab_y);
            printf("  ");
        }
    }
    return 0;
}

int show_total_block()
{
    int i, j;
    SetColor(DARK_GRAY);
    for (i = 0; i < 21; i++)
    {
        for (j = 0; j < 14; j++)
        {
            if (j == 0 || j == 13 || i == 20)        // 레벨에 따라 외벽 색이 변함
                SetColor((level % 6) + 1);
            else
                SetColor(DARK_GRAY);

            gotoxy((j * 2) + ab_x, i + ab_y);
            if (total_block[i][j] == 1)
                printf("■");
            else
                printf("  ");
        }
    }
    SetColor(BLACK);
    gotoxy(77, 23);
    return 0;
}

// UI를 호출하지 않도록 순수 생성만 수행
int make_new_block()
{
    int i = rand() % 100;
    if (i <= stage_data[level].stick_rate) // 막대 확률
        return 0;
    return (rand() % 6) + 1; // 1~6
}

// 인덱싱 통일 + 범위 가드
int strike_check(int shape, int angle, int x, int y)
{
    int r, c;
    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            if (!block[shape][angle][r][c]) continue;

            int gy = y + r;
            int gx = x + c;

            // 좌우 벽 충돌
            if (gx <= 0 || gx >= 13) return 1;

            // 윗부분은 통과
            if (gy < 0) continue;

            // 바닥 넘음
            if (gy >= 21) return 1;

            // 기존 블럭과 충돌
            if (total_block[gy][gx]) return 1;
        }
    }
    return 0;
}

// 인덱싱 통일 + 범위 가드
int merge_block(int shape, int angle, int x, int y)
{
    int r, c;
    for (r = 0; r < 4; r++)
    {
        for (c = 0; c < 4; c++)
        {
            if (!block[shape][angle][r][c]) continue;
            int gy = y + r, gx = x + c;
            if (gy < 0 || gy > 20 || gx < 0 || gx > 13) continue;
            total_block[gy][gx] = 1;
        }
    }
    check_full_line();
    show_total_block();
    return 0;
}

int block_start(int shape, int* angle, int* x, int* y)
{
    *x = 5;
    *y = -3;
    *angle = 0;
    return 0;
}

int show_gameover()
{
    SetColor(RED);
    gotoxy(15, 8);
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(15, 9);
    printf("┃**************************┃");
    gotoxy(15, 10);
    printf("┃*        GAME OVER       *┃");
    gotoxy(15, 11);
    printf("┃**************************┃");
    gotoxy(15, 12);
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    Sleep(1000);

    _getch();
    system("cls");

    return 0;
}

int move_block(int* shape, int* angle, int* x, int* y, int* next_shape)
{
    erase_cur_block(*shape, *angle, *x, *y);

    (*y)++;    // 블럭을 한칸 아래로 내림
    if (strike_check(*shape, *angle, *x, *y) == 1)
    {
        if (*y < 0)    // 게임오버
            return 1;

        (*y)--;
        merge_block(*shape, *angle, *x, *y);
        *shape = *next_shape;
        *next_shape = make_new_block();

        block_start(*shape, angle, x, y);    // angle,x,y는 포인터
        show_next_block(*next_shape);
        return 2; // 착지
    }
    return 0; // 계속 진행
}

int rotate_block(int shape, int* angle, int* x, int* y)
{
    return 0;
}

int check_full_line()
{
    int i, j, k;
    for (i = 0; i < 20; i++)
    {
        for (j = 1; j < 13; j++)
        {
            if (total_block[i][j] == 0)
                break;
        }
        if (j == 13)    // 한 줄이 다 채워졌음
        {
            lines++;
            show_total_block();
            SetColor(BLUE);
            gotoxy(1 * 2 + ab_x, i + ab_y);
            for (j = 1; j < 13; j++)
            {
                printf("□");
                Sleep(10);
            }
            gotoxy(1 * 2 + ab_x, i + ab_y);
            for (j = 1; j < 13; j++)
            {
                printf("  ");
                Sleep(10);
            }

            for (k = i; k > 0; k--)
            {
                for (j = 1; j < 13; j++)
                    total_block[k][j] = total_block[k - 1][j];
            }
            for (j = 1; j < 13; j++)
                total_block[0][j] = 0;

            score += 100 + (level * 10) + (rand() % 10);
            show_gamestat();
        }
    }
    return 0;
}

int show_next_block(int shape)
{
    int i, j;
    SetColor((level + 1) % 6 + 1);
    for (i = 1; i < 7; i++)
    {
        gotoxy(33, i);
        for (j = 0; j < 6; j++)
        {
            if (i == 1 || i == 6 || j == 0 || j == 5)
                printf("■");
            else
                printf("  ");
        }
    }
    show_cur_block(shape, 0, 15, 1); // 사이드 패널에 표시
    return 0;
}

int show_gamestat()
{
    SetColor(GRAY);
    if (!hud_drawn)
    {
        gotoxy(35, 7);
        printf("STAGE");

        gotoxy(35, 9);
        printf("SCORE");

        gotoxy(35, 12);
        printf("LINES");

        hud_drawn = 1; // 한 번만 출력
    }
    gotoxy(41, 7);
    printf("%d", level + 1);
    gotoxy(35, 10);
    printf("%10d", score);
    gotoxy(35, 13);
    printf("%10d", stage_data[level].clear_line - lines);
    return 0;
}

// 안전 입력: 숫자만 허용 (비숫자 입력 시 커서 발작 방지)
int input_data()
{
    SetColor(GRAY);
    gotoxy(10, 7);  printf("┏━━━━━━━━━━<GAME KEY>━━━━━━━━┓");
    Sleep(10);      gotoxy(10, 8);  printf("┃ UP   : Rotate Block        ┃");
    Sleep(10);      gotoxy(10, 9);  printf("┃ DOWN : Move One-Step Down  ┃");
    Sleep(10);      gotoxy(10, 10); printf("┃ SPACE: Move Bottom Down    ┃");
    Sleep(10);      gotoxy(10, 11); printf("┃ LEFT : Move Left           ┃");
    Sleep(10);      gotoxy(10, 12); printf("┃ RIGHT: Move Right          ┃");
    Sleep(10);      gotoxy(10, 13); printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    char buf[64];
    long sel = 0;
    while (1) {
        gotoxy(10, 3);
        printf("Select Start level[1-8]:          ");
        gotoxy(35, 3);

        if (!fgets(buf, sizeof(buf), stdin)) {
            clearerr(stdin);
            continue;
        }

        char* endp = NULL;
        sel = strtol(buf, &endp, 10);

        while (*endp == ' ' || *endp == '\t' || *endp == '\r' || *endp == '\n') endp++;

        if (*endp == '\0' && sel >= 1 && sel <= 8) break;

        gotoxy(10, 5);
        printf("Please enter a number between 1 and 8.      ");
        Sleep(600);
        gotoxy(10, 5);
        printf("                                            ");
    }

    level = (int)sel - 1;
    system("cls");
    return 0;
}

int show_logo()
{
    int i, j;
    gotoxy(13, 3); 
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    Sleep(100);
    gotoxy(13, 4);
    printf("┃◆◆◆  ◆◆◆  ◆◆◆   ◆◆     ◆   ◆◆◆  ┃");
    Sleep(100);
    gotoxy(13, 5);
    printf("┃ ◆   ◆     ◆    ◆ ◆    ◆   ◆    ┃");
    Sleep(100);
    gotoxy(13, 6);
    printf("┃ ◆   ◆◆◆   ◆    ◆◆     ◆     ◆  ┃");
    Sleep(100);
    gotoxy(13, 7);
    printf("┃ ◆   ◆     ◆    ◆ ◆    ◆     ◆  ┃");
    Sleep(100);
    gotoxy(13, 8);
    printf("┃ ◆   ◆◆◆   ◆    ◆  ◆   ◆   ◆◆◆  ┃");
    Sleep(100);
    gotoxy(13, 9);
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

    gotoxy(28, 20);
    printf("Please Press Any Key~!");

    for (i = 0; i >= 0; i++) {
        if (i % 40 == 0)
        {
            for (j = 0; j < 5; j++)
            {
                gotoxy(18, 14 + j);
                printf("                                                          ");
            }
            show_cur_block(rand() % 7, rand() % 4, 6, 14);
            show_cur_block(rand() % 7, rand() % 4, 12, 14);
            show_cur_block(rand() % 7, rand() % 4, 19, 14);
            show_cur_block(rand() % 7, rand() % 4, 24, 14);
        }
        if (_kbhit())
            break;
        Sleep(30);
    }

    _getch();
    system("cls");

    return 0;
}
