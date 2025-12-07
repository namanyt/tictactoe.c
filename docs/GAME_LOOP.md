# Game Loop and Flow Control

This document explains how the game executes from start to finish.

## Main Program Flow

```
Program Start
    ↓
main() initialization
    ├─ Display title and welcome message
    ├─ Set default AI parameters (verbose=2, difficulty=2)
    └─ Enter main menu loop
        ↓
    ┌─────────────────────────────────────────┐
    │ MAIN MENU                               │
    │ 1. Play Game (vs AI)                    │
    │ 2. Player vs Player (with AI Analysis)  │
    │ 3. Watch AI vs AI                       │
    │ 4. View Leaderboard                     │
    │ 5. View All Game Statistics             │
    │ 6. View My Statistics                   │
    │ 7. Exit                                 │
    └─────────────────────────────────────────┘
        ↓
    User Input → Switch Statement
        ├─→ [Case 1] playGame()
        ├─→ [Case 2] playPlayerVsPlayer()
        ├─→ [Case 3] playAIVsAI()
        ├─→ [Case 4] displayLeaderboard()
        ├─→ [Case 5] displayAllStats()
        ├─→ [Case 6] displayPlayerStats()
        └─→ [Case 7] Exit Program
```

## Case 1: Player vs AI Game Loop

```
playGame(playerStarts, aiVerbose, aiDifficulty, username)
    ↓
1. INITIALIZATION
    ├─ Create Game object
    ├─ Initialize game board (all empty)
    ├─ Create AI object
    ├─ Set AI difficulty level
    ├─ Set AI verbosity
    ├─ Determine starting player (X=human, O=AI)
    ├─ Load player's previous record (if exists)
    ├─ Display welcome message with AI quote
    └─ Initialize GameStats structure
        ↓
2. GAME LOOP (while game not over)
    │
    ├─→ Check previous turn's win condition
    │    ├─ Call game.checkWin()
    │    ├─ Check result:
    │    │  ├─ If 1 (X wins): player won, break loop
    │    │  ├─ If -1 (O wins): AI won, break loop
    │    │  └─ If 0 (draw): no moves left, break loop
    │    └─ If 2: game continues
    │
    ├─→ DISPLAY GAME STATE
    │    ├─ Call UI_drawGame()
    │    │  ├─ Display current board
    │    │  ├─ Show AI analysis (if available)
    │    │  └─ Display AI personality comment
    │
    ├─→ PLAYER'S TURN (if current player is human)
    │    ├─ Prompt "Enter move (row col): "
    │    ├─ Call UI_getPlayerInput()
    │    │  ├─ Get input: two integers (row col)
    │    │  ├─ Validate:
    │    │  │  ├─ Are they numbers?
    │    │  │  ├─ Is 0 ≤ row ≤ 2?
    │    │  │  ├─ Is 0 ≤ col ≤ 2?
    │    │  │  └─ Is board[row][col] empty?
    │    │  ├─ If invalid: display error, re-prompt
    │    │  └─ If valid: return 1, filled row/col
    │    ├─ Call game.makeMove(row, col, 'X')
    │    │  └─ Place 'X' at board[row][col]
    │    ├─ Increment X move counter
    │    └─ Update display
    │
    ├─→ Check win condition (same as above)
    │
    ├─→ AI'S TURN (if game continues and it's AI's turn)
    │    ├─ Display "Thinking..." message
    │    ├─ Call AI_findBestMove()
    │    │  └─ Algorithm depends on difficulty:
    │    │     ├─ Easy: random_move()
    │    │     ├─ Medium: limited_minimax() + randomness
    │    │     └─ Hard: full_minimax_with_alpha_beta()
    │    ├─ AI calculates best move: returns Move(row, col)
    │    ├─ If verbose: Call AI_explain() for reasoning
    │    │  └─ Display top 3 candidate moves with scores
    │    ├─ Call game.makeMove(row, col, 'O')
    │    │  └─ Place 'O' at board[row][col]
    │    ├─ Increment O move counter
    │    ├─ Display "Placed at (row, col)" message
    │    └─ Update display
    │
    ├─→ Check win condition (same as above)
    │
    └─→ LOOP CONTINUES until game over (winner found or draw)
        ↓
3. GAME OVER
    ├─ Call UI_drawGameOver()
    │  ├─ Display final board
    │  ├─ Show result: "You Won!", "AI WINS!", or "IT'S A DRAW!"
    │  └─ Display appropriate AI reaction quote
    ├─ Save game statistics
    │  └─ Call saveGameStats()
    │     └─ Log: player names, move counts, winner, AI performance
    ├─ Update leaderboard
    │  └─ Call updateLeaderboard()
    │     ├─ Load player record
    │     ├─ Update wins/losses/draws based on result
    │     └─ Save updated record
    ├─ Display final statistics
    │  └─ Show player's updated record
    └─ Return to main menu
```

