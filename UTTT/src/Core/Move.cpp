#include "core/Move.h"

Move::Move() : boardIndex(-1), cellIndex(-1) {}

Move::Move(int b, int c) : boardIndex(b), cellIndex(c) {}

bool Move::isValid() const {
    return boardIndex >= 0 && boardIndex < 9 &&
           cellIndex >= 0 && cellIndex < 9;
}

int Move::getBoardIndex() const {
    return boardIndex;
}

int Move::getCellIndex() const {
    return cellIndex;
}
