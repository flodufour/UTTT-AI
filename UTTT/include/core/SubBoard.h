#pragma once

#include "core/Cell.h"
#include <array>
#include <stack>

/// @struct SubMove
/// @brief Represents a move made inside a sub-board
struct SubMove
{
    int index;                  ///< Index of the played cell (0–8)
    CellState previousState;    ///< State of the cell before the move
};


/// @class SubBoard
/// @brief Represents a 3x3 Tic Tac Toe board
///
/// Handles local game logic including move placement, win detection,
/// and undo functionality.
class SubBoard {
private:
    std::array<Cell, 9> _cells;
    std::stack<SubMove> _history;

public:
    /// @brief Constructs an empty sub-board
    SubBoard();

    /// @brief Plays a move on the board
    /// @param index cell index (0–8)
    /// @param player player making the move
    /// @return true if move is valid and applied, false otherwise
    bool playMove(int index, CellState player);

    /// @brief Checks if a player has won the sub-board
    /// @return X, O, or EMPTY if no winner
    CellState checkWinner() const;

    /// @brief Checks if the board is full
    /// @return true if no empty cells remain
    bool isFull() const;

    /// @brief Checks if the board is still playable
    /// @return true if no winner and not full
    bool isPlayable() const;

    /// @brief Gets a cell at a given index
    /// @param index cell index (0–8)
    /// @return constant reference to the cell
    const Cell& getCell(int index) const;

    /// @brief Resets the board to empty state
    void reset();

    /// @brief Checks if the board is completely empty
    /// @return true if all cells are EMPTY
    bool isEmpty() const;

    /// @brief Undoes the last move played
    void undoMove();
};
