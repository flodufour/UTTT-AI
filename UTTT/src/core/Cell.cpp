#include "core/Cell.h"

Cell::Cell() : _state(CellState::EMPTY) {}

CellState Cell::getState() const {
    return _state;
}

void Cell::setState(CellState newState) {
    _state = newState;
}

bool Cell::isEmpty() const {
    return _state == CellState::EMPTY;
}
