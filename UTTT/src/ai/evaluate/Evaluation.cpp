#include "ai/evaluate/Evaluation.h"
#include "core/WinPatterns.h"
#include "core/SubBoard.h"

int Evaluation::evaluate(const GameState& state)
{
    int score = 0;

    const UltimateBoard& b = state.getBoard();

    CellState me = state.getMyPlayer();
    CellState opp = state.getOpponent();

    int end = checkEndgame(b, me, opp);
    if (end != 0)
        return end;

    int active = b.getActiveBoard();

    if (active != -1)
    {
        const SubBoard& target = b.getBoard(active);

        score += evaluateSubBoard(target, me);
        score -= evaluateSubBoard(target, opp);
    }

    score += evaluateMetaBoard(b, me);

    score += evaluateAllSubBoards(b, me, opp);

    return score;
}

int Evaluation::evaluateAllSubBoards(const UltimateBoard& b, CellState me, CellState opp)
{
    int score = 0;

    for (int i = 0; i < 9; i++)
    {
        const SubBoard& sb = b.getBoard(i);
        CellState sw = sb.checkWinner();

        if (sw == me)
            score += 150;
        else if (sw == opp)
            score -= 150;
        else
            score += evaluateSubBoard(sb, me) * boardWeight[i];
    }

    return score;
}

int Evaluation::checkEndgame(const UltimateBoard& b, CellState me, CellState opp)
{
    CellState winner = b.checkWinner();

    if (winner == me)
        return 100000;

    if (winner == opp)
        return -100000;

    return 0;
}

int Evaluation::evaluateSubBoard(const SubBoard& sb, CellState me)
{
    int score = 0;
    CellState opp = (me == CellState::X) ? CellState::O : CellState::X;

    if (sb.getCell(4).getState() == me) score += 6;
    if (sb.getCell(4).getState() == opp) score -= 6;

    int corners[4] = {0,2,6,8};

    for (int i : corners)
    {
        if (sb.getCell(i).getState() == me) score += 3;
        if (sb.getCell(i).getState() == opp) score -= 3;
    }

    for (const auto& line : WIN_LINES)
    {
        int meCount = 0, oppCount = 0, emptyCount = 0;

        for (int i : line)
        {
            CellState s = sb.getCell(i).getState();

            if (s == me) meCount++;
            else if (s == opp) oppCount++;
            else emptyCount++;
        }

        if (meCount == 2 && emptyCount == 1)
            score += 25;

        if (oppCount == 2 && emptyCount == 1)
            score -= 30;
    }

    return score;
}

int Evaluation::evaluateMetaBoard(const UltimateBoard& b, CellState me)
{
    int score = 0;

    CellState opp = (me == CellState::X) ? CellState::O : CellState::X;

    for (const auto& line : WIN_LINES)
    {
        int meCount = 0;
        int oppCount = 0;

        for (int i : line)
        {
            CellState s = b.getBoard(i).checkWinner();

            if (s == me) meCount++;
            else if (s == opp) oppCount++;
        }

        if (meCount == 3)
            return 100000;

        if (oppCount == 3)
            return -100000;

        if (meCount == 2)
            score += 1500;

        if (oppCount == 2)
            score -= 1800;

        if (meCount == 1 && oppCount == 0)
            score += 150;

        if (oppCount == 1 && meCount == 0)
            score -= 150;
    }

    const SubBoard& center = b.getBoard(4);
    if (center.checkWinner() == me)
        score += 50;

    return score;
}
