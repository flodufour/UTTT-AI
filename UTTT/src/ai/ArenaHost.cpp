#include "ai/ArenaHost.h"
#include <iostream>

ArenaHost::ArenaHost(GameManager& manager)
    : _manager(manager) {}

void ArenaHost::runSession(int numGames, Level level) {
    // initialisation
    game.initialize(numGames, level, Mode::ARENA, false, "MyAi");

    while (!game.isAllGameFinish()) {
        playSingleGame();
        _manager.finalizeGame();
    }
}

void ArenaHost::playSingleGame() {
    // game initialisation
    GameMove opponentMove;
    game.getMove(opponentMove);
    AIMove opponentAIMove = _converter.toAIMove(opponentMove);

    if (opponentAIMove.boardIndex < 0 || opponentAIMove.cellIndex < 0) {
        // AI starts as X
        _manager.init(CellState::X);
        std::cerr << "opponent move " << opponentAIMove.boardIndex << " " << opponentAIMove.cellIndex << std::endl;
    } else {
        // AI starts as O and applies first opponent move
        _manager.init(CellState::O);
        std::cerr << "opponent move " << opponentAIMove.boardIndex << " " << opponentAIMove.cellIndex << std::endl;
        _manager.applyMove(opponentAIMove);
    }

    // game play
    while (!game.isFinish()) {
        // aI turn
        AIMove myMove = _manager.chooseMove();
        std::cerr << "my move " << myMove.boardIndex << " " << myMove.cellIndex << std::endl;

        _manager.applyMove(myMove);
        GameMove engineMove = _converter.toGameMove(myMove);
        game.setMove(engineMove);

        // opponent turn
        GameMove nextOpponentMove;
        game.getMove(nextOpponentMove);
        AIMove nextOpponentAIMove = _converter.toAIMove(nextOpponentMove);

        std::cerr << "opponent move " << nextOpponentAIMove.boardIndex << " " << nextOpponentAIMove.cellIndex << std::endl;
        _manager.applyMove(nextOpponentAIMove);
    }
}
