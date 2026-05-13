#pragma once

#include "core/UltimateBoard.h"
#include "utils/MoveUndo.h"
#include "main.h"
#include <vector>
#include <stack>
#include <cstdint>


/// @class GameState
/// @brief Manages the full state and logic of an Ultimate Tic Tac Toe game.
///
/// This class acts as the central engine controller, handling:
/// - Turn management and player roles
/// - Move validation and application (including fast undo support)
/// - Incremental Zobrist hashing for efficient transposition table lookups
/// - Game status queries (terminal state detection, winner)
class GameState
{
private:

    UltimateBoard _board;

    CellState _myPlayer;
    CellState _opponent;
    CellState _currentPlayer;

    uint64_t _currentHash = 0;

public:

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

    /// @brief Applies a move for search algorithms
    /// @param move The move to apply
    /// @return A MoveUndo object containing AIMove and previousboardIndex needed to revert the move
    MoveUndo applyMoveFast(const AIMove& move);

    /// @brief Reverts the state to a previous point using undo data
    /// @param undo The data structure returned by applyMoveFast
    void undoMove(const MoveUndo& undo);

    /// @brief Fully recomputes the Zobrist hash from scratch
    /// @return The calculated hash
    uint64_t calculateHash() const;

    /// @brief Gets the index of the board where the current player must play
    /// @return Board index (0-8) or -1 if the player has a "free move"
    int getActiveBoard() const;

    /// @brief Executes a "pass" move for pruning techniques (Not implemented yet)
    /// @return The active board index before the move was applied
    int applyNullMove();

    /// @brief Reverts a previously applied null move
    /// @param activeBoard The active board index to restore
    /// @return True if the revert was successful
    bool undoNullMove(int activeBoard);

    /// @brief Checks the number of moves remaining in the game
    /// @return Count of empty cells that are still playable
    int getMovesLeft() const;

    /// @brief Gets the current incremental Zobrist hash
    /// @return Current hash value
    uint64_t getHash() const;

    /// @brief Incrementally updates the hash after a move
    /// @param move The move that was made
    /// @param player The player who made the move
    /// @param oldActive The active board index before the move
    /// @param newActive The active board index after the move
    void updateHash(const AIMove& move, CellState player, int oldActive, int newActive);



};

