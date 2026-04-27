#pragma once

#include "core/GameState.h"
#include "main.h"

class GameManager
{
private:
    GameState state;
    CellState me;
    CellState opponent;

public:
    GameManager();

    void init(CellState mySide);

    void applyMove(const AIMove& move);

    AIMove chooseMove();

    const GameState& getState() const;
};
