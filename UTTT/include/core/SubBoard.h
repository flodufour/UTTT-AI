#pragma once

#include "core/Cell.h"
#include <array>

class SubBoard {
private:
    std::array<Cell, 9> _cells;

public:
    SubBoard();

    bool playMove(int index, CellState player);

    CellState checkWinner() const;

    bool isFull() const;
    bool isPlayable() const;

    Cell& getCell(int index);
    const Cell& getCell(int index) const;

    void reset();

    bool isEmpty() const;
};
