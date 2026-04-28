#pragma once

#include "subBoard.h"
#include "AIMove.h"
#include <array>

class UltimateBoard {
private:
    std::array<SubBoard, 9> boards;

    int activeBoard;

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

    CellState hasWinner() const;
};
