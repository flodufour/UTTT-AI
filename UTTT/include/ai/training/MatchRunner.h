#pragma once
#include <vector>

class DataLogger;

class MatchRunner {
public:
    MatchRunner() = default;

    int runMatch(const std::vector<double>& weightsX,
                 const std::vector<double>& weightsO,
                 DataLogger* logger = nullptr,
                 int gameId = 0) const;
};
