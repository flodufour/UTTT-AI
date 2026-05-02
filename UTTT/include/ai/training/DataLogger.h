#pragma once

#include <vector>
#include <string>

class GameState;
class AIMove;

class DataLogger {
public:
    struct Game {
        std::vector<std::vector<int>> states;
        int result;
    };

public:
    DataLogger(const std::string& filename);

    void startGame();
    void addState(const GameState& state);
    void endGame(int result);

    void save();

private:
    std::vector<int> flattenState(const GameState& state);

private:
    std::string _filename;
    Game _currentGame;
    std::vector<Game> _games;
};
