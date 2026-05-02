#pragma once

/// @enum CellState
/// @brief Represents the state of a cell in the game
enum class CellState {
    EMPTY,  ///< Cell is not occupied
    X,      ///< Cell occupied by player X
    O       ///< Cell occupied by player O
};

/// @class Cell
/// @brief Represents a single cell in the game board
///
/// Stores and manages the state of a cell (EMPTY, X, or O).
class Cell {
private:
    CellState _state;

public:

    /// @brief Constructs an empty cell
    Cell();

    /// @brief Gets the current state of the cell
    /// @return current CellState
    CellState getState() const;

    /// @brief Sets the state of the cell
    /// @param newState new state to assign
    void setState(CellState newState);

    /// @brief Checks if the cell is empty
    /// @return true if the cell is EMPTY, false otherwise
    bool isEmpty() const;
};
