#include <iostream>
#include "main.h"
#include "manager/GameManager.h"

int main()
{
    game.initialize(10, Level::EASY_1, Mode::DEBUG, false, "Pseudo");

    GameManager manager;
    manager.init(CellState::O);

    while (!game.isAllGameFinish())
    {
        while (!game.isFinish())
        {
            GameMove opponentMove;
            game.getMove(opponentMove);
            std::cerr << "IA move " << opponentMove.boardIndex << " " << opponentMove.cellIndex << std::endl;
            manager.applyMove(opponentMove);

            GameMove myMove = manager.chooseMove();
            std::cerr << "Send move " << myMove.boardIndex << " " << myMove.cellIndex<< std::endl;
            game.setMove(myMove);
            manager.applyMove(myMove);

        }
    }

    return 0;
}
