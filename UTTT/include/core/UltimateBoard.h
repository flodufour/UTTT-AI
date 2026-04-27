#pragma once

#include "subBoard.h"
#include "GameMove.h"
#include <array>

class UltimateBoard {
private:
    std::array<SubBoard, 9> boards;

    int activeBoard;

public:
    UltimateBoard();

    bool playMove(GameMove gameMove, CellState player);

    bool isValidMove(GameMove gameMove) const;

    CellState checkWinner() const;

    bool isFull() const;

    void updateActiveBoard(int lastCellIndex);

    int getActiveBoard() const;
    SubBoard& getBoard(int index);
    const SubBoard& getBoard(int index) const;

    void reset();
};
