# Chess (C++ CLI)

## Description
This is a chess game implemented in C++ using object-oriented programming.
The project is currently a CLI (command-line interface) version and supports basic two-player gameplay.


---

## Features
- Two-player mode (local)
- Simple board representation (using standard output)
- Separate class for each chess piece
- Move validation
- Check detection
- Checkmate detection
- Castling support
- En Passant support
- Pin detection (prevents illegal moves that expose your king to check)

---

## Project structure
Each chess piece is implemented in its own file:

- Piece.cpp (Base class for all chess pieces)
- Pawn.cpp 
- Knight.cpp 
- Bishop.cpp 
- Rook.cpp 
- Queen.cpp 
- King.cpp 
- Board.cpp (Implements chess board logic)
- Game_flow.cpp (Handles game flow)
- Main.cpp (Entry point of the application)

---

## Build and run

To compile and run the project:

```bash
g++ -Iinclude -std=c++17 main.cpp src/*.cpp -o chess
./chess
```

---

## How to play

The game is controlled via simple text commands in the terminal

Only command is for moving one piece from one square to another one.

Every piece has its own identifier:

- King: 'K'
- Queen: 'q'
- Rook: 'r'
- Knight: 'k'
- Bishop: 'b'
- Pawn: 'p'

Moves must be written in coordinate format:

- File: a-h
- Rank: 1-8

Format of Command:

[piece_identifer][from] [to]

Example:
```bash
qd1 d4
kg1 f3
bc1 f4
```

---

## Future Improvements

- GUI version
- Rules implementation (Pawn promotion, stalemate, 50-move rule, insufficient mating material rule, draw offer)
- Performance optimizations

---

## Known Issues
- No GUI yet
- Some rules not fully implemented
- In some cases, checkmate is recognised one turn after it is supposed to, when checkmated player wants to make a move

---

## Project Status
Active development (v0.2.0)

## Author
Created by: Igor Milovanovic
