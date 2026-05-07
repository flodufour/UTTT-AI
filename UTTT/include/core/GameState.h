#pragma once

#include "core/UltimateBoard.h"
#include "main.h"
#include <vector>
#include <stack>
#include <cstdint>


/// @class GameState
/// @brief Represents the full state of an Ultimate Tic Tac Toe game.
///
/// This class wraps the game board and manages:
/// - player turns
/// - move history (undo system)
/// - game state queries (valid moves, terminal state, winner)
class GameState
{
private:

    UltimateBoard _board;

    CellState _myPlayer;
    CellState _opponent;
    CellState _currentPlayer;

public:
    struct MoveUndo
    {
        AIMove move;
        CellState prevPlayer;
        int prevActiveBoard;
        uint64_t prevHash;
    };


     /// @brief Constructs a new game state.
    GameState();

    /// @brief Sets the AI player and initializes opponent.
    /// @param me The AI player symbol.
    void setPlayers(CellState me);

    /// @brief Switches the current player.
    void switchPlayers();

    /// @brief Resets the game to initial state.
    void reset();

    /// @brief Applies a move to the game state.
    /// @param move Move to apply.
    /// @return True if the move was successfully applied.
    bool applyMove(const AIMove& move);

    /// @brief Gets the current player.
    CellState getCurrentPlayer() const;

    /// @brief Gets the game board.
    const UltimateBoard& getBoard() const;

    /// @brief Computes all valid moves for the current state.
    std::vector<AIMove> getValidMoves() const;

    /// @brief Checks if the game is finished.
    /// @return True if the game is in a terminal state.
    bool isTerminal() const;

    /// @brief Gets the player.
    CellState getMyPlayer() const { return _myPlayer; }

    /// @brief Gets the opponent player.
    CellState getOpponent() const { return _opponent; }

    /// @brief Gets the winner of the game.
    /// @return Winning player or EMPTY if no winner.
    CellState getWinner() const;

    MoveUndo applyMoveFast(const AIMove& move);
    void undoMove(const MoveUndo& undo);

    uint64_t getHash() const;

    int getActiveBoard() const;
    void setActiveBoard(int);

    int applyNullMove();
    void undoNullMove(int activeBoard);

    int getMovesLeft() const;



};

