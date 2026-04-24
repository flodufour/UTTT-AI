#include <iostream>
#include "main.h"
#include "core/GameState.h"

int main()
{
    game.initialize(10, Level::EASY_1, Mode::DEBUG, false, "Pseudo");

    GameState state;

    while (!game.isAllGameFinish())
    {
        while (!game.isFinish())
        {
            GameMove opponentMove;
            game.getMove(opponentMove);

            std::cerr << "IA move " << opponentMove.row << " " << opponentMove.col << std::endl;

            state.applyOpponentMove(opponentMove);

            GameMove myMove = {0, 2};

            std::cerr << "Send move " << myMove.row << " " << myMove.col << std::endl;

            game.setMove(myMove);

            state.applyMyMove(myMove);
        }
    }

    return 0;
}
