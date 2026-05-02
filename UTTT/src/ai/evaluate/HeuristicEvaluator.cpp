#include "ai/evaluate/HeuristicEvaluator.h"
#include "core/WinPatterns.h"
#include "core/SubBoard.h"
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

    score += evaluateMeta(b, me, opp);
    score += evaluateBoards(b, me, opp);
    score += evaluateForcedMove(b, me, opp);

    return std::clamp(score, -10000, 10000);
}

int HeuristicEvaluator::evaluateMeta(const UltimateBoard& b, CellState me, CellState opp)
{
    int score = 0;

    for (const auto& line : WIN_LINES)
    {
        int meScore = 0;
        int oppScore = 0;

        for (int i : line)
        {
            CellState s = b.getBoard(i).checkWinner();

            int weight = _w.boardWeight[i];

            if (s == me) meScore += weight;
            else if (s == opp) oppScore += weight;
        }

        // win condition
        if (meScore >= 12) return _w.WIN;
        if (oppScore >= 12) return -_w.WIN;

        int diff = meScore - oppScore;

        if (diff > 0)
            score += _w.META_TWO * diff / 4;
        else if (diff < 0)
            score += _w.META_TWO * diff / 4; // diff négatif => pénalise automatiquement
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

        if (w == me) score += _w.SUB_WIN;
        else if (w == opp) score -= _w.SUB_WIN;
        else
            score += evaluateSubBoard(sb, me);
    }

    return score;
}
int HeuristicEvaluator::evaluateSubBoard(const SubBoard& sb, CellState me)
{
    int score = 0;

    CellState opp = (me == CellState::X) ? CellState::O : CellState::X;

    if (sb.getCell(4).getState() == me) score += _w.CENTER;
    if (sb.getCell(4).getState() == opp) score -= _w.CENTER;

    int corners[4] = {0,2,6,8};

    for (int i : corners)
    {
        if (sb.getCell(i).getState() == me) score += 3;
        if (sb.getCell(i).getState() == opp) score -= 3;
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
            score += _w.SUB_TWO;

        if (oppCount == 2 && empty == 1)
            score -= _w.SUB_TWO;
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

    int oppPotential = evaluateSubBoard(sb, opp);
    int myPotential  = evaluateSubBoard(sb, me);

    int score = 0;

    score -= oppPotential * 5;
    score += myPotential * 2;

    if (oppPotential >= _w.SUB_TWO)
        score -= _w.FORCED_BAD;

    return score;
}

int HeuristicEvaluator::evaluateTerminal(const UltimateBoard& b, CellState me, CellState opp)
{
    CellState w = b.checkWinner();

    if (w == me) return _w.WIN;
    if (w == opp) return -_w.WIN;

    return 0;
}

HeuristicEvaluator::HeuristicEvaluator(const Weights& w)
{
    _w = w;
}
