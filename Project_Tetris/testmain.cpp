#include "TetrisGame.h"

int main() {
    // 랜덤 시드 초기화
    srand((unsigned)time(NULL));

    // 게임 객체 생성 및 실행
    TetrisGame game;
    game.run();

    return 0;
}