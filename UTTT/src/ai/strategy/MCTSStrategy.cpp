#include "ai/strategy/MCTSStrategy.h"
#include "utils/ZobristHasher.h"

#include <limits>
#include <random>
#include <algorithm>
#include <iostream>

MCTSStrategy::MCTSStrategy(IEvaluator* evaluator,
                           int iterations,
                           double exploration)
    : _evaluator(evaluator),
      _iterations(iterations),
      _exploration(exploration)
{
}

AIMove MCTSStrategy::chooseMove(GameState& rootState)
{

    // creation of the tree
    Node root(rootState);

    // simulation of _iterations games
    for (int i = 0; i < _iterations; i++)
    {
        // selection of the most promising node
        Node* node = select(&root);

        // if the node has sub-nodes, we explore them
        if (!node->state.isTerminal() && !node->untriedMoves.empty())
        {
            node = expand(node);
        }

        // play the game from the selected start node. result is in [-1,1]
        double result = simulate(node->state);

        // we climb the tree from the bottom and update stats
        backpropagate(node, result);
    }

    Node* best = nullptr;
    int bestVisits = -1;

    // we get the most visited move
    for (auto& child : root.children)
    {
        if (child->visits > bestVisits)
        {
            bestVisits = child->visits;
            best = child.get();
        }
    }

    return best ? best->move : AIMove(-1, -1);
}

MCTSStrategy::Node* MCTSStrategy::select(Node* node)
{

    while (!node->state.isTerminal())
    {
        //  test all moves of the node
        if (!node->untriedMoves.empty())
            return node;

        if (node->children.empty())
            return node;

        Node* best = nullptr;
        double bestScore = -std::numeric_limits<double>::infinity();

        // choose the best scored children
        for (auto& child : node->children)
        {
            double score = uctValue(child.get(), node);

            if (score > bestScore)
            {
                bestScore = score;
                best = child.get();
            }
        }

        if (!best)
            return node;

        node = best;
    }

    return node;
}

MCTSStrategy::Node* MCTSStrategy::expand(Node* node)
{
    if (node->untriedMoves.empty())
        return node;

    static thread_local std::mt19937 rng(std::random_device{}());

    int K = std::min(5, (int)node->untriedMoves.size());

    std::uniform_int_distribution<int> dist(0, node->untriedMoves.size() - 1);

    AIMove bestMove;
    int bestScore = std::numeric_limits<int>::min();

    std::vector<int> used;
    used.reserve(K);

    // sample K unique moves
    for (int i = 0; i < K; i++)
    {
        int idx;
        do {
            idx = dist(rng);
        } while (std::find(used.begin(), used.end(), idx) != used.end());

        used.push_back(idx);

        AIMove move = node->untriedMoves[idx];

        GameState tmp = node->state;
        if (!tmp.applyMove(move))
            continue;

        int score = _evaluator->evaluate(tmp);

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
    }

    // remove chosen move from untried list
    auto it = std::find(node->untriedMoves.begin(),
                         node->untriedMoves.end(),
                         bestMove);

    if (it == node->untriedMoves.end())
        return node;

    node->untriedMoves.erase(it);

    // create child node
    GameState nextState = node->state;

    if (!nextState.applyMove(bestMove))
        return node;

    node->children.push_back(
        std::make_unique<Node>(nextState, node, bestMove)
    );

    return node->children.back().get();
}

double MCTSStrategy::simulate(GameState state)
{
    // gets the root player (our ai)
    CellState rootPlayer = state.getMyPlayer();

    int movesLeft = state.getMovesLeft();

    // smooth depth scaling
    int maxDepth = 20 + std::log(100 - movesLeft + 1) * 5;

    static thread_local std::mt19937 rng(std::random_device{}());

    int depth = 0;

    // simulates the game by applying move after move
    while (!state.isTerminal() && depth < maxDepth)
    {
        auto moves = state.getValidMoves();
        if (moves.empty())
            break;

        AIMove move;

        // noise reduction
        if (state.getMovesLeft() <= 12)
        {
            move = selectRolloutMove(state); // no randomness anymore
        }
        else {
            std::uniform_int_distribution<int> prob(0, 9);
            if (prob(rng) < 8)
            {
                move = selectRolloutMove(state);
            }
            else
            {
                std::uniform_int_distribution<int> dist(0, (int)moves.size() - 1);
                move = moves[dist(rng)];
            }
        }



        if (!state.applyMove(move))
            break;

        depth++;
    }

    // returns reward depending on the winner

    CellState winner = state.getWinner();

    if (winner == rootPlayer)
        return 1.0;
    if (winner != CellState::EMPTY)
        return -1.0;

    int score = _evaluator->evaluate(state);

    return std::tanh(score / 1000.0);
}

double MCTSStrategy::uctValue(Node* node, Node* parent) const
{
    if (node->visits == 0)
        return std::numeric_limits<double>::infinity();

    double exploitation = node->value / node->visits;

    double exploration = _exploration *
        std::sqrt(std::log(parent->visits + 1) / node->visits);

    return exploitation + exploration;
}

uint64_t MCTSStrategy::getHash(const GameState& state) const
{
    return state.getHash();
}

AIMove MCTSStrategy::selectRolloutMove(GameState& state)
{
    auto moves = state.getValidMoves();

    struct ScoredMove {
        AIMove move;
        int score;
    };

    std::vector<ScoredMove> scored;
    scored.reserve(moves.size());

    for (const auto& move : moves)
    {
        auto undo = state.applyMoveFast(move);

        if (undo.move.boardIndex == -1 && undo.move.cellIndex == -1)
            continue;

        int score = _evaluator->evaluate(state);

        scored.push_back({move, score});

        state.undoMove(undo);
    }

    if (scored.empty())
        return moves[0];

    std::sort(scored.begin(), scored.end(),
              [](auto& a, auto& b) {
                  return a.score > b.score;
              });

    int K = std::min(4, (int)scored.size());

    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, K - 1);

    return scored[dist(rng)].move;
}

void MCTSStrategy::backpropagate(Node* node, double result)
{
    while (node)
    {
        node->visits++;
        node->value += result;
        node = node->parent;
    }
}

