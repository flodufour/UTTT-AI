#include <iostream>
#include "main.h"
#include "ai/GameManager.h"
#include "utils/MoveConverter.h"

int main()
{

    game.initialize(10, Level::EASY_2, Mode::DEBUG, false, "Pseudo");

    MoveConverter converter;

    GameManager manager;


    while (!game.isAllGameFinish())
    {
        manager.init(CellState::O);
        while (!game.isFinish())
        {
            GameMove opponentMove;
            game.getMove(opponentMove);
            AIMove opponentAIMove = converter.toAIMove(opponentMove);

            std::cerr << "IA move " << opponentAIMove.boardIndex << " " << opponentAIMove.cellIndex << std::endl;
            manager.applyMove(opponentAIMove);

            AIMove myMove = manager.chooseMove();
            std::cerr << "Send move " << myMove.boardIndex << " " << myMove.cellIndex<< std::endl;
            manager.applyMove(myMove);
            GameMove engineMove = converter.toGameMove(myMove);
            game.setMove(engineMove);

        }
    }

    return 0;
}


