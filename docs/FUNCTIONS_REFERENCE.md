# Functions Reference

Complete reference for all functions in the project.

## main.c

### Core Game Functions

#### `void playGame(int playerStarts, int aiVerbose, int aiDifficulty, const char *username)`
Orchestrates a complete game between a player and AI opponent.

**Parameters:**
- `playerStarts` - 1 if player goes first, 0 if AI goes first
- `aiVerbose` - Verbosity level: 0=silent, 1=brief, 2=detailed
- `aiDifficulty` - AI strength: 0=Easy, 1=Medium, 2=Hard
- `username` - Player's name for record-keeping

**Flow:**
1. Initialize game board
2. Initialize AI opponent
3. Alternately take turns until game ends
4. Update leaderboard and statistics

**Returns:** void

---

#### `void playPlayerVsPlayer(const char *player1, const char *player2, int aiDifficulty)`
Manages a game between two human players with AI analysis.

**Parameters:**
- `player1` - First player's name (plays as X)
- `player2` - Second player's name (plays as O)
- `aiDifficulty` - AI difficulty for analysis only (doesn't play)

**Flow:**
1. Initialize game and analysis AI
2. Alternate player inputs
3. AI provides move analysis after each turn
4. Save match statistics

**Returns:** void

---

#### `void playAIVsAI(int aiDifficulty1, int aiDifficulty2)`
Demonstrates an automated game between two AI opponents.

**Parameters:**
- `aiDifficulty1` - First AI's difficulty level (plays as X)
- `aiDifficulty2` - Second AI's difficulty level (plays as O)

**Flow:**
1. Create two AI objects with different strategies
2. Alternately call AI move selection
3. Display reasoning and moves
4. Continue until game ends
5. Show performance comparison

**Returns:** void

---

### Leaderboard Functions

#### `void displayLeaderboard()`
Displays ranked player list sorted by win rate.

**Parameters:** None

**Output Format:**
```
Player Name | Total Games | Wins | Losses | Draws | Win Rate
```

**Behavior:**
- Reads `leaderboard.txt`
- Sorts by win count (descending)
- Shows performance metrics
- Handles no-data case gracefully

**Returns:** void

---

#### `void updateLeaderboard(const char *username, char winner)`
Updates a player's record after a game.

**Parameters:**
- `username` - Player to update
- `winner` - Game result: 'X'=player won, 'O'=AI won, 'D'=draw

**Flow:**
1. Load current player record
2. Increment total games
3. Update appropriate counter (wins/losses/draws)
4. Save updated record

**Returns:** void

---

### Statistics Functions

#### `void displayAllStats()`
Prints all games played with full details.

**Parameters:** None

**Output:** 
Each game shows:
- Timestamp
- Player names
- Move counts
- AI performance metrics
- Winner

**Returns:** void

---

#### `void displayPlayerStats(const char *username)`
Prints all games for a specific player.

**Parameters:**
- `username` - Player to view

**Returns:** void

---

#### `void saveGameStats(const GameStats *stats)`
Logs game details to persistent storage.

**Parameters:**
- `stats` - Pointer to GameStats struct containing:
  - Player names
  - Move counts
  - Winner
  - AI performance metrics

**File Updated:** `game_stats.txt`

**Returns:** void

---

### AI Personality Functions

#### `const char *getAIName(int difficulty)`
Returns the AI's name based on difficulty.

**Parameters:**
- `difficulty` - 0=Kitty, 1=Cop, 2=Sera

**Returns:** Pointer to static string (AI's name)

**Names:**
- 0 → "Kitty"
- 1 → "Cop"
- 2 → "Sera"

---

#### `const char *getAIQuote(int difficulty, int occasion)`
Returns contextual personality quotes.

**Parameters:**
- `difficulty` - 0=Kitty, 1=Cop, 2=Sera
- `occasion` - 0=intro, 1=win, 2=lose, 3=draw, 4=thinking

**Returns:** Pointer to static string (personality quote)

**Examples:**
- Kitty intro: "Meow~ Let's play! I'm still learning..."
- Cop win: "Justice served. Better luck next time, citizen."
- Sera hard: "Predictable. Victory was inevitable."

---

## game.c/h

### Game Initialization

#### `void Game_init(Game *g)`
Initializes game board and method function pointers.

**Parameters:**
- `g` - Pointer to Game struct to initialize

**Behavior:**
1. Clear entire 3x3 board (set all cells to ' ')
2. Set function pointers:
   - `g->display = game_display`
   - `g->makeMove = game_makeMove`
   - `g->checkWin = game_checkWin`
   - `g->isMovesLeft = game_isMovesLeft`

**Returns:** void

---

### Game Operations

#### `void game_display(Game *self)`
Displays the current board state.

**Parameters:**
- `self` - Pointer to Game object

**Output Format:**

The board is displayed as a 3x3 grid with separators:
```
-----------
| X |   | O |
-----------
|   | X |   |
-----------
| O |   |   |
-----------
```

Empty cells display a space character, occupied cells display 'X' or 'O'.

**Board Structure:**
- Row 0 (top): board[0][0], board[0][1], board[0][2]
- Row 1 (middle): board[1][0], board[1][1], board[1][2]
- Row 2 (bottom): board[2][0], board[2][1], board[2][2]

**Returns:** void

---

#### `void game_makeMove(Game *self, int row, int col, char symbol)`
Places a symbol (X or O) on the board.

**Parameters:**
- `self` - Pointer to Game object
- `row` - Row (0-2): 0=top, 1=middle, 2=bottom
- `col` - Column (0-2): 0=left, 1=center, 2=right
- `symbol` - 'X' or 'O' to place

**Behavior:**
- Sets board[row][col] = symbol
- No validation (caller must validate)

**Returns:** void

---

#### `int game_checkWin(Game *self)`
Determines if there's a winner on the board.

**Parameters:**
- `self` - Pointer to Game object

**Returns:**
- 1 if X wins (three-in-a-row for X)
- -1 if O wins (three-in-a-row for O)
- 0 if draw (board full, no winner)
- 2 if game ongoing (moves available, no winner)

**Checks:**
1. All 3 rows
2. All 3 columns
3. Both diagonals

**Note:** Returns winner regardless of which player just moved; caller must track turn.

---

#### `int game_isMovesLeft(Game *self)`
Checks if empty cells remain on board.

**Parameters:**
- `self` - Pointer to Game object

**Returns:**
- 1 if at least one empty cell exists
- 0 if board is full

**Use Case:** Determines if game is drawn (no moves left and no winner).

---

## ai.c/h

### AI Initialization

#### `void AI_init(AI *ai, Game *game)`
Initializes AI object and sets default parameters.

**Parameters:**
- `ai` - Pointer to AI struct to initialize
- `game` - Pointer to Game object for AI to analyze

**Behavior:**
1. Store game pointer
2. Set default difficulty (2 = Hard)
3. Set default verbosity (2 = detailed)
4. Initialize stats counters

**Returns:** void

---

### AI Configuration

#### `void AI_setDifficulty(AI *ai, int level)`
Sets AI difficulty level.

**Parameters:**
- `ai` - Pointer to AI object
- `level` - 0=Easy, 1=Medium, 2=Hard

**Effect:** Changes move selection algorithm

**Returns:** void

---

#### `void AI_setVerbose(AI *ai, int v)`
Sets verbosity level for AI explanations.

**Parameters:**
- `ai` - Pointer to AI object
- `v` - 0=silent, 1=brief, 2=detailed

**Returns:** void

---

#### `void AI_resetStats(AI *ai)`
Clears performance metrics before analysis.

**Parameters:**
- `ai` - Pointer to AI object

**Resets:**
- nodesExplored counter
- maxDepth counter

**Use Case:** Before analyzing a new position

**Returns:** void

---

### Move Selection

#### `Move AI_findBestMove(AI *ai)`
Calculates AI's best move for current board state.

**Parameters:**
- `ai` - Pointer to AI object

**Returns:** Move struct with:
- `row` - Suggested row (0-2)
- `col` - Suggested column (0-2)

**Algorithm Depends on Difficulty:**
- **Easy:** Returns random empty position
- **Medium:** Limited minimax (4-5 ply) with 20% randomness
- **Hard:** Full minimax with alpha-beta pruning (searches entire game tree)

**Time Complexity:**
- Easy: O(1)
- Medium: O(b^4) where b ≈ 5 average branching factor
- Hard: O(b^d) with alpha-beta pruning reducing branches

---

#### `int AI_getPrediction(AI *ai)`
Returns AI's evaluation of current position without making a move.

**Parameters:**
- `ai` - Pointer to AI object

**Returns:**
- Positive value: AI advantage
- Negative value: Opponent advantage
- 0: Draw expected

**Use Case:** For analysis without playing

**Returns:** int (score)

---

### Analysis Functions

#### `int AI_explain(AI *ai, AICandidate *out, int maxOut)`
Generates detailed move analysis with candidate moves.

**Parameters:**
- `ai` - Pointer to AI object
- `out` - Output array for candidate moves
- `maxOut` - Maximum number of candidates to return

**Fills Output Array with:**
- `row` - Candidate move row
- `col` - Candidate move column
- `score` - Evaluation score

**Returns:** Number of candidates found (up to maxOut)

**Use Case:** Display top 3 moves with scores to user

---

#### `void AI_getStats(AI *ai, int *nodes, int *maxDepth)`
Retrieves performance metrics from last search.

**Parameters:**
- `ai` - Pointer to AI object
- `nodes` - Pointer to store nodes explored count
- `maxDepth` - Pointer to store maximum depth reached

**Output:**
- `*nodes` - Number of board positions evaluated
- `*maxDepth` - Ply depth (moves ahead) searched

**Use Case:** Display "Considered 4,532 positions, depth 8" to user

**Returns:** void

---

## ui.c/h

### UI System

#### `void UI_init(void)`
Initializes UI system.

**Parameters:** None

**Behavior:**
- Sets up terminal display
- Initializes color/formatting (if terminal supports)

**Returns:** void

---

#### `void UI_cleanup(void)`
Cleans up UI resources.

**Parameters:** None

**Returns:** void

---

### Display Functions

#### `void UI_drawGame(Game *g, UIGameState *state, int isPlayerTurn)`
Displays current game board and state information.

**Parameters:**
- `g` - Pointer to Game object
- `state` - Pointer to UIGameState with display info
- `isPlayerTurn` - 1 if waiting for player input, 0 if AI thinking

**Display Elements:**
1. Game board (3x3 grid)
2. Current turn indicator
3. AI personality comment
4. Top candidate moves (if available)
5. Performance stats (for Hard AI)
6. Status message

**Returns:** void

---

#### `void UI_drawGameOver(Game *g, UIGameState *state, const char *result)`
Displays end-game screen.

**Parameters:**
- `g` - Pointer to Game object
- `state` - Pointer to UIGameState
- `result` - String describing outcome ("You Won!", "Draw!", etc.)

**Display Elements:**
1. Final board
2. Result message
3. AI's reaction quote
4. Game statistics
5. Updated player record

**Returns:** void

---

### Input Functions

#### `int UI_getPlayerInput(int *row, int *col)`
Gets and validates player input (row and column).

**Parameters:**
- `row` - Pointer to store row (0-2)
- `col` - Pointer to store column (0-2)

**Input Process:**
1. Prompt "Enter move (row col): "
2. Read two integers: row and column
3. Validate both are 0-2 and cell is not occupied
4. If invalid, re-prompt internally
5. Fill row and col with valid values

**Returns:**
- 0 if input invalid (clears buffer, re-prompts internally)
- 1 if move valid (row/col filled)

**Validation:**
- Are inputs integers?
- Is 0 ≤ row ≤ 2?
- Is 0 ≤ col ≤ 2?
- Is board[row][col] empty?

---

## utils.c/h

### Leaderboard File I/O

#### `void savePlayerRecord(const PlayerRecord *record)`
Saves or updates a player's record in leaderboard.

**Parameters:**
- `record` - Pointer to PlayerRecord with:
  - username
  - wins, losses, draws, totalGames

**File:** `leaderboard.txt`

**Format:** `username|wins|losses|draws|totalGames`

**Behavior:**
1. Check if username already exists
2. If exists: update line
3. If not: append new line

**Returns:** void

---

#### `int loadPlayerRecord(const char *username, PlayerRecord *record)`
Loads a player's record from leaderboard.

**Parameters:**
- `username` - Player name to look up
- `record` - Pointer to PlayerRecord to fill

**Returns:**
- 1 if player found and loaded
- 0 if player not found (record initialized to 0s)

**Behavior:**
- Searches leaderboard.txt for matching username
- Fills record struct with stats
- If not found: initializes record with 0 wins/losses/draws

**Returns:** int (0=new player, 1=returning player)

---

### Statistics File I/O

#### `void saveGameStats(const GameStats *stats)`
Logs a game's details to statistics file.

**Parameters:**
- `stats` - Pointer to GameStats with game information

**File:** `game_stats.txt`

**Format (appended):** `Timestamp | Match: P1 vs P2 | Moves: N | Winner: X/O/D`

**Behavior:**
- Appends new line to file
- Includes timestamp
- Records all game details

**Returns:** void

---

#### `void displayAllStats()`
Displays all games from statistics file.

**Parameters:** None

**Returns:** void

---

#### `void displayPlayerStats(const char *username)`
Displays all games for a specific player.

**Parameters:**
- `username` - Player to search for

**Returns:** void

---

### Utility Functions

#### `void clearScreen()`
Clears the terminal display.

**Parameters:** None

**Behavior:**
- On Unix/Linux/macOS: calls `system("clear")`
- On Windows: calls `system("cls")`

**Returns:** void

---

### AI Personality

#### `const char *getAIName(int difficulty)`
Returns AI name string.

**Parameters:**
- `difficulty` - 0, 1, or 2

**Returns:** "Kitty", "Cop", or "Sera"

---

#### `const char *getAIQuote(int difficulty, int occasion)`
Returns personality-appropriate quote.

**Parameters:**
- `difficulty` - 0=Kitty, 1=Cop, 2=Sera
- `occasion` - 0=intro, 1=win, 2=lose, 3=draw, 4=thinking

**Returns:** Static string (char*)

---

## Data Structures

### Game
```c
struct Game {
    char board[3][3];
    void (*display)(Game *self);
    void (*makeMove)(Game *self, int row, int col, char symbol);
    int (*checkWin)(Game *self);
    int (*isMovesLeft)(Game *self);
};
```

### AI
```c
struct AI {
    Game *game;
    Move (*findBestMove)(AI *self);
    int difficulty;
    int verbose;
};
```

### Move
```c
typedef struct {
    int row;
    int col;
} Move;
```

### PlayerRecord
```c
typedef struct {
    char username[MAX_USERNAME];
    int wins;
    int losses;
    int draws;
    int totalGames;
} PlayerRecord;
```

### GameStats
```c
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

### AICandidate
```c
typedef struct {
    int row;
    int col;
    int score;
} AICandidate;
```

### UIGameState
```c
typedef struct {
    char aiName[64];
    int aiDifficulty;
    char username[64];
    AICandidate candidates[9];
    int candidateCount;
    char aiThought[256];
    char lastAIComment[256];
    int aiNodesExplored;
    int aiMaxDepth;
    char statusMessage[256];
} UIGameState;
```
