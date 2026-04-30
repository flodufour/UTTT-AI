#pragma once

#include "core/UltimateBoard.h"
#include "main.h"
#include <vector>
#include <stack>

struct StateSnapshot
{
    AIMove move;
    CellState currentPlayer;
    int activeBoard;
};


class GameState
{
private:

    UltimateBoard _board;

    CellState _myPlayer;
    CellState _opponent;
    CellState _currentPlayer;

    std::stack<StateSnapshot> _history;

public:
    GameState();

    void setPlayers(CellState me);

    void switchPlayers();

    void reset();

    bool applyMove(const AIMove& move);

    CellState getCurrentPlayer() const;

    const UltimateBoard& getBoard() const;

    std::vector<AIMove> getValidMoves() const;

    bool isTerminal() const;

    CellState getMyPlayer() const { return _myPlayer; }

    CellState getOpponent() const { return _opponent; }

    CellState getWinner() const;

    void undoMove();


};

