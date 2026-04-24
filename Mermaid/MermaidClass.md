classDiagram

class Game {
    -board: UltimateBoard
    -currentPlayer: int
    +playTurn()
    +isGameOver(): bool
}

class UltimateBoard {
    -subBoards[9]: SubBoard
    +applyMove(move: Move)
    +getValidMoves(): list~Move~
    +checkWin(): int
}

class SubBoard {
    -cells[9]: Cell
    +applyMove(move: Move)
    +getValidMoves(): list~Move~
    +checkWin(): int
}

class Cell {
    -value: int
}

class Move {
    +subBoardIndex: int
    +cellIndex: int
}

class AI {
    -depth: int
    +getBestMove(board: UltimateBoard): Move
}

class Minimax {
    +search(board: UltimateBoard, depth: int, alpha: int, beta: int): int
}

class Heuristic {
    +evaluate(board: UltimateBoard): int
}

class MoveGenerator {
    +getValidMoves(board: UltimateBoard): list~Move~
}

class Timer {
    +start()
    +stop()
    +getMs(): int
}

class Adapter {
    +readState(): UltimateBoard
    +sendMove(move: Move)
}

%% --- Core ---
Game "1" *-- "1" UltimateBoard
UltimateBoard "1" *-- "9" SubBoard
SubBoard "1" *-- "9" Cell

Game ..> Move

%% --- AI ---
AI ..> Minimax
AI ..> MoveGenerator
AI ..> Heuristic
AI ..> Timer
AI ..> UltimateBoard

Minimax ..> Heuristic
Minimax ..> MoveGenerator

%% --- Interface ---
Adapter ..> Game
Adapter ..> UltimateBoard
Adapter ..> Move
