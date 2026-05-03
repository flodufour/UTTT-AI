#pragma once
#include "IStrategy.h"
#include "ai/evaluate/IEvaluator.h"
#include <chrono>

class MinimaxStrategy : public IStrategy {
public:
    struct ScoredMove {
    AIMove move;
    int score;
    };

    MinimaxStrategy(IEvaluator* eval);

    AIMove chooseMove(const GameState& state) override;

    std::vector<ScoredMove> getTopMoves(const GameState& state);
private:

    int minimax(GameState state, int depth, int alpha, int beta, bool maximizing);

    int _topK = 3;

    IEvaluator* _eval;

    std::chrono::steady_clock::time_point _start;
    int _maxTimeMs = 350;

    bool isTimeUp();

    int heuristicScore(const GameState& state, const AIMove& move);
};
