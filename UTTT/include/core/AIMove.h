#pragma once

/// @struct AIMove
/// @brief Represents a move in Ultimate Tic Tac Toe.
///
/// A move is defined by:
/// - boardIndex: which sub-board (0-8)
/// - cellIndex: which cell inside the sub-board (0-8)
struct AIMove
{
    int boardIndex;
    int cellIndex;

    /// @brief Constructs an invalid move (-1, -1).
    AIMove() : boardIndex(-1), cellIndex(-1) {}

     /// @brief Constructs a move with given board and cell indices.
    AIMove(int b, int c) : boardIndex(b), cellIndex(c) {}


    /// @brief Checks if the move is within valid bounds.
    /// @return True if move is in [0..8] for both indices.
    bool isValid() const
    {
        return boardIndex >= 0 && boardIndex < 9 &&
               cellIndex >= 0 && cellIndex < 9;
    }

    /// @brief Equal operator for AIMove.
    /// @return True if both AIMoves are equals.
    bool operator==(const AIMove& other) const
    {
        return boardIndex == other.boardIndex &&
               cellIndex == other.cellIndex;
    }

    /// @brief Not equal operator for AIMove.
    /// @return True if both AIMoves are different.
    bool operator!=(const AIMove& other) const
    {
        return !(*this == other);
    }
};
