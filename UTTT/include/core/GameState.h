#pragma once

#include "core/UltimateBoard.h"
#include "main.h"
#include <vector>

class GameState
{
private:

    UltimateBoard board;

    CellState myPlayer;
    CellState opponent;
    CellState currentPlayer;

public:
    GameState();

    void setPlayers(CellState me);

    void switchPlayers();

    void reset();

    void applyMove(const GameMove& move);

    CellState getCurrentPlayer() const;

    const UltimateBoard& getBoard() const;

    std::vector<GameMove> getValidMoves() const;
};
