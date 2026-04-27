#include "core/GameState.h"
#include <iostream>

GameState::GameState()
{
    reset();
}

void GameState::reset()
{
    board.reset();
}

void GameState::setPlayers(CellState me)
{
    myPlayer = me;
    opponent = (me == CellState::X) ? CellState::O : CellState::X;

    currentPlayer = CellState::X;
}

void GameState::applyMove(const GameMove& gameMove)
{
    board.playMove(gameMove, currentPlayer);
}

void GameState::switchPlayers()
{
    currentPlayer = (currentPlayer == CellState::X)
                    ? CellState::O
                    : CellState::X;
}

CellState GameState::getCurrentPlayer() const
{
    return currentPlayer;
}

const UltimateBoard& GameState::getBoard() const
{
    return board;
}

std::vector<GameMove> GameState::getValidMoves() const
{
    std::vector<GameMove> moves;

    const UltimateBoard& b = board;

    int active = b.getActiveBoard();

    for (int boardIndex = 0; boardIndex < 9; boardIndex++)
    {
        if (active != -1 && boardIndex != active)
            continue;

        for (int cellIndex = 0; cellIndex < 9; cellIndex++)
        {
            GameMove m(boardIndex, cellIndex);

            if (b.isValidMove(m))
                moves.push_back(m);
        }
    }

    return moves;
}
