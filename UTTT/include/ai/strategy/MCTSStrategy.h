
#pragma once

#include "ai/strategy/IStrategy.h"
#include "core/GameState.h"
#include "core/AIMove.h"
#include "ai/evaluate/IEvaluator.h"

#include <vector>
#include <memory>
#include <unordered_map>
#include <cmath>

class MCTSStrategy : public IStrategy
{
public:
    MCTSStrategy(IEvaluator* evaluator,
                 int iterations = 10000,
                 double exploration = 1.4);

    AIMove chooseMove(GameState& state) override;

private:
    struct TTEntry
    {
        double value = 0.0;
        int visits = 0;
    };

    struct Node
    {
        GameState state;
        AIMove move;
        Node* parent = nullptr;

        std::vector<std::unique_ptr<Node>> children;
        std::vector<AIMove> untriedMoves;

        int visits = 0;
        double value = 0.0;

        Node(const GameState& s, Node* p = nullptr, AIMove m = AIMove(-1, -1))
            : state(s), parent(p), move(m)
        {
            untriedMoves = state.getValidMoves();
        }

        bool isFullyExpanded() const
        {
            return untriedMoves.empty();
        }

        bool isLeaf() const
        {
            return children.empty();
        }
    };

private:
    Node* select(Node* node);
    Node* expand(Node* node);
    double simulate(GameState state);
    void backpropagate(Node* node, double result);

    double uctValue(Node* node, Node* parent) const;

    uint64_t getHash(const GameState& state) const;

    IEvaluator* _evaluator;

    int _iterations;
    double _exploration;

    std::unordered_map<uint64_t, TTEntry> _tt;
    AIMove selectRolloutMove(GameState& state);

};
