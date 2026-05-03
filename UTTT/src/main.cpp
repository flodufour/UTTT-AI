#include <iostream>
#include "main.h"
#include "ai/GameManager.h"
#include "utils/MoveConverter.h"
#include <ctime>

int main()
{
    long long start_timestamp = std::time(nullptr);

    MoveConverter converter;

    GameManager manager(start_timestamp);

    game.initialize(100, Level::MEDIUM_2, Mode::ARENA, false, "MyAi");

    while (!game.isAllGameFinish())
    {
        bool initialized = false;

        while (!game.isFinish())
        {
            GameMove opponentMove;
            game.getMove(opponentMove);
            AIMove opponentAIMove = converter.toAIMove(opponentMove);

            if(!initialized){
                if (opponentAIMove.boardIndex < 0 || opponentAIMove.cellIndex < 0)
                {
                    manager.init(CellState::X);
                    std::cerr << "opponent move " << opponentAIMove.boardIndex << " " << opponentAIMove.cellIndex << std::endl;
                }
                else{
                    manager.init(CellState::O);
                    std::cerr << "opponent move " << opponentAIMove.boardIndex << " " << opponentAIMove.cellIndex << std::endl;
                    manager.applyMove(opponentAIMove);
                }
                initialized = true;
            }
            else {
                std::cerr << "opponent move " << opponentAIMove.boardIndex << " " << opponentAIMove.cellIndex << std::endl;
                manager.applyMove(opponentAIMove);
            }

            AIMove myMove = manager.chooseMove();
            std::cerr << "my move " << myMove.boardIndex << " " << myMove.cellIndex<< std::endl;
            manager.applyMove(myMove);
            GameMove engineMove = converter.toGameMove(myMove);
            game.setMove(engineMove);

        }
        manager.finalizeGame();
    }

    return 0;
}


