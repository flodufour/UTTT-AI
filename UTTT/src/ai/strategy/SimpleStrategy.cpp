#include "ai/strategy/SimpleStrategy.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <cstdlib>

// ------------------------------------------------------------
// CONSTRUCTOR
// ------------------------------------------------------------
MCTSHeuristicStrategy::MCTSHeuristicStrategy(
    std::shared_ptr<IEvaluator> eval,
    std::shared_ptr<MinimaxStrategy> minimax,
    int iterations
)
    : _eval(std::move(eval)),
      _minimax(std::move(minimax)),
      _iterations(iterations)
{
}

// ------------------------------------------------------------
// ROOT CHOICE
// ------------------------------------------------------------
AIMove MCTSHeuristicStrategy::chooseMove(const GameState& state)
{
    auto root = std::make_unique<MCTSNode>();
    root->state = state;

    auto moves = state.getValidMoves();

    if (moves.empty())
        return {-1, -1};

    for (int i = 0; i < _iterations; i++)
    {
        MCTSNode* node = selection(root.get());

        if (!node->state.isTerminal())
        {
            expansion(node);

            if (!node->children.empty())
            {
                node = node->children[
                    rand() % node->children.size()
                ].get();
            }
        }

        double result = simulation(node);

        backpropagation(node, result);
    }

    MCTSNode* bestChild = nullptr;
    double bestScore = -1e18;

    for (auto& child : root->children)
    {
        if (child->visits == 0)
            continue;

        double avg =
            child->totalScore / child->visits;

        if (avg > bestScore)
        {
            bestScore = avg;
            bestChild = child.get();
        }
    }

    if (bestChild)
        return bestChild->moveFromParent;

    return moves[0];
}

// ------------------------------------------------------------
// SELECTION
// ------------------------------------------------------------
MCTSNode* MCTSHeuristicStrategy::selection(MCTSNode* node)
{
    while (!node->children.empty() && node->fullyExpanded)
    {
        double bestUCT = -1e18;
        MCTSNode* bestChild = nullptr;

        for (auto& child : node->children)
        {
            double uct =
                computeUCT(node, child.get());

            if (uct > bestUCT)
            {
                bestUCT = uct;
                bestChild = child.get();
            }
        }

        if (!bestChild)
            break;

        node = bestChild;
    }

    return node;
}

// ------------------------------------------------------------
// EXPANSION
// ------------------------------------------------------------
void MCTSHeuristicStrategy::expansion(MCTSNode* node)
{
    if (node->fullyExpanded)
        return;

    auto moves = node->state.getValidMoves();

    if (moves.empty())
        return;

    // IMPORTANT:
    // on expand TOUS les coups
    // sinon tu rates des winning lines
    for (const auto& move : moves)
    {
        GameState next = node->state;
        next.applyMove(move);

        auto child = std::make_unique<MCTSNode>();
        child->state = next;
        child->parent = node;
        child->moveFromParent = move;

        node->children.push_back(
            std::move(child)
        );
    }

    node->fullyExpanded = true;
}

// ------------------------------------------------------------
// SIMULATION
// ------------------------------------------------------------
double MCTSHeuristicStrategy::simulation(MCTSNode* node)
{
    GameState simState = node->state;

    for (int depth = 0; depth < ROLLOUT_DEPTH; depth++)
    {
        if (simState.isTerminal())
            break;

        auto moves = simState.getValidMoves();

        if (moves.empty())
            break;

        AIMove bestMove = moves[0];
        int bestScore = -100000000;

        for (const auto& move : moves)
        {
            // win instant
            if (isImmediateWinningMove(simState, move))
            {
                bestMove = move;
                bestScore = 100000000;
                break;
            }

            // éviter moves suicidaires
            if (isImmediateLosingMove(simState, move))
                continue;

            int score =
                evaluateMove(simState, move);

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
        }

        simState.applyMove(bestMove);
    }

    return static_cast<double>(
        _eval->evaluate(simState)
    );
}

// ------------------------------------------------------------
// BACKPROPAGATION
// ------------------------------------------------------------
void MCTSHeuristicStrategy::backpropagation(
    MCTSNode* node,
    double result
)
{
    while (node)
    {
        node->visits++;

        // inversion joueur/adversaire
        node->totalScore += result;

        result = -result;

        node = node->parent;
    }
}

// ------------------------------------------------------------
// UCT
// ------------------------------------------------------------
double MCTSHeuristicStrategy::computeUCT(
    MCTSNode* parent,
    MCTSNode* child
)
{
    if (child->visits == 0)
        return std::numeric_limits<double>::infinity();

    double exploitation =
        child->totalScore / child->visits;

    double exploration =
        EXPLORATION *
        std::sqrt(
            std::log(parent->visits + 1.0)
            / child->visits
        );

    return exploitation + exploration;
}

// ------------------------------------------------------------
// IMMEDIATE WIN
// ------------------------------------------------------------
bool MCTSHeuristicStrategy::isImmediateWinningMove(
    const GameState& state,
    const AIMove& move
)
{
    GameState next = state;
    next.applyMove(move);

    return next.isTerminal();
}

// ------------------------------------------------------------
// IMMEDIATE LOSS PREVENTION
// ------------------------------------------------------------
bool MCTSHeuristicStrategy::isImmediateLosingMove(
    const GameState& state,
    const AIMove& move
)
{
    GameState next = state;
    next.applyMove(move);

    auto opponentMoves = next.getValidMoves();

    for (const auto& oppMove : opponentMoves)
    {
        GameState oppState = next;
        oppState.applyMove(oppMove);

        if (oppState.isTerminal())
            return true;
    }

    return false;
}

// ------------------------------------------------------------
// HYBRID EVAL
// ------------------------------------------------------------
int MCTSHeuristicStrategy::evaluateMove(
    const GameState& state,
    const AIMove& move
)
{
    GameState next = state;
    next.applyMove(move);

    if (next.isTerminal())
        return 1000000;

    // early game -> heuristique rapide uniquement
    auto movesCount = next.getValidMoves().size();

    if (movesCount > 8)
    {
        return _eval->evaluate(next);
    }

    // seulement late game / tactique
    if (_minimax)
    {
        AIMove best = _minimax->chooseMove(next);

        GameState future = next;
        future.applyMove(best);

        return _eval->evaluate(future);
    }

    return _eval->evaluate(next);
}
