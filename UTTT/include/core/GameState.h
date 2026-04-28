#pragma once

#include "core/UltimateBoard.h"
#include "main.h"
#include <vector>

class GameState
{
private:

    UltimateBoard _board;

    CellState _myPlayer;
    CellState _opponent;
    CellState _currentPlayer;

public:
    GameState();

    void setPlayers(CellState me);

    void switchPlayers();

    void reset();

    void applyMove(const AIMove& move);

    CellState getCurrentPlayer() const;

    const UltimateBoard& getBoard() const;

    std::vector<AIMove> getValidMoves() const;

    bool isTerminal() const;

    CellState getMyPlayer() const { return _myPlayer; }

    CellState getOpponent() const { return _opponent; }

    CellState getWinner() const;

};
