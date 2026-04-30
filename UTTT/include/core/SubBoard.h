#pragma once

#include "core/Cell.h"
#include <array>
#include <stack>

struct SubMove
{
    int index;
    CellState previousState;
};

class SubBoard {
private:
    std::array<Cell, 9> _cells;
    std::stack<SubMove> _history;

public:
    SubBoard();

    bool playMove(int index, CellState player);

    CellState checkWinner() const;

    bool isFull() const;
    bool isPlayable() const;

    const Cell& getCell(int index) const;

    void reset();

    bool isEmpty() const;

    void undoMove();
};
