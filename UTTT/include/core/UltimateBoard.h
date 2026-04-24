#pragma once

#include "subBoard.h"
#include <array>

class UltimateBoard {
private:
    std::array<SubBoard, 9> boards;

    int activeBoard;

public:
    UltimateBoard();

    bool playMove(int boardIndex, int cellIndex, CellState player);

    bool isValidMove(int boardIndex, int cellIndex) const;

    CellState checkWinner() const;

    bool isFull() const;

    int getActiveBoard() const;

    void updateActiveBoard(int lastCellIndex);

    SubBoard& getBoard(int index);
    const SubBoard& getBoard(int index) const;

    void reset();
};
