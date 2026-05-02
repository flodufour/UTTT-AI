#include "ai/GameManager.h"
#include "ai/strategy/RandomStrategy.h"
#include "ai/strategy/SimpleStrategy.h"
#include "ai/strategy/MinimaxStrategy.h"
#include "ai/evaluate/HeuristicEvaluator.h"
#include <iostream>

int GameManager::s_gameId = 0;

GameManager::GameManager()
{
    _strategy = new MinimaxStrategy(new HeuristicEvaluator());
}

void GameManager::init(CellState mySide)
{
    _gameId = s_gameId++;
    _me = mySide;
    _opponent = (mySide == CellState::X) ? CellState::O : CellState::X;

    _state.reset();
    _state.setPlayers(mySide);
    if (!_logger)
        _logger = std::make_unique<DataLogger>("dataset.jsonl");
    _logger->setGameId(_gameId);
}

void GameManager::applyMove(const AIMove& move)
{
    _state.applyMove(move);

    if (_logger)
    {
        std::array<int, DataLogger::SIZE> state{};

        int idx = 0;
        const UltimateBoard& board = _state.getBoard();

        for (int b = 0; b < 9; ++b)
        {
            const SubBoard& sb = board.getBoard(b);

            for (int c = 0; c < 9; ++c)
            {
                CellState s = sb.getCell(c).getState();

                state[idx++] =
                    (s == CellState::X) ? 1 :
                    (s == CellState::O) ? 2 : 0;
            }
        }

        _logger->logState(
            state,
            move.boardIndex,
            move.cellIndex,
            (_state.getCurrentPlayer() == CellState::X ? 1 : 2),
            board.getActiveBoard()
        );
    }
}

AIMove GameManager::chooseMove()
{
    return _strategy->chooseMove(_state);
}


const GameState& GameManager::getState() const
{
    return _state;
}


void GameManager::finalizeGame()
{
    if (!_logger)
        return;

    CellState w = _state.getWinner();

    int result =
        (w == CellState::X) ? 1 :
        (w == CellState::O) ? 2 : 0;

    _logger->setResult(result);
    _logger->flush();
}
