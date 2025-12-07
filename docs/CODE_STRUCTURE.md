# Code Structure and Architecture

This document explains the organization and architecture of the Tic Tac Toe game.

## Project Overview

The project is organized in a modular, object-oriented style (simulated in C using structs and function pointers). Each module has a clear responsibility with minimal coupling between components.

## File Organization

```
tictactoe.2.c/
├── main.c          # Entry point, main menu, game flow control
├── game.c/h        # Core game logic and board management
├── ai.c/h          # AI opponent with multiple difficulty levels
├── ui.c/h          # User interface and display functions
├── utils.c/h       # Utility functions, file I/O, statistics
├── Makefile        # Build configuration
└── docs/           # Documentation files
```

## Module Breakdown

### 1. main.c - Game Flow Control

**Purpose:** Central hub for game orchestration and menu system

**Key Functions:**
- `main()` - Program entry point with main menu loop
- `playGame()` - Orchestrates player vs AI match
- `playPlayerVsPlayer()` - Orchestrates player vs player match with AI analysis
- `playAIVsAI()` - Orchestrates AI vs AI demonstration match

**Menu Options:**
1. Play vs AI - Single player game
2. Player vs Player with AI Analysis - Two human players with AI commentary
3. Watch AI vs AI - Two AI opponents compete
4. View Leaderboard - Player rankings
5. View All Game Statistics - Complete game history
6. View Player Statistics - Individual player stats
7. Exit - Quit program

**Global Variables:**
- `aiVerbose` - Controls AI explanation verbosity (0=none, 1=brief, 2=full)
- `aiDifficulty` - Current AI difficulty level (0=Easy, 1=Medium, 2=Hard)
- `username` - Current player's name
- `menuChoice` - Main menu selection

### 2. game.c/h - Core Game Logic

**Purpose:** Implements the Tic Tac Toe game rules and board management

**Key Data Structure:**
```c
struct Game {
    char board[3][3];           // 3x3 board: ' ', 'X', 'O'
    void (*display)(Game *);    // Display board function
    void (*makeMove)();         // Make a move function
    int (*checkWin)(Game *);    // Check for win condition
    int (*isMovesLeft)(Game *); // Check if moves available
};
```

**Key Functions:**
- `Game_init(Game *g)` - Initialize game board and function pointers
- `game_display()` - Display the current board state
- `game_makeMove()` - Place a symbol on the board
- `game_checkWin()` - Determine if there's a winner
  - Returns `1` if X wins
  - Returns `-1` if O wins
  - Returns `0` if draw (no moves left, no winner)
  - Returns `2` if game ongoing
- `game_isMovesLeft()` - Check if empty spaces remain

**Board Representation:**
```
Row 0:  board[0][0] | board[0][1] | board[0][2]
Row 1:  board[1][0] | board[1][1] | board[1][2]
Row 2:  board[2][0] | board[2][1] | board[2][2]
```

**Win Conditions:**
- Three in a row (horizontal)
- Three in a column (vertical)
- Three in a diagonal (both directions)

**Win Detection Logic:**
```c
// Returns 1 if X wins, -1 if O wins, 0 if draw, 2 if ongoing
if (board[i][0] != ' ' && board[i][0] == board[i][1] == board[i][2])
    return (board[i][0] == 'X') ? 1 : -1;
```

### 3. ai.c/h - AI Opponent

**Purpose:** Implements multiple difficulty levels using different algorithms

**Key Data Structure:**
```c
struct AI {
    Game *game;                  // Pointer to game state
    Move (*findBestMove)();      // Function pointer for move selection
    int difficulty;              // 0=Easy, 1=Medium, 2=Hard
    int verbose;                 // 0=silent, 1=brief, 2=detailed
};
```

**Difficulty Levels:**

#### Easy (Difficulty 0 - "Kitty")
- **Algorithm:** Random move selection
- **Strategy:** Picks a random valid move
- **Behavior:** Makes mistakes, playable for beginners
- **Speed:** Instant

#### Medium (Difficulty 1 - "Cop")
- **Algorithm:** Mixed minimax with randomness
- **Strategy:** Makes smart moves but with occasional random choices
- **Behavior:** Balanced difficulty, competitive but beatable
- **Speed:** Quick (limited depth search)

#### Hard (Difficulty 2 - "Sera")
- **Algorithm:** Full Minimax with alpha-beta pruning
- **Strategy:** Explores game tree to find optimal moves
- **Behavior:** Unbeatable when playing optimally
- **Speed:** Fast (alpha-beta pruning optimizes search)

**Key Functions:**
- `AI_init(AI *ai, Game *game)` - Initialize AI with game reference
- `AI_findBestMove(AI *ai)` - Return best move based on difficulty
- `AI_explain(AI *ai, AICandidate *out, int maxOut)` - Explain move candidates
- `AI_setDifficulty(AI *ai, int level)` - Change difficulty level
- `AI_setVerbose(AI *ai, int v)` - Control explanation verbosity
- `AI_getStats(AI *ai, int *nodes, int *maxDepth)` - Get performance metrics
- `AI_resetStats(AI *ai)` - Clear performance counters

**Minimax Algorithm (Hard Mode):**
The minimax algorithm recursively evaluates all possible game states:
1. If game is terminal (win/loss/draw), return score
2. If maximizing (AI's turn), pick move with highest score
3. If minimizing (opponent's turn), pick move with lowest score
4. Alpha-beta pruning eliminates unnecessary branches for speed

### 4. ui.c/h - User Interface

**Purpose:** Handles all display and user input

**Key Data Structure:**
```c
typedef struct {
    char aiName[64];            // AI's name (Kitty, Cop, Sera)
    int aiDifficulty;           // Difficulty level
    char username[64];          // Current player name
    AICandidate candidates[9];  // Top move options
    int candidateCount;         // Number of candidates
    char aiThought[256];        // AI's reasoning
    char lastAIComment[256];    // AI's personality comment
    int aiNodesExplored;        // Performance stat
    int aiMaxDepth;             // Search depth achieved
    char statusMessage[256];    // Current status
} UIGameState;
```

**Key Functions:**
- `UI_init()` - Initialize UI system
- `UI_drawGame(Game *g, UIGameState *state, int isPlayerTurn)` - Display game board and state
- `UI_drawGameOver(Game *g, UIGameState *state, const char *result)` - Display end-game screen
- `UI_getPlayerInput(int *row, int *col)` - Get and validate player input (row 0-2, col 0-2)
- `UI_cleanup()` - Clean up UI resources

**Display Features:**
- Clear, formatted 3x3 board with current state
- AI candidate moves with scores
- AI personality comments and quips
- Move validation with error messages
- Performance statistics (nodes explored, search depth)
- Win/loss/draw messages with AI reactions

**Player Input:**
Players enter moves as two integers: `row col`
- Row: 0, 1, or 2 (top to bottom)
- Column: 0, 1, or 2 (left to right)
- Example: `0 1` places a mark at top-middle position

### 5. utils.c/h - Utilities and File I/O

**Purpose:** Data persistence, leaderboard management, and utility functions

**Key Data Structures:**
```c
typedef struct {
    char username[MAX_USERNAME];
    int wins;
    int losses;
    int draws;
    int totalGames;
} PlayerRecord;

typedef struct {
    char player1[MAX_USERNAME];
    char player2[MAX_USERNAME];
    int totalMoves;
    int player1Moves;
    int player2Moves;
    int aiNodesExplored;
    int maxDepth;
    char winner;  // 'X', 'O', or 'D'
} GameStats;
```

**Files Used:**
- `leaderboard.txt` - Stores player rankings and records
- `game_stats.txt` - Stores detailed match statistics

**Key Functions:**

*Leaderboard Management:*
- `savePlayerRecord(const PlayerRecord *record)` - Save/update player record
- `loadPlayerRecord(const char *username, PlayerRecord *record)` - Load player record
- `displayLeaderboard()` - Show ranked player list
- `updateLeaderboard(const char *username, char winner)` - Update after game

*Statistics:*
- `saveGameStats(const GameStats *stats)` - Log game details
- `displayAllStats()` - Show all games played
- `displayPlayerStats(const char *username)` - Show specific player's games

*AI Personality:*
- `getAIName(int difficulty)` - Get AI's name (Kitty/Cop/Sera)
- `getAIQuote(int difficulty, int occasion)` - Get personality comment

*Utilities:*
- `clearScreen()` - Clear terminal display

## Data Flow Diagram

```
main.c
  ├─→ Menu System
  │    └─→ Select Game Mode
  │
  ├─→ Case 1: Play vs AI
  │    ├─→ game.c: Initialize board
  │    ├─→ ai.c: Initialize AI with difficulty
  │    ├─→ Game Loop:
  │    │   ├─→ ui.c: Display board
  │    │   ├─→ ui.c: Get player input (row col)
  │    │   ├─→ game.c: Validate & make move
  │    │   ├─→ game.c: Check win/draw
  │    │   ├─→ ai.c: Calculate best move
  │    │   ├─→ game.c: Apply AI move
  │    │   └─→ game.c: Check win/draw
  │    ├─→ utils.c: Update leaderboard
  │    ├─→ utils.c: Save game stats
  │    └─→ ui.c: Display result
  │
  ├─→ Case 2: Player vs Player
  │    ├─→ game.c: Initialize board
  │    ├─→ ai.c: Initialize for analysis
  │    ├─→ Game Loop (alternating players)
  │    │   ├─→ ui.c: Display board & AI analysis
  │    │   ├─→ ui.c: Get current player input (row col)
  │    │   ├─→ game.c: Validate & make move
  │    │   ├─→ ai.c: Analyze move (optional)
  │    │   └─→ game.c: Check win/draw
  │    ├─→ utils.c: Save game stats
  │    └─→ ui.c: Display result
  │
  ├─→ Case 3: AI vs AI
  │    ├─→ game.c: Initialize board
  │    ├─→ ai.c: Initialize two AIs
  │    ├─→ Game Loop (alternating AIs)
  │    │   ├─→ ui.c: Display board
  │    │   ├─→ ai.c: Calculate move
  │    │   ├─→ game.c: Apply move
  │    │   └─→ game.c: Check win/draw
  │    ├─→ utils.c: Save game stats
  │    └─→ ui.c: Display result
  │
  └─→ Cases 4-6: View Statistics
       └─→ utils.c: Read & display data
```

## Module Dependencies

```
game.h          (no dependencies)
  ↑
  ├─ ai.h       (depends on game.h)
  │   ↑
  │   └─ ui.h   (depends on ai.h, game.h)
  │
  └─ utils.h    (no dependencies)
       ↑
       └─ main.c (depends on all above)
```

## Design Patterns Used

### 1. Object-Oriented Programming in C
Uses structs with function pointers to simulate OOP:
```c
struct Game {
    char board[3][3];
    void (*display)(Game *self);
    int (*checkWin)(Game *self);
};
```

### 2. Strategy Pattern
AI difficulty levels implement different strategies:
- Easy: Random strategy
- Medium: Limited search strategy
- Hard: Full minimax strategy

### 3. State Pattern
`UIGameState` encapsulates the UI's current state and display information

### 4. Singleton Pattern (Implicit)
Game and AI instances are managed by main.c as singletons for each game session

## Memory Management

- **Stack:** Board arrays, game states, local variables
- **Heap:** Dynamically allocated strings for usernames, game logs
- **File I/O:** Persistent data in text files (leaderboard.txt, game_stats.txt)

All dynamic memory is freed before program exit or between game sessions to prevent leaks.

## Error Handling

- Input validation for user choices (rows/columns 0-2)
- File I/O error checking (NULL pointer checks on fopen)
- Buffer overflow protection using safe string functions
- Clear error messages displayed to user when invalid moves are attempted
