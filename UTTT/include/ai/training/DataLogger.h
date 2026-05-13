#pragma once

#include <fstream>
#include <array>
#include <vector>
#include <string>

/// @class DataLogger
/// @brief Handles recording of game states and moves for dataset generation.
///
/// This class buffers game samples during a match and flushes them to a file
/// (JSONL format) once the game result is known.
class DataLogger {
public:
    static constexpr int SIZE = 81;

    /// @brief Constructs a logger and opens the output file.
    /// @param filename Path to the output log file.
    DataLogger(const std::string& filename);

    /// @brief Destructor that ensures all remaining data is flushed.
    ~DataLogger();

    /// @brief Sets context for the current game session.
    /// @param gameId Unique identifier for the game.
    /// @param runTimeStamp Unix timestamp identifying the execution run.
    void setGameId(int gameId, long long runTimeStamp);

    /// @brief Captures a single game state and the corresponding move made.
    /// @param state Array representing the 81 cells of the board.
    /// @param board Index of the sub-board where the move occurred.
    /// @param cell Index of the cell within the sub-board.
    /// @param player The player who made the move (1 for X, 2 for O).
    /// @param activeBoard The active board constraint at the time of the move.
    void logState(
        const std::array<int, SIZE>& state,
        int board,
        int cell,
        int player,
        int activeBoard
    );

    /// @brief Sets the final outcome of the game.
    /// @param result Winner ID (0: Draw, 1: X wins, 2: O wins).
    void setResult(int result);

    /// @brief Serializes buffered samples and writes them to disk.
    void flush();

private:

    struct Sample {
        std::array<int, SIZE> state;
        int board = 0;
        int cell = 0;
        int player = 0;
        int activeBoard = -1;
        int moveId = 0;
    };

private:
    long long _runTimeStamp;
    std::ofstream _file;
    std::vector<Sample> _buffer;

    int _gameId = 0;
    int _result = 0;

    int _moveId = 0;
};
