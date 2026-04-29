#include "ai/evaluate/Evaluation.h"
#include "core/WinPatterns.h"
#include "core/SubBoard.h"

int Evaluation::evaluate(const GameState& state)
{
    // Initializing the score that will be affected to the board
    int score = 0;

    // Getting the board
    const UltimateBoard& b = state.getBoard();

    // Getting players
    CellState me = state.getMyPlayer();
    CellState opp = state.getOpponent();

    // Checks if there is a winner
    int end = checkEndgame(b, me, opp);
    if( end != 0){
        return end;
    }

    // Attributes a score to the meta board
    score += evaluateMetaBoard(b, me);

    // Attributes a score to all the subBoards
    score += evaluateAllSubBoards(b, me, opp);

    return score;
}

int Evaluation::evaluateAllSubBoards(const UltimateBoard& b, CellState me, CellState opp){
    int suboardScores = 0;
    for (int i = 0; i < 9; i++)
    {
        const SubBoard& sb = b.getBoard(i);
        CellState sw = sb.checkWinner();

        if (sw == me)
            suboardScores += 100;
        else if (sw == opp)
            suboardScores -= 100;
        else
            suboardScores += evaluateSubBoard(sb, me);
    }
    return suboardScores;
}

int Evaluation::checkEndgame(const UltimateBoard& b, CellState me, CellState opp){

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

    if (sb.getCell(4).getState() == me) score += 5;
    if (sb.getCell(4).getState() == opp) score -= 5;

    int corners[4] = {0,2,6,8};
    for (int i : corners)
    {
        if (sb.getCell(i).getState() == me) score += 2;
        if (sb.getCell(i).getState() == opp) score -= 2;
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

        if (meCount == 2 && emptyCount == 1) score += 20;
        if (oppCount == 2 && emptyCount == 1) score -= 25;
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

        if (meCount == 2 && oppCount == 0)
            score += 500;

        if (oppCount == 2 && meCount == 0)
            score -= 500;
    }

    const SubBoard& center = b.getBoard(4);
    if (center.checkWinner() == me)
        score += 30;

    return score;
}
