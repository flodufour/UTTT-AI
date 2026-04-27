#include <iostream>
#include "main.h"
#include "core/GameState.h"

int main()
{
    game.initialize(10, Level::EASY_1, Mode::DEBUG, false, "Pseudo");

    GameState state;
    state.setPlayers(CellState::O);

    while (!game.isAllGameFinish())
    {
        while (!game.isFinish())
        {
            GameMove opponentMove;
            game.getMove(opponentMove);
            state.applyMove(opponentMove);
            std::cerr << "IA move " << opponentMove.row << " " << opponentMove.col << std::endl;
            state.switchPlayers();

            GameMove myMove = {0, 2};
            game.setMove(myMove);
            state.applyMove(myMove);
            std::cerr << "Send move " << myMove.row << " " << myMove.col << std::endl;
            state.switchPlayers();

        }
    }

    return 0;
}