## Case 2: Player vs Player Game Loop

```
playPlayerVsPlayer(player1, player2, aiDifficulty)
    ↓
1. INITIALIZATION
    ├─ Create Game object and initialize board
    ├─ Create AI object for analysis (not playing)
    ├─ Load both players' records (if exist)
    ├─ Alternate turns: Player1=X, Player2=O
    └─ Initialize GameStats for match record
        ↓
2. GAME LOOP (similar to player vs AI, but two humans)
    │
    ├─→ CURRENT PLAYER'S TURN
    │    ├─ Call UI_drawGame() with AI analysis
    │    │  └─ Show what the AI would recommend
    │    ├─ Call AI_explain() to get move candidates
    │    │  └─ AI analyzes position without playing
    │    ├─ Display recommendations
    │    ├─ Prompt current player for input (row col)
    │    ├─ Validate and apply move
    │    └─ Update move counter
    │
    ├─→ CHECK GAME STATE
    │    ├─ Check for winner (same logic as before)
    │    ├─ Check for draw
    │    └─ Decide if game continues
    │
    ├─→ SWITCH PLAYERS
    │    └─ Alternate X ↔ O
    │
    └─→ LOOP until game over
        ↓
3. GAME OVER
    ├─ Display final board
    ├─ Show result
    ├─ Save game statistics
    └─ Return to main menu
```

## Case 3: AI vs AI Game Loop

```
playAIVsAI(aiDifficulty1, aiDifficulty2)
    ↓
1. INITIALIZATION
    ├─ Create Game object
    ├─ Create two AI objects with different difficulties
    ├─ AI1 plays as 'X', AI2 plays as 'O'
    ├─ Display match setup
    └─ Initialize GameStats
        ↓
2. GAME LOOP (automated, no human input)
    │
    ├─→ AI1'S TURN (plays as X)
    │    ├─ Call AI1_findBestMove()
    │    ├─ Display move and reasoning
    │    ├─ Apply move to board
    │    └─ Check win/draw
    │
    ├─→ AI2'S TURN (plays as O)
    │    ├─ Call AI2_findBestMove()
    │    ├─ Display move and reasoning
    │    ├─ Apply move to board
    │    └─ Check win/draw
    │
    └─→ LOOP until game over
        ↓
3. GAME OVER
    ├─ Display final board and result
    ├─ Show performance stats (nodes explored, depth reached)
    ├─ Compare AI strategies
    └─ Return to main menu
```

## Detailed AI Move Selection (Case 1 & 3)

### Easy Difficulty (Kitty)

```
AI_findBestMove (Easy)
    ↓
1. Get all empty positions on board
2. Pick random position from empty list
3. Return Move(row, col)
4. Result: Unpredictable, often makes mistakes
```

### Medium Difficulty (Cop)

```
AI_findBestMove (Medium)
    ↓
1. Check if AI can win in 1 move
   └─ If yes, play that move immediately
2. Check if opponent can win in 1 move
   └─ If yes, block it
3. Check if can create winning opportunity
   └─ If yes, play that
4. Otherwise:
   └─ Use limited minimax (depth 4-5)
   └─ With occasional random choice (20% probability)
5. Return best move
6. Result: Plays defensively and strategically, but not perfectly
```

### Hard Difficulty (Sera)

```
AI_findBestMove (Hard)
    ↓
1. For each empty position on board:
   ├─ Simulate placing AI move there
   ├─ Call minimax_with_alpha_beta_pruning()
   │  ├─ If game over: return terminal score
   │  │  ├─ AI wins: +10
   │  │  ├─ Opponent wins: -10
   │  │  └─ Draw: 0
   │  ├─ If maximizing (AI turn):
   │  │  ├─ Try each move recursively
   │  │  ├─ Keep track of best (max) score
   │  │  ├─ If max_score >= beta: prune remaining
   │  │  └─ Return max_score
   │  └─ If minimizing (opponent turn):
   │     ├─ Try each move recursively
   │     ├─ Keep track of worst (min) score for AI
   │     ├─ If min_score <= alpha: prune remaining
   │     └─ Return min_score
   ├─ Undo simulation
   └─ Track this move's score
2. Return move with highest score
3. Result: Plays optimally, unbeatable
```

## Input Validation Flow

