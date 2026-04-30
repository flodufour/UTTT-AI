#include "core/GameState.h"
#include <iostream>

GameState::GameState()
{
    reset();
}

void GameState::reset()
{
    _board.reset();

    while (!_history.empty())
        _history.pop();

    _currentPlayer = CellState::X;
    _myPlayer = CellState::EMPTY;
    _opponent = CellState::EMPTY;
}

void GameState::setPlayers(CellState me)
{
    _myPlayer = me;
    _opponent = (me == CellState::X) ? CellState::O : CellState::X;

    _currentPlayer = CellState::X;
}

bool GameState::applyMove(const AIMove& aIMove)
{
    if (!_board.playMove(aIMove, _currentPlayer))
        return false;

    _history.push({
        aIMove,
        _currentPlayer,
        _board.getActiveBoard()
    });

    switchPlayers();
    return true;
}

void GameState::switchPlayers()
{
    _currentPlayer = (_currentPlayer == CellState::X)
                    ? CellState::O
                    : CellState::X;
}

CellState GameState::getCurrentPlayer() const
{
    return _currentPlayer;
}

const UltimateBoard& GameState::getBoard() const
{
    return _board;
}

std::vector<AIMove> GameState::getValidMoves() const
{
    std::vector<AIMove> moves;

    const UltimateBoard& b = _board;

    int active = b.getActiveBoard();

    for (int boardIndex = 0; boardIndex < 9; boardIndex++)
    {
        if (active != -1 && boardIndex != active)
            continue;

        for (int cellIndex = 0; cellIndex < 9; cellIndex++)
        {
            AIMove m(boardIndex, cellIndex);

            if (b.isValidMove(m))
                moves.push_back(m);
        }
    }

    return moves;
}


bool GameState::isTerminal() const
{
    return _board.checkWinner() != CellState::EMPTY
        || _board.isFull();
}

CellState GameState::getWinner() const
{
    return _board.checkWinner();
}

void GameState::undoMove()
{
    if (_history.empty())
        return;

    StateSnapshot snap = _history.top();
    _history.pop();

    _board.undoMove();
    _board.updateActiveBoard(snap.activeBoard);
    _currentPlayer = snap.currentPlayer;
}
