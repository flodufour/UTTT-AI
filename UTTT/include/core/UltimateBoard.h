#pragma once

#include "subBoard.h"
#include "AIMove.h"
#include <array>
#include <stack>

/// @class UltimateBoard
/// @brief Represents the full Ultimate Tic Tac Toe game board.
///
/// The board is composed of 9 SubBoards.
class UltimateBoard {
private:
    std::array<SubBoard, 9> _boards;

    int _activeBoard;

public:
    /// @brief Constructs an empty Ultimate board.
    UltimateBoard();

    /// @brief Plays a move on the board.
    /// @param aIMove Move to apply.
    /// @param player Player making the move.
    /// @return True if the move was successfully applied.
    bool playMove(AIMove aIMove, CellState player);

    /// @brief Checks if a move is valid in the current state.
    /// @param aIMove Move to validate.
    /// @return True if the move is legal.
    bool isValidMove(AIMove aIMove) const;

    /// @brief Checks if a player has won the game.
    /// @return CellState::X, CellState::O, or EMPTY if no winner.
    CellState checkWinner() const;

    /// @brief Checks if the game is a draw.
    /// @return True if all boards are full and no winner exists.
    bool isFull() const;

    /// @brief Updates the active board based on last move played.
    /// @param lastCellIndex Index of the last played cell.
    void updateActiveBoard(int lastCellIndex);

    /// @brief Gets the current active board index.
    /// @return Active board index, or -1 if any board is playable.
    int getActiveBoard() const;

    /// @brief Gets a modifiable sub-board.
    /// @param index Sub-board index [0..8].
    /// @return Reference to the sub-board.
    SubBoard& getBoard(int index);

    /// @brief Gets a read-only sub-board.
    /// @param index Sub-board index [0..8].
    /// @return Const reference to the sub-board.
    const SubBoard& getBoard(int index) const;

    /// @brief Resets the entire game state.
    void reset();

    /// @brief Checks if the board is empty.
    /// @return True if no moves have been played.
    bool isEmpty() const;

    void undoMove(const AIMove& move, int prevActiveBoard);

    void setActiveBoard(int index);



};