```
UI_getPlayerInput()
    ↓
Prompt "Enter move (row col): "
    ↓
Get two integers from user
    ↓
Validate: Are they numbers?
    ├─ NO: Return 0 (invalid)
    └─ YES: Continue
    ↓
Validate: Is 0 ≤ row ≤ 2?
    ├─ NO: Display error, re-prompt
    └─ YES: Continue
    ↓
Validate: Is 0 ≤ col ≤ 2?
    ├─ NO: Display error, re-prompt
    └─ YES: Continue
    ↓
Check: Is board[row][col] empty?
    ├─ NO: Display error, re-prompt
    └─ YES: Return 1 (valid), row/col filled
```

## Win Condition Check

```
game_checkWin()
    ↓
Check all rows (3 checks)
    └─ For each row i: if board[i][0] == board[i][1] == board[i][2] != ' '
       └─ Return (board[i][0] == 'X') ? 1 : -1
    ↓
Check all columns (3 checks)
    └─ For each col j: if board[0][j] == board[1][j] == board[2][j] != ' '
       └─ Return (board[0][j] == 'X') ? 1 : -1
    ↓
Check diagonals (2 checks)
    ├─ Top-left to bottom-right: board[0][0] == board[1][1] == board[2][2] != ' '
    └─ Top-right to bottom-left: board[0][2] == board[1][1] == board[2][0] != ' '
       └─ Return (board[0][0] == 'X') ? 1 : -1
    ↓
If board is full (no moves left)
    └─ Return 0 (draw)
    ↓
If no winner found and moves remain
    └─ Return 2 (game ongoing)
```

## Board Display and Input

The game board is displayed with empty cells showing the actual symbol at that position (either X, O, or a space character). Players do not see position numbers on the board itself.

**Board Layout Reference:**

The internal board structure uses indices [row][col] where:
```
board[0][0] | board[0][1] | board[0][2]    (Row 0: top)
-----------+-----------+-----------
board[1][0] | board[1][1] | board[1][2]    (Row 1: middle)
-----------+-----------+-----------
board[2][0] | board[2][1] | board[2][2]    (Row 2: bottom)

Col:   0        1          2
      (left)  (center)   (right)
```

**Example Game State:**

```
-----------
| X |   | O |
-----------
|   | X |   |
-----------
| O |   |   |
-----------
```

In this example:
- X is at position [0][0] (row 0, col 0) - top-left
- O is at position [0][2] (row 0, col 2) - top-right
- X is at position [1][1] (row 1, col 1) - center
- O is at position [2][0] (row 2, col 0) - bottom-left

**Player Input:**

To place a mark, the player enters two integers separated by a space:
```
row col
```

Where:
- **row**: 0 (top), 1 (middle), or 2 (bottom)
- **col**: 0 (left), 1 (center), or 2 (right)

**Input Examples:**
- `0 0` - Place at top-left
- `0 1` - Place at top-center
- `0 2` - Place at top-right
- `1 0` - Place at middle-left
- `1 1` - Place at center
- `1 2` - Place at middle-right
- `2 0` - Place at bottom-left
- `2 1` - Place at bottom-center
- `2 2` - Place at bottom-right

## State Transitions

```
MAIN_MENU
    ├─→ [1] PLAYING_VS_AI → GAME_OVER_VS_AI → LEADERBOARD_UPDATED → MAIN_MENU
    ├─→ [2] PLAYING_PVP → GAME_OVER_PVP → STATS_SAVED → MAIN_MENU
    ├─→ [3] PLAYING_AI_VS_AI → GAME_OVER_AI_VS_AI → STATS_SAVED → MAIN_MENU
    ├─→ [4] VIEWING_LEADERBOARD → MAIN_MENU
    ├─→ [5] VIEWING_ALL_STATS → MAIN_MENU
    ├─→ [6] VIEWING_PLAYER_STATS → MAIN_MENU
    └─→ [7] EXIT_PROGRAM
```

## File I/O During Game

### On Game Start
1. Load player record from `leaderboard.txt` (if player vs AI)
2. Display player's stats

### During Game
- Display stats in UI
- No file writes until game ends

### On Game End
1. Write to `game_stats.txt`:
   - Timestamp, player names, move counts, winner, AI performance
2. Update `leaderboard.txt`:
   - Increment wins/losses/draws for relevant player(s)
   - Recalculate rankings

## Performance Metrics

During Hard AI moves, the program tracks:
- **Nodes Explored:** How many board positions the minimax algorithm evaluated
- **Max Depth:** How many moves ahead the AI planned

These are displayed to show:
- AI's computational effort
- Search space size
- Effectiveness of alpha-beta pruning
