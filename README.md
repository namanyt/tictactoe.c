# Tic Tac Toe Game

A comprehensive Tic Tac Toe implementation in C featuring multiple game modes, AI opponents with configurable difficulty levels, player statistics tracking, and a leaderboard system.

## Features

- **Game Modes**
  - Player vs AI with three difficulty levels
  - Player vs Player with AI analysis
  - AI vs AI (watch two AIs compete)

- **AI Difficulty Levels**
  - **Easy (Kitty):** Random move selection
  - **Medium (Cop):** Limited minimax search with balanced play
  - **Hard (Sera):** Full minimax with alpha-beta pruning (unbeatable)

- **Player Statistics & Leaderboard**
  - Win/loss/draw tracking per player
  - Player rankings sorted by win rate
  - Complete game history with timestamps
  - Performance metrics for AI analysis

- **User Interface**
  - Clear board display
  - Real-time AI candidate move analysis
  - AI personality quotes and commentary
  - Performance statistics (nodes explored, search depth)

## Quick Start

### Building

```bash
make
```

See [BUILD_INSTRUCTIONS.md](docs/BUILD_INSTRUCTIONS.md) for detailed build information.

### Running

```bash
./tictactoe
```

Follow the interactive menu to choose your game mode.

## Documentation

The `docs/` directory contains comprehensive documentation:

### For Users
- **[BUILD_INSTRUCTIONS.md](docs/BUILD_INSTRUCTIONS.md)** - How to compile and run the program
  - Step-by-step build process
  - Makefile targets and options
  - Troubleshooting guide

### For Developers
- **[CODE_STRUCTURE.md](docs/CODE_STRUCTURE.md)** - Overview of project architecture
  - Module breakdown and responsibilities
  - Data structures
  - Design patterns used
  - Module dependencies

- **[GAME_LOOP.md](docs/GAME_LOOP.md)** - Detailed game execution flow
  - Main program flow diagram
  - Player vs AI game loop step-by-step
  - Player vs Player game loop
  - AI vs AI game loop
  - AI move selection algorithms (Easy, Medium, Hard)
  - Input validation process
  - Win condition checking

- **[FUNCTIONS_REFERENCE.md](docs/FUNCTIONS_REFERENCE.md)** - Complete function documentation
  - All public functions with parameters and return values
  - Data structure definitions
  - Usage examples

## Project Structure

```
tictactoe.c/
├── main.c              # Entry point, menu system, game orchestration
├── game.c/h            # Core game logic and board management
├── ai.c/h              # AI opponent (Easy/Medium/Hard)
├── ui.c/h              # User interface and display
├── utils.c/h           # Utilities, file I/O, leaderboard
├── Makefile            # Build configuration
├── docs/
│   ├── BUILD_INSTRUCTIONS.md      # How to build
│   ├── CODE_STRUCTURE.md          # Architecture overview
│   ├── GAME_LOOP.md               # Execution flow details
│   └── FUNCTIONS_REFERENCE.md     # Function documentation
└── README.md           # This file
```

## Game Modes

### 1. Player vs AI

Play against one of three AI opponents with different strategies:

- **Kitty (Easy):** Makes random moves, good for learning
- **Cop (Medium):** Plays strategically but makes some mistakes
- **Sera (Hard):** Uses minimax algorithm, nearly unbeatable

The game shows AI's candidate moves with scores, allowing you to learn from its analysis.

**Dialogue:** Each AI has personality-based commentary that reacts to the game state.

### 2. Player vs Player with AI Analysis

Two human players compete while an AI analyzes each move in real-time. The AI provides:
- Recommended moves with evaluation scores
- Strategic commentary
- Performance metrics

Perfect for learning Tic Tac Toe strategy together.

### 3. AI vs AI

Watch two AI opponents compete automatically. Shows:
- Move selections from each AI
- Reasoning for each move
- Performance metrics (nodes explored, search depth)
- Comparison of different strategies

Great for understanding how different difficulty levels play.

