#include <iostream>
#include "main.h"
#include "ai/GameManager.h"
#include "utils/MoveConverter.h"
#include "utils/ZobristHasher.h"
#include <ctime>

int main()
{
    Zobrist::init();

    long long start_timestamp = std::time(nullptr);

    MoveConverter converter;

    GameManager manager(start_timestamp);

    game.initialize(100, Level::MEDIUM_2, Mode::ARENA, false, "MyAi");

    while (!game.isAllGameFinish())
    {
        GameMove opponentMove;
        game.getMove(opponentMove);
        AIMove opponentAIMove = converter.toAIMove(opponentMove);

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

        while (!game.isFinish())
        {
            AIMove myMove = manager.chooseMove();
            std::cerr << "my move " << myMove.boardIndex << " " << myMove.cellIndex<< std::endl;
            manager.applyMove(myMove);
            GameMove engineMove = converter.toGameMove(myMove);
            game.setMove(engineMove);

            GameMove opponentMove;
            game.getMove(opponentMove);
            AIMove opponentAIMove = converter.toAIMove(opponentMove);
            std::cerr << "opponent move " << opponentAIMove.boardIndex << " " << opponentAIMove.cellIndex << std::endl;
            manager.applyMove(opponentAIMove);

        }

        manager.finalizeGame();

    }
    std::cerr << "MAIN FINISHED" << std::endl;
    return 0;
}


