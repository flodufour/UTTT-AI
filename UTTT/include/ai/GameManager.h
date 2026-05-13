#pragma once

#include "core/GameState.h"
#include "main.h"
#include "ai/strategy/IStrategy.h"
#include "ai/training/DataLogger.h"
#include "ai/evaluate/IEvaluator.h"
#include <memory>

class GameManager
{
private:
    static int s_gameId;
    int _gameId;

    long long _runTimestamp;

    GameState _state;
    CellState _me;
    CellState _opponent;
    std::unique_ptr<IEvaluator> _evaluator;
    std::unique_ptr<IStrategy> _minimaxStrategy;
    std::unique_ptr<DataLogger> _logger;

public:

    GameManager(long long _runTimestamp);

    void init(CellState mySide);

    void applyMove(const AIMove& move);

    AIMove chooseMove();

    void finalizeGame();

    const GameState& getState() const;

    CellState getOpponent() const;

    ~GameManager();
};
