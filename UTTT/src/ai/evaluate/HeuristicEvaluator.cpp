#include "ai/evaluate/HeuristicEvaluator.h"
#include "core/WinPatterns.h"
#include <algorithm>

int HeuristicEvaluator::evaluate(const GameState& state) const
{
    const UltimateBoard& b = state.getBoard();

    CellState me  = state.getMyPlayer();
    CellState opp = (me == CellState::X) ? CellState::O : CellState::X;


    int terminal = evaluateTerminal(b, me, opp, 0);
    if (terminal != 0)
        return terminal;


    int score = 0;

    score += evaluateMeta(b, me, opp);
    score += evaluateBoards(b, me, opp);
    score += evaluateForcedMove(b, me, opp);

    return score;
}

int HeuristicEvaluator::evaluateTerminal(const UltimateBoard& b,
                                         CellState me,
                                         CellState opp,
                                         int /*ply*/) const
{
    CellState winner = b.checkWinner();

    if (winner == me)
        return W::WIN;

    if (winner == opp)
        return -W::WIN;

    return 0;
}

int HeuristicEvaluator::evaluateMeta(const UltimateBoard& b,
                                     CellState me,
                                     CellState opp) const
{
    int score = 0;

    auto lineScore = [&](CellState c)
    {
        if (c == me)  return W::META_TWO;
        if (c == opp) return -W::META_TWO;
        return 0;
    };

    for (int i = 0; i < 9; i++)
    {
        CellState owner = b.getBoard(i).checkWinner();
        score += lineScore(owner);
    }

    return score;
}

int HeuristicEvaluator::evaluateBoards(const UltimateBoard& b,
                                       CellState me,
                                       CellState opp) const
{
    int score = 0;

    for (int i = 0; i < 9; i++)
    {
        score += evaluateSubBoard(b.getBoard(i), me, opp);
    }

    return score;
}

int HeuristicEvaluator::evaluateSubBoard(const SubBoard& sb,
                                         CellState me,
                                         CellState opp) const
{
    int score = 0;

    auto center = sb.getCell(4).getState();

    if (center == me)  score += W::CENTER;
    if (center == opp) score -= W::CENTER;

    static const int corners[4] = {0, 2, 6, 8};

    for (int i = 0; i < 4; i++)
    {
        auto c = sb.getCell(corners[i]).getState();

        if (c == me)
            score += W::CORNER;
        else if (c == opp)
            score -= W::CORNER;
    }

    CellState w = sb.checkWinner();

    if (w == me)  score += W::SUB_WIN;
    if (w == opp) score -= W::SUB_WIN;

    return score;
}

int HeuristicEvaluator::evaluateForcedMove(const UltimateBoard& b,
                                            CellState me,
                                            CellState opp) const
{
    int active = b.getActiveBoard();

    if (active >= 0 && active < 9)
    {
        const SubBoard& sb = b.getBoard(active);

        if (sb.checkWinner() == me)
            return 20;

        if (sb.checkWinner() == opp)
            return -20;
    }

    return 0;
}

