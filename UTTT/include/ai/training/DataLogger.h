#pragma once

#include <fstream>
#include <array>
#include <vector>
#include <string>

class DataLogger {
public:
    static constexpr int SIZE = 81;

    DataLogger(const std::string& filename);
    ~DataLogger();

    void setGameId(int gameId);

    void logState(
        const std::array<int, SIZE>& state,
        int board,
        int cell,
        int player,
        int activeBoard
    );

    void setResult(int result);

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
    std::ofstream _file;
    std::vector<Sample> _buffer;

    int _gameId = 0;
    int _result = 0;

    int _moveId = 0;
};
