#include "ai/training/DataLogger.h"

DataLogger::DataLogger(const std::string& filename)
    : _file(filename, std::ios::out | std::ios::app)
{
}

DataLogger::~DataLogger()
{
    if (_file.is_open())
        _file.close();
}

void DataLogger::setGameId(int gameId, long long runTimeStamp)
{
    _runTimeStamp = runTimeStamp;
    _gameId = gameId;
    _moveId = 0;
}

void DataLogger::logState(
    const std::array<int, SIZE>& state,
    int board,
    int cell,
    int player,
    int activeBoard
) {
    _buffer.push_back({
        state,
        board,
        cell,
        player,
        activeBoard,
        _moveId++
    });
}

void DataLogger::setResult(int result)
{
    _result = result;
}

void DataLogger::flush()
{
    if (!_file.is_open())
        return;

    for (const auto& s : _buffer)
    {
        _file << "{";

        _file << "\"game_id\":" << _gameId << "_" << _runTimeStamp << ",";
        _file << "\"move_id\":" << s.moveId << ",";

        _file << "\"state\":[";
        for (int i = 0; i < SIZE; ++i)
        {
            _file << s.state[i];
            if (i + 1 < SIZE)
                _file << ",";
        }
        _file << "],";

        _file << "\"action\":["
              << s.board << ","
              << s.cell << "],";

        _file << "\"player\":" << s.player << ",";
        _file << "\"active_board\":" << s.activeBoard << ",";
        _file << "\"result\":" << _result;

        _file << "}\n";
    }

    _file.flush();
    _buffer.clear();
}
