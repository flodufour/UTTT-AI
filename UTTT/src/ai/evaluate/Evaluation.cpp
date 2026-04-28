#include "ai/evaluate/Evaluation.h"

int Evaluation::evaluate(const GameState& state)
{
    const UltimateBoard& b = state.getBoard();

    CellState me = state.getMyPlayer();
    CellState opp = state.getOpponent();

    CellState winner = b.checkWinner();

    if (winner == me)
        return 100000;

    if (winner == opp)
        return -100000;

    int score = 0;

    for (int i = 0; i < 9; i++)
    {
        const SubBoard& sb = b.getBoard(i);
        CellState sw = sb.checkWinner();

        if (sw == me)
            score += 100;
        else if (sw == opp)
            score -= 100;
        else
            score += evaluateSubBoard(sb, me);
    }

    const SubBoard& center = b.getBoard(4);
    if (center.checkWinner() == me)
        score += 30;

    return score;
}

int Evaluation::evaluateSubBoard(const SubBoard& sb, CellState me)
{
    int score = 0;

    if (sb.getCell(4).getState() == me)
        score += 5;

    int corners[4] = {0,2,6,8};

    for (int i : corners)
        if (sb.getCell(i).getState() == me)
            score += 2;

    return score;
}
