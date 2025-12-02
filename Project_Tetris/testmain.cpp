#include "TetrisGame.h"
#include "BattleTetris.h"
#include "ConsoleHelper.h"

int main() {
    // [ìˆ˜ì •] cols=100 -> 120, lines=30 -> 40 ìœ¼ë¡œ ë³€ê²½ (ConsoleHelper ì„¤ì •ê³¼ í†µì¼)
    system("mode con:cols=120 lines=40");
    system("title Tetris Project");
    ConsoleHelper::setCursorVisible(false);

    while (true) {
        // ... (ë‚˜ë¨¸ì§€ ì½”ë“œëŠ” ê·¸ëŒ€ë¡œ ìœ ì§€) ...
        system("cls");
        ConsoleHelper::setColor(SKY_BLUE);
        ConsoleHelper::setCursorPosition(20, 5);  printf("====================================");
        ConsoleHelper::setCursorPosition(20, 6);  printf("       TETRIS PROJECT LAUNCHER      ");
        ConsoleHelper::setCursorPosition(20, 7);  printf("====================================");

        ConsoleHelper::setColor(WHITE);
        ConsoleHelper::setCursorPosition(25, 10); printf("[1] SINGLE PLAYER (CLASSIC)");
        ConsoleHelper::setCursorPosition(25, 11); printf("[2] SINGLE PLAYER (SEA LEVEL MODE)");
        ConsoleHelper::setCursorPosition(25, 12); printf("[3] 2-PLAYER BATTLE (CLASSIC)");
        // [Ãß°¡] 4¹ø ¸Þ´º Ãß°¡
        ConsoleHelper::setCursorPosition(25, 13); printf("[4] 2-PLAYER BATTLE (SEA LEVEL MODE)");
        // [º¯°æ] Á¾·á ¸Þ´º ¹øÈ£ º¯°æ (4 -> 5)
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
            battle.run(false); // Å¬·¡½Ä ¸ðµå
        }
        else if (choice == '4') {
            // [Ãß°¡] 2ÀÎ¿ë ÇØ¼ö¸é ¸ðµå ½ÇÇà
            BattleTetris battle;
            battle.run(true); // ÇØ¼ö¸é ¸ðµå ÄÑ±â (true)
        }
        else if (choice == '5' || choice == KEY_ESC) {
            // [º¯°æ] Á¾·á Å° 5¹øÀ¸·Î º¯°æ
            ConsoleHelper::setCursorPosition(25, 19);
            printf("Exiting Game...");
            Sleep(1000);
            break;
        }
    }

    return 0;
}