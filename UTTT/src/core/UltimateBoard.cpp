#include "core/UltimateBoard.h"

UltimateBoard::UltimateBoard() {
    reset();
}

void UltimateBoard::reset() {
    for (auto& b : boards) {
        b.reset();
    }
    activeBoard = -1;
}

bool UltimateBoard::isValidMove(AIMove aIMove) const {
    int boardIndex = aIMove.boardIndex;
    int cellIndex = aIMove.cellIndex;
    if (boardIndex < 0 || boardIndex >= 9) return false;
    if (cellIndex < 0 || cellIndex >= 9) return false;

    const SubBoard& board = boards[boardIndex];

    if (!board.isPlayable()) return false;

    if (!board.getCell(cellIndex).isEmpty()) return false;

    if (activeBoard != -1 && boardIndex != activeBoard)
        return false;

    return true;
}

bool UltimateBoard::playMove(AIMove aIMove, CellState player) {
    int boardIndex = aIMove.boardIndex;
    int cellIndex = aIMove.cellIndex;
    if (!isValidMove(aIMove))
        return false;

    boards[boardIndex].playMove(cellIndex, player);

    updateActiveBoard(cellIndex);

    return true;
}

void UltimateBoard::updateActiveBoard(int lastCellIndex) {


    if (lastCellIndex < 0 || lastCellIndex >= 9) {
        activeBoard = -1;
        return;
    }

    if (!boards[lastCellIndex].isPlayable()) {
        activeBoard = -1;
    }
    else {
        activeBoard = lastCellIndex;
    }
}

CellState UltimateBoard::checkWinner() const {
    const int win[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };

    for (const auto& line : win) {
        CellState a = boards[line[0]].checkWinner();
        CellState b = boards[line[1]].checkWinner();
        CellState c = boards[line[2]].checkWinner();

        if (a != CellState::EMPTY && a == b && b == c)
            return a;
    }

    return CellState::EMPTY;
}

bool UltimateBoard::isFull() const {
    for (const auto& b : boards) {
        if (!b.isFull())
            return false;
    }
    return true;
}

int UltimateBoard::getActiveBoard() const {
    return activeBoard;
}

SubBoard& UltimateBoard::getBoard(int index) {
    return boards[index];
}

const SubBoard& UltimateBoard::getBoard(int index) const {
    return boards[index];
}

CellState UltimateBoard::hasWinner() const
{
    const int win[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };

    for (const auto& line : win)
    {
        CellState a = boards[line[0]].checkWinner();
        CellState b = boards[line[1]].checkWinner();
        CellState c = boards[line[2]].checkWinner();

        if (a != CellState::EMPTY && a == b && b == c)
            return a;
    }

    return CellState::EMPTY;
}
