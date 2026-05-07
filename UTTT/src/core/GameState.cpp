#include "core/GameState.h"
#include <iostream>
#include "utils/ZobristHasher.h"


GameState::GameState()
{
    reset();
}

void GameState::reset()
{
    _board.reset();


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

GameState::MoveUndo GameState::applyMoveFast(const AIMove& move)
{
    MoveUndo undo;

    undo.move = move;
    undo.prevPlayer = _currentPlayer;
    undo.prevActiveBoard = _board.getActiveBoard();

    if(!_board.playMove(move, _currentPlayer)){
        undo.move = {-1, -1};
    }

    return undo;
}

void GameState::undoMove(const MoveUndo& undo)
{
    _board.undoMove(undo.move, undo.prevActiveBoard);
    _currentPlayer = undo.prevPlayer;
}

uint64_t GameState::getHash() const
{

    uint64_t h = 0;

    for (int b = 0; b < 9; b++)
    {
        for (int c = 0; c < 9; c++)
        {
            CellState s = _board.getBoard(b).getCell(c).getState();

            if (s == CellState::X){

                h ^= Zobrist::cell[b][c][0];            }
            else if (s == CellState::O){

                h ^= Zobrist::cell[b][c][1];            }
        }
    }

    int activeBoard = _board.getActiveBoard();
    if(activeBoard == -1){
        activeBoard =9;
    }
    h ^= Zobrist::activeBoard[Zobrist::activeBoardIndex(activeBoard)];
    h ^= Zobrist::player[(int)_currentPlayer];
    return h;
}

int GameState::getActiveBoard() const
{
    return _board.getActiveBoard();
}

void GameState::setActiveBoard(int b)
{
    _board.setActiveBoard(b);
}


int GameState::applyNullMove()
{
    switchPlayers();
    int active = _board.getActiveBoard();
    setActiveBoard(-1);


    return active;
}

void GameState::undoNullMove(int activeBoard)
{
    switchPlayers();
    setActiveBoard(activeBoard);
}

int GameState::getMovesLeft() const{
    return _board.getMovesLeftBoard();
}