### 4. Leaderboard

View rankings of all players sorted by win rate:
```
Player Name | Total Games | Wins | Losses | Draws | Win Rate
```

### 5. Game Statistics

View complete game history with:
- Timestamps
- Player names
- Move counts
- Winners
- AI performance metrics (for AI-played games)

## How to Play

### Board Layout

The board is displayed as:
```
    0 | 1 | 2
   -----------
    3 | 4 | 5
   -----------
    6 | 7 | 8
```

Where the numbers represent positions. When moves are made, X or O replaces the numbers.

### Making a Move

Enter your move as **two integers: row and column**

- **Row:** 0 (top), 1 (middle), or 2 (bottom)
- **Column:** 0 (left), 1 (center), or 2 (right)

**Examples:**
- `0 0` - Top-left corner
- `1 1` - Center
- `2 2` - Bottom-right corner
- `0 1` - Top-middle

The position must be empty to place your mark.

## Building from Source

### Requirements

- GCC compiler
- Make
- Standard C library

### Build Commands

```bash
# Compile the project
make

# Clean build artifacts (but keeps player data)
make clean

# Clean build (fresh compilation)
make clean && make

# Run the program
./tictactoe
```

For detailed build instructions, see [BUILD_INSTRUCTIONS.md](docs/BUILD_INSTRUCTIONS.md).

## Understanding the AI

### Easy Difficulty (Kitty)

- **Algorithm:** Random move selection
- **Strategy:** Picks any valid move randomly
- **Strength:** Good for beginners, makes obvious mistakes
- **Speed:** Instant

### Medium Difficulty (Cop)

- **Algorithm:** Limited minimax search (4-5 moves ahead) with randomness
- **Strategy:** 
  - Wins immediately if possible
  - Blocks opponent's winning moves
  - Seeks winning opportunities
  - Otherwise uses limited lookahead with 20% randomness
- **Strength:** Competitive, beatable with good play
- **Speed:** Quick (under 1 second per move)

### Hard Difficulty (Sera)

- **Algorithm:** Full minimax with alpha-beta pruning
- **Strategy:** Explores entire game tree to find optimal moves
  - Guarantees best move every turn
  - Never loses to imperfect play
- **Strength:** Unbeatable (draws or wins every game)
- **Speed:** Fast (alpha-beta pruning optimizes search)

### Minimax Algorithm

The Hard AI uses minimax—a recursive algorithm that:
1. Evaluates every possible future game state
2. Scores positions: +10 for AI win, -10 for AI loss, 0 for draw
3. Maximizes AI's advantage while assuming opponent minimizes it
4. Uses alpha-beta pruning to skip irrelevant branches
5. Returns the move with the best score

This guarantees the AI plays optimally.

## Data Files

The program creates two files to persist player data:

### leaderboard.txt
Stores player records with format:
```
username|wins|losses|draws|totalGames
```

### game_stats.txt
Stores detailed match statistics (one per line):
```
Timestamp | Match: Player1 vs Player2 | Moves: N | ...
```

Both files are updated automatically when games complete.

## Code Architecture

The project uses modular design with object-oriented principles simulated in C:

- **game.c/h:** Pure game logic (no UI, no AI decision-making)
- **ai.c/h:** AI strategies (independent of UI, works with game state)
- **ui.c/h:** Display and input (works with game and AI states)
- **utils.c/h:** File I/O and utilities (leaderboard, statistics)
- **main.c:** Orchestration layer (connects all modules)

This separation allows:
- Easy testing of individual components
- Reusing code in different contexts
- Clear responsibility for each module
- Minimal coupling between components

For more details, see:
- [CODE_STRUCTURE.md](docs/CODE_STRUCTURE.md) - Architecture overview
- [GAME_LOOP.md](docs/GAME_LOOP.md) - Execution flow
- [FUNCTIONS_REFERENCE.md](docs/FUNCTIONS_REFERENCE.md) - Function documentation

## License

MIT License - See [LICENSE](LICENSE) file for details
