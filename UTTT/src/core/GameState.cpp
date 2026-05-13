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

    _currentHash = calculateHash();

}

void GameState::setPlayers(CellState me)
{
    _myPlayer = me;
    _opponent = (me == CellState::X) ? CellState::O : CellState::X;

    _currentPlayer = CellState::X;
}

//========================================================
//                      APPLY & UNDO                  //
//========================================================

bool GameState::applyMove(const AIMove& aIMove)
{

    int oldActive = _board.getActiveBoard();
    CellState playerWhoMoved = _currentPlayer;

    if (!_board.playMove(aIMove, _currentPlayer))
        return false;

    switchPlayers();

    updateHash(aIMove, playerWhoMoved, oldActive, _board.getActiveBoard());

    return true;
}

MoveUndo GameState::applyMoveFast(const AIMove& move)
{
    MoveUndo undo;
    undo.move = move;
    undo.prevActiveBoard = _board.getActiveBoard();
    CellState playerWhoMoved = _currentPlayer;

    if(!_board.playMove(move, _currentPlayer)) {
        undo.move = {-1, -1};
        return undo;
    }

    switchPlayers();

    updateHash(move, playerWhoMoved, undo.prevActiveBoard, _board.getActiveBoard());

//    if (_currentHash != calculateHash()) {
//        std::cerr << "Désynchro Hash après applyMoveFast !" << std::endl;
//    }

    return undo;
}

void GameState::undoMove(const MoveUndo& undo)
{
    int currentActive = _board.getActiveBoard();
    CellState playerWhoJustPlayed = (_currentPlayer == CellState::X) ? CellState::O : CellState::X;


    updateHash(undo.move, playerWhoJustPlayed, currentActive, undo.prevActiveBoard);

    _board.undoMove(undo.move, undo.prevActiveBoard);

    switchPlayers();

//    if (_currentHash != calculateHash()) {
//        std::cerr << "ERREUR : Désynchro Hash dans undoMove !" << std::endl;
//    }
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
    int active = _board.getActiveBoard();

    if (active != -1) {
        for (int cellIndex = 0; cellIndex < 9; cellIndex++) {
            AIMove m(active, cellIndex);
            if (_board.isValidMove(m)) moves.push_back(m);
        }
    } else {
        for (int boardIndex = 0; boardIndex < 9; boardIndex++) {
            for (int cellIndex = 0; cellIndex < 9; cellIndex++) {
                AIMove m(boardIndex, cellIndex);
                if (_board.isValidMove(m)) moves.push_back(m);
            }
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



int GameState::getActiveBoard() const
{
    return _board.getActiveBoard();
}


int GameState::applyNullMove()
{
    switchPlayers();
    int active = _board.getActiveBoard();
    _board.updateActiveBoard(-1);


    return active;
}

bool GameState::undoNullMove(int activeBoard)
{
    if(activeBoard > -2 && activeBoard < 9){
        switchPlayers();
        _board.updateActiveBoard(activeBoard);
        return true;
    }
    return false;
}

int GameState::getMovesLeft() const{
    return _board.getMovesLeftBoard();
}


//========================================================
//                        HASH                          //
//========================================================

uint64_t GameState::getHash() const
{
    return _currentHash;
}


void GameState::updateHash(const AIMove& move, CellState player, int oldActive, int newActive) {
    _currentHash ^= Zobrist::activeBoard[Zobrist::activeBoardIndex(oldActive)];
    _currentHash ^= Zobrist::player[(int)player];

    int pieceIdx = (player == CellState::X) ? 0 : 1;
    _currentHash ^= Zobrist::cell[move.boardIndex][move.cellIndex][pieceIdx];

    _currentHash ^= Zobrist::player[(int)_currentPlayer];
    _currentHash ^= Zobrist::activeBoard[Zobrist::activeBoardIndex(newActive)];
}

uint64_t GameState::calculateHash() const
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
