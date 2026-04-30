#pragma once

#include "subBoard.h"
#include "AIMove.h"
#include <array>
#include <stack>


struct UltimateMove
{
    AIMove move;
    int previousActiveBoard;
};

class UltimateBoard {
private:
    std::array<SubBoard, 9> _boards;

    int _activeBoard;

    std::stack<UltimateMove> _history;

public:
    UltimateBoard();

    bool playMove(AIMove aIMove, CellState player);

    bool isValidMove(AIMove aIMove) const;

    CellState checkWinner() const;

    bool isFull() const;

    void updateActiveBoard(int lastCellIndex);

    int getActiveBoard() const;
    SubBoard& getBoard(int index);
    const SubBoard& getBoard(int index) const;

    void reset();

    bool isEmpty() const;

    void undoMove();
};
