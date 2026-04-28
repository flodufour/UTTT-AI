#include "core/SubBoard.h"

SubBoard::SubBoard() {
    reset();
}

bool SubBoard::playMove(int index, CellState player) {
    if (index < 0 || index >= 9)
        return false;

    if (!_cells[index].isEmpty())
        return false;

    if (checkWinner() != CellState::EMPTY)
        return false; // board déjà gagné

    _cells[index].setState(player);
    return true;
}

Cell& SubBoard::getCell(int index) {
    return _cells[index];
}

const Cell& SubBoard::getCell(int index) const {
    return _cells[index];
}

bool SubBoard::isFull() const {
    for (const auto& cell : _cells) {
        if (cell.isEmpty())
            return false;
    }
    return true;
}

CellState SubBoard::checkWinner() const {
    const int win[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };

    for (const auto& line : win) {
        CellState a = _cells[line[0]].getState();
        CellState b = _cells[line[1]].getState();
        CellState c = _cells[line[2]].getState();

        if (a != CellState::EMPTY && a == b && b == c)
            return a;
    }

    return CellState::EMPTY;
}

bool SubBoard::isPlayable() const {
    return checkWinner() == CellState::EMPTY && !isFull();
}

void SubBoard::reset() {
    for (auto& cell : _cells) {
        cell.setState(CellState::EMPTY);
    }
}
