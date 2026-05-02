#pragma once

#include "core/GameState.h"
#include "main.h"
#include "ai/strategy/IStrategy.h"
#include "ai/training/DataLogger.h"
#include <memory>

class GameManager
{
private:
    static int s_gameId;
    int _gameId;

    GameState _state;
    CellState _me;
    CellState _opponent;
    IStrategy* _strategy;
    std::unique_ptr<DataLogger> _logger;

public:
    GameManager();

    void init(CellState mySide);

    void applyMove(const AIMove& move);

    AIMove chooseMove();

    void finalizeGame();

    const GameState& getState() const;
};
