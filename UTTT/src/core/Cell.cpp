#include "core/Cell.h"

Cell::Cell() : state(CellState::EMPTY) {}

CellState Cell::getState() const {
    return state;
}

void Cell::setState(CellState newState) {
    state = newState;
}

bool Cell::isEmpty() const {
    return state == CellState::EMPTY;
}
