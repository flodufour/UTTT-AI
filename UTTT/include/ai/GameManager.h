#pragma once

#include "core/GameState.h"
#include "main.h"
#include "ai/strategy/IStrategy.h"

class GameManager
{
private:
    GameState _state;
    CellState _me;
    CellState _opponent;
    IStrategy* _strategy;

public:
    GameManager();

    void init(CellState mySide);

    void applyMove(const AIMove& move);

    AIMove chooseMove();

    const GameState& getState() const;
};
