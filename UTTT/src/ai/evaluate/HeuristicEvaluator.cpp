#include "ai/evaluate/HeuristicEvaluator.h"
#include "core/WinPatterns.h"
#include <algorithm>

int HeuristicEvaluator::evaluate(const GameState& state) const
{
    const UltimateBoard& b = state.getBoard();

    CellState me  = state.getMyPlayer();
    CellState opp = (me == CellState::X) ? CellState::O : CellState::X;


    int terminal = evaluateTerminal(b, me, opp);
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
                                         CellState opp) const
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

     int myThreats = 0;
    int oppThreats = 0;

    for (int i = 0; i < 9; i++)
    {
        CellState owner = b.getBoard(i).checkWinner();

        if (owner == me)
            score += boardWeight[i] * 25;
        else if (owner == opp)
            score -= boardWeight[i] * 25;
    }

    for (const auto& line : WIN_LINES)
    {
        int myCount = 0;
        int oppCount = 0;
        int emptyCount = 0;



        for (int i = 0; i < 3; i++)
        {
            CellState owner = b.getBoard(line[i]).checkWinner();

            if (owner == me)
                myCount++;
            else if (owner == opp)
                oppCount++;
            else
                emptyCount++;
        }

        if (myCount > 0 && oppCount > 0)
            continue;

        if (myCount == 2 && emptyCount == 1){
             score += W::META_TWO;
            myThreats++;
        }

        else if (myCount == 1 && emptyCount == 2)
            score += W::META_ONE;

        if (oppCount == 2 && emptyCount == 1){
            score -= W::META_TWO;
            oppThreats++;
        }

        else if (oppCount == 1 && emptyCount == 2)
            score -= W::META_ONE;


        }
        if (myThreats >= 3)
            score += 900;
        else if (myThreats >= 2)
            score += 450;

        if (oppThreats >= 3)
            score -= 1100;
        else if (oppThreats >= 2)
            score -= 500;

    return score;
}

int HeuristicEvaluator::evaluateBoards(const UltimateBoard& b,
                                       CellState me,
                                       CellState opp) const
{
    int score =0;
    for (int i = 0; i < 9; i++)
    {
        const SubBoard& sb = b.getBoard(i);

        if (sb.checkWinner() == CellState::EMPTY && !sb.isFull())
            score += evaluateSubBoard(sb, me, opp);
    }

    return score;
}

int HeuristicEvaluator::evaluateSubBoard(const SubBoard& sb,
                                         CellState me,
                                         CellState opp) const
{
    int score = 0;

    int myThreats = 0;
    int oppThreats = 0;

    for (int i = 0; i < 9; i++)
    {
        CellState owner = sb.getCell(i).getState();

        if (owner == me)
            score += boardWeight[i] * 5;
        else if (owner == opp)
            score -= boardWeight[i] * 5;
    }

    for (const auto& line : WIN_LINES)
    {
        int myCount = 0;
        int oppCount = 0;
        int emptyCount = 0;

        for (int i = 0; i < 3; i++)
        {
            CellState owner = sb.getCell(line[i]).getState();

            if (owner == me)
                myCount++;
            else if (owner == opp)
                oppCount++;
            else
                emptyCount++;
        }

        if (myCount > 0 && oppCount > 0)
            continue;

        if (myCount == 2 && emptyCount == 1)
        {
            score += W::SUB_TWO;
            myThreats++;
        }
        else if (myCount == 1 && emptyCount == 2)
        {
            score += W::SUB_ONE;
        }

        if (oppCount == 2 && emptyCount == 1)
        {
            score -= W::SUB_TWO;
            oppThreats++;
        }
        else if (oppCount == 1 && emptyCount == 2)
        {
            score -= W::SUB_ONE;
        }
    }

    if (myThreats >= 2)
        score += 60;
    else if (myThreats == 1)
        score += 15;

    if (oppThreats >= 2)
        score -= 70;
    else if (oppThreats == 1)
        score -= 20;

    return score;
}

int HeuristicEvaluator::evaluateForcedMove(const UltimateBoard& b,
                                            CellState me,
                                            CellState opp) const
{
    int boardIndex = b.getActiveBoard();

    if (boardIndex == -1)
        return W::FREE_MOVE;

    const SubBoard& sb = b.getBoard(boardIndex);

    if (sb.checkWinner() != CellState::EMPTY || sb.isFull())
        return W::FREE_MOVE;

    int score = 0;

    for (const auto& line : WIN_LINES)
    {
        int myCount = 0;
        int oppCount = 0;
        int emptyCount = 0;

        for (int i = 0; i < 3; i++)
        {
            CellState owner = sb.getCell(line[i]).getState();

            if (owner == me)
                myCount++;
            else if (owner == opp)
                oppCount++;
            else
                emptyCount++;
        }

        if (myCount > 0 && oppCount > 0)
            continue;

        if (myCount == 2 && emptyCount == 1)
            score += W::FORCED_VERY_GOOD;
        else if (myCount == 1 && emptyCount == 2)
            score += W::FORCED_GOOD;

        if (oppCount == 2 && emptyCount == 1)
            score -= W::FORCED_VERY_BAD;
        else if (oppCount == 1 && emptyCount == 2)
            score -= W::FORCED_BAD;
    }

    score += boardWeight[boardIndex] * 3;

    return score;
}

