#include "ai/evaluate/HeuristicEvaluator.h"
#include "core/WinPatterns.h"
#include <algorithm>


int HeuristicEvaluator::evaluate(const GameState& state)
{
    const auto& b = state.getBoard();

    CellState me = state.getMyPlayer();
    CellState opp = state.getOpponent();

    int terminal = evaluateTerminal(b, me, opp);
    if (terminal != 0)
        return terminal;

    int score = 0;

    score += evaluateMeta(b, me, opp) * 4;
    score += evaluateBoards(b, me, opp) * 2;
    score += evaluateForcedMove(b, me, opp);

    return std::clamp(score, -100000, 100000);
}


int HeuristicEvaluator::evaluateMeta(const UltimateBoard& b, CellState me, CellState opp)
{
    int score = 0;

    for (const auto& line : WIN_LINES)
    {
        int meCount = 0, oppCount = 0;

        for (int i : line)
        {
            CellState s = b.getBoard(i).checkWinner();

            if (s == me) meCount++;
            else if (s == opp) oppCount++;
        }

        if (meCount == 3) return PositionalWeights::WIN;
        if (oppCount == 3) return -PositionalWeights::WIN;

        if (meCount == 2 && oppCount == 0)
            score += PositionalWeights::META_TWO;

        if (oppCount == 2 && meCount == 0)
            score -= PositionalWeights::META_TWO;

        if (meCount == 1 && oppCount == 0)
            score += PositionalWeights::META_ONE;

        if (oppCount == 1 && meCount == 0)
            score -= PositionalWeights::META_ONE;
    }

    return score;
}


int HeuristicEvaluator::evaluateBoards(const UltimateBoard& b, CellState me, CellState opp)
{
    int score = 0;

    for (int i = 0; i < 9; i++)
    {
        const auto& sb = b.getBoard(i);

        CellState w = sb.checkWinner();

        if (w == me)
            score += PositionalWeights::SUB_WIN;

        else if (w == opp)
            score -= PositionalWeights::SUB_WIN;

        else
            score += evaluateSubBoard(sb, me);
    }

    return score;
}


int HeuristicEvaluator::evaluateSubBoard(const SubBoard& sb, CellState me)
{
    int score = 0;

    CellState opp = (me == CellState::X) ? CellState::O : CellState::X;

    if (sb.getCell(4).getState() == me)
        score += PositionalWeights::CENTER;

    if (sb.getCell(4).getState() == opp)
        score -= PositionalWeights::CENTER;

    int corners[4] = {0, 2, 6, 8};

    for (int i : corners)
    {
        if (sb.getCell(i).getState() == me)
            score += PositionalWeights::CORNER;

        if (sb.getCell(i).getState() == opp)
            score -= PositionalWeights::CORNER;
    }

    for (const auto& line : WIN_LINES)
    {
        int meCount = 0, oppCount = 0, empty = 0;

        for (int i : line)
        {
            CellState s = sb.getCell(i).getState();

            if (s == me) meCount++;
            else if (s == opp) oppCount++;
            else empty++;
        }

        if (meCount == 2 && empty == 1)
            score += PositionalWeights::SUB_TWO;

        if (oppCount == 2 && empty == 1)
            score -= PositionalWeights::SUB_TWO;
    }

    return score;
}


int HeuristicEvaluator::evaluateForcedMove(const UltimateBoard& b, CellState me, CellState opp)
{
    int active = b.getActiveBoard();

    if (active == -1)
        return 0;

    const SubBoard& sb = b.getBoard(active);

    if (!sb.isPlayable())
        return 0;

    int myPotential  = evaluateSubBoard(sb, me);
    int oppPotential  = evaluateSubBoard(sb, opp);

    int score = (myPotential - oppPotential);

    if (oppPotential > PositionalWeights::SUB_TWO * 2)
        score -= PositionalWeights::FORCED_BAD;

    return score;
}


int HeuristicEvaluator::evaluateTerminal(const UltimateBoard& b, CellState me, CellState opp)
{
    CellState w = b.checkWinner();

    if (w == me) return PositionalWeights::WIN;
    if (w == opp) return -PositionalWeights::WIN;

    return 0;
}


int HeuristicEvaluator::checkEndgame(const UltimateBoard& b, CellState me, CellState opp)
{
    return 0;
}
