#pragma once

#include "core/UltimateBoard.h"
#include "main.h"

class GameState
{
private:
    UltimateBoard board;

    CellState myPlayer = CellState::X;
    CellState opponent = CellState::O;

public:
    GameState();

    void reset();

    void applyOpponentMove(const GameMove& move);

    void applyMyMove(const GameMove& move);

    const UltimateBoard& getBoard() const;
};
