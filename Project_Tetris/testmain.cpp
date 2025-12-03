#include "TetrisGame.h"
#include "BattleTetris.h"
#include "ConsoleHelper.h"

int main() {
    
    system("mode con:cols=120 lines=40");
    system("title Tetris Project");
    ConsoleHelper::setCursorVisible(false);

    while (true) {
        
        system("cls");
        ConsoleHelper::setColor(SKY_BLUE);
        ConsoleHelper::setCursorPosition(20, 5);  printf("====================================");
        ConsoleHelper::setCursorPosition(20, 6);  printf("       TETRIS PROJECT LAUNCHER      ");
        ConsoleHelper::setCursorPosition(20, 7);  printf("====================================");

        ConsoleHelper::setColor(WHITE);
        ConsoleHelper::setCursorPosition(25, 10); printf("[1] SINGLE PLAYER (CLASSIC)");
        ConsoleHelper::setCursorPosition(25, 11); printf("[2] SINGLE PLAYER (SEA LEVEL MODE)");
        ConsoleHelper::setCursorPosition(25, 12); printf("[3] 2-PLAYER BATTLE (CLASSIC)");
        // [추가] 4번 메뉴 추가
        ConsoleHelper::setCursorPosition(25, 13); printf("[4] 2-PLAYER BATTLE (SEA LEVEL MODE)");
        // [변경] 종료 메뉴 번호 변경 (4 -> 5)
        ConsoleHelper::setCursorPosition(25, 14); printf("[5] EXIT");

        ConsoleHelper::setCursorPosition(25, 17); printf(">> SELECT OPTION: ");

        while (_kbhit()) _getch();
        char choice = _getch();

        if (choice == '1') {
            TetrisGame game(false);
            game.run();
        }
        else if (choice == '2') {
            TetrisGame game(true);
            game.run();
        }
        else if (choice == '3') {
            BattleTetris battle;
            battle.run(false); // 클래식 모드
        }
        else if (choice == '4') {
            // [추가] 2인용 해수면 모드 실행
            BattleTetris battle;
            battle.run(true); // 해수면 모드 켜기 (true)
        }
        else if (choice == '5' || choice == KEY_ESC) {
            // [변경] 종료 키 5번으로 변경
            ConsoleHelper::setCursorPosition(25, 19);
            printf("Exiting Game...");
            Sleep(1000);
            break;
        }
    }

    return 0;
}