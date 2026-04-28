#pragma once

enum class CellState {
    EMPTY,
    X,
    O
};

class Cell {
private:
    CellState _state;

public:
    Cell();

    CellState getState() const;
    void setState(CellState newState);

    bool isEmpty() const;
};
