#pragma once

#include "ai/strategy/IStrategy.h"
#include "ai/evaluate/IEvaluator.h"
#include "ai/strategy/MinimaxStrategy.h"
#include "core/GameState.h"
#include "core/AIMove.h"

#include <memory>
#include <vector>

struct MCTSNode
{
    GameState state;
    MCTSNode* parent = nullptr;
    std::vector<std::unique_ptr<MCTSNode>> children;

    AIMove moveFromParent;

    int visits = 0;
    double totalScore = 0.0;

    bool fullyExpanded = false;
};

class MCTSHeuristicStrategy : public IStrategy
{
public:
    MCTSHeuristicStrategy(
        std::shared_ptr<IEvaluator> eval,
        std::shared_ptr<MinimaxStrategy> minimax,
        int iterations = 5000
    );

    AIMove chooseMove(const GameState& state) override;

private:
    std::shared_ptr<IEvaluator> _eval;
    std::shared_ptr<MinimaxStrategy> _minimax;
    int _iterations;

    static constexpr double EXPLORATION = 1.41;
    static constexpr int ROLLOUT_DEPTH = 12;
    static constexpr int MINIMAX_DEPTH = 3;

private:
    MCTSNode* selection(MCTSNode* node);
    void expansion(MCTSNode* node);
    double simulation(MCTSNode* node);
    void backpropagation(MCTSNode* node, double result);

    double computeUCT(MCTSNode* parent, MCTSNode* child);

    bool isImmediateWinningMove(
        const GameState& state,
        const AIMove& move
    );

    bool isImmediateLosingMove(
        const GameState& state,
        const AIMove& move
    );

    int evaluateMove(
        const GameState& state,
        const AIMove& move
    );
};
