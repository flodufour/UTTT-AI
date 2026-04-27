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

    void applyMove(const GameMove& move);

    GameMove chooseMove();

    const GameState& getState() const;
};
