/*
 * main.c
 * 
 * Main entry point and game orchestration for Tic-Tac-Toe
 * Implements menu system and coordinates all three game modes:
 * - Player vs AI
 * - Player vs Player (with AI analysis)
 * - AI vs AI
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ai.h"
#include "game.h"
#include "ui.h"
#include "utils.h"
#include <time.h>

/* Function prototypes */
void playGame(int playerStarts, int aiVerbose, int aiDifficulty,
              const char *username);
void playPlayerVsPlayer(const char *player1, const char *player2,
                        int aiDifficulty);
void playAIVsAI(int aiDifficulty1, int aiDifficulty2);

/**
 * main - Program entry point and main menu loop
 * 
 * Displays main menu with options:
 * 1. Play vs AI
 * 2. Player vs Player with AI analysis
 * 3. Watch AI vs AI
 * 4. View Leaderboard
 * 5. View All Game Statistics
 * 6. View My Statistics  
 * 7. Exit
 * 
 * Returns: 0 on successful exit
 */
int main() {
  int choice = 0;
  int aiVerbose = 2;      // Default: full verbosity (prediction + details)
  int aiDifficulty = 2;   // Default: Hard difficulty
  char username[MAX_USERNAME];
  int menuChoice = 0;

  // Display title
  printf("========================================\n");
  printf("   TIC-TAC-TOE AI\n");
  printf("========================================\n\n");

  // Main menu loop
  do {
    printf("\n===== MAIN MENU =====\n");
    printf("1. Play Game (vs AI)\n");
    printf("2. Player vs Player (with AI Analysis)\n");
    printf("3. Watch AI vs AI\n");
    printf("4. View Leaderboard\n");
    printf("5. View All Game Statistics\n");
    printf("6. View My Statistics\n");
    printf("7. Exit\n");
    printf("Enter choice: ");
    
    // Get and validate menu input
    if (scanf("%d", &menuChoice) != 1) {
      while (getchar() != '\n')
        ; // Clear input buffer
      menuChoice = 0;
      continue;
    }
    while (getchar() != '\n')
      ; // Clear input buffer

    switch (menuChoice) {
    case 1:
      /* ===== CASE 1: PLAY GAME VS AI ===== */
      // Get player username
      printf("\nEnter your username: ");
      if (fgets(username, MAX_USERNAME, stdin) != NULL) {
        username[strcspn(username, "\n")] = '\0'; // Remove newline
      }

      // Load player record
      PlayerRecord playerRec;
      int isReturning = loadPlayerRecord(username, &playerRec);
      if (isReturning) {
        printf("\nWelcome back, %s!\n", username);
        printf("Your record: %d Wins, %d Losses, %d Draws (%d total games)\n",
               playerRec.wins, playerRec.losses, playerRec.draws,
               playerRec.totalGames);
      } else {
        printf("\nWelcome, %s! This is your first game.\n", username);
      }

      // Select AI difficulty
      printf("\nChoose your opponent:\n");
      printf("  0. Kitty (Easy) - Playful and learning\n");
      printf("  1. Cop (Medium) - Fair and strategic\n");
      printf("  2. Sera (Hard) - Ruthless perfectionist\n");
      printf("Select difficulty (0-2): ");
      if (scanf("%d", &aiDifficulty) != 1 || aiDifficulty < 0 ||
          aiDifficulty > 2)
        aiDifficulty = 2;
      while (getchar() != '\n')
        ; // Clear input buffer

      // Display AI's intro quote
      printf("\n%s says: \"%s\"\n", getAIName(aiDifficulty),
             getAIQuote(aiDifficulty, 0));

      // Choose who starts (X goes first)
      printf("\nChoose who starts: (1) Player X  (2) AI O : ");
      if (scanf("%d", &choice) != 1)
        choice = 1;
      while (getchar() != '\n')
        ; // Clear input buffer

      // Start the game
      playGame(choice == 1, aiVerbose, aiDifficulty, username);
      break;

    case 2:
      /* ===== CASE 2: PLAYER VS PLAYER WITH AI ANALYSIS ===== */
      // Get player 1 username
      printf("\nPlayer 1 username: ");
      if (fgets(username, MAX_USERNAME, stdin) != NULL) {
        username[strcspn(username, "\n")] = '\0';
      }
      
      // Get player 2 username
      char player2[MAX_USERNAME];
      printf("Player 2 username: ");
      if (fgets(player2, MAX_USERNAME, stdin) != NULL) {
        player2[strcspn(player2, "\n")] = '\0';
      }

      // Select AI difficulty for analysis
      printf("\nSelect AI difficulty for analysis:\n");
      printf("  0. Kitty (Easy)\n");
      printf("  1. Cop (Medium)\n");
      printf("  2. Sera (Hard)\n");
      printf("Select (0-2): ");
      if (scanf("%d", &aiDifficulty) != 1 || aiDifficulty < 0 ||
          aiDifficulty > 2)
        aiDifficulty = 2;
      while (getchar() != '\n')
        ;

      printf("\n%s will analyze each move...\n\n", getAIName(aiDifficulty));
      sleep(1);

      // Start player vs player game
      playPlayerVsPlayer(username, player2, aiDifficulty);
      break;

    case 3:
      /* ===== CASE 3: AI VS AI ===== */
      // Select first AI difficulty
      printf("\nAI #1 (X):\n");
      printf("  0. Kitty (Easy)\n");
      printf("  1. Cop (Medium)\n");
      printf("  2. Sera (Hard)\n");
      printf("Select (0-2): ");
      if (scanf("%d", &aiDifficulty) != 1 || aiDifficulty < 0 ||
          aiDifficulty > 2)
        aiDifficulty = 2;
      while (getchar() != '\n')
        ;

      // Select second AI difficulty
      int aiDifficulty2;
      printf("\nAI #2 (O):\n");
      printf("  0. Kitty (Easy)\n");
      printf("  1. Cop (Medium)\n");
      printf("  2. Sera (Hard)\n");
      printf("Select (0-2): ");
      if (scanf("%d", &aiDifficulty2) != 1 || aiDifficulty2 < 0 ||
          aiDifficulty2 > 2)
        aiDifficulty2 = 2;
      while (getchar() != '\n')
        ;

      printf("\n%s (X) vs %s (O) - Starting...\n\n", getAIName(aiDifficulty),
             getAIName(aiDifficulty2));
      sleep(1);

      // Start AI vs AI game
      playAIVsAI(aiDifficulty, aiDifficulty2);
      break;

    case 4:
      /* ===== CASE 4: VIEW LEADERBOARD ===== */
      displayLeaderboard();
      break;

    case 5:
      /* ===== CASE 5: VIEW ALL GAME STATISTICS ===== */
      displayAllStats();
      break;

    case 6:
      /* ===== CASE 6: VIEW MY STATISTICS ===== */
      printf("\nEnter username to view stats: ");
      if (fgets(username, MAX_USERNAME, stdin) != NULL) {
        username[strcspn(username, "\n")] = '\0';
        displayPlayerStats(username);
      }
      break;

    case 7:
      /* ===== CASE 7: EXIT ===== */
      printf("\nThanks for playing! Goodbye!\n");
      break;

    default:
      printf("Invalid choice. Please try again.\n");
      break;
    }

  } while (menuChoice != 7);

  return 0;
}

/**
 * playGame - Orchestrate a player vs AI game
 * @playerStarts: 1 if player goes first (as X), 0 if AI goes first
 * @aiVerbose: AI verbosity level (0=silent, 1=brief, 2=detailed)
 * @aiDifficulty: AI difficulty (0=Easy, 1=Medium, 2=Hard)
 * @username: Player's username for record keeping
 * 
 * Game flow:
 * 1. Initialize game board and AI
 * 2. Alternate turns between player and AI
 * 3. Check for win/draw after each move
 * 4. Update leaderboard and save statistics
 */
void playGame(int playerStarts, int aiVerbose, int aiDifficulty,
              const char *username) {
  Game g;
  AI ai;
  UIGameState uiState;
  GameStats matchStats;

  // Initialize game components
  Game_init(&g);
  AI_init(&ai, &g);
  AI_setDifficulty(&ai, aiDifficulty);
  AI_setVerbose(&ai, aiVerbose);

  // Initialize UI state
  strncpy(uiState.username, username, 63);
  uiState.username[63] = '\0';
  strncpy(uiState.aiName, getAIName(aiDifficulty), 63);
  uiState.aiName[63] = '\0';
  uiState.aiDifficulty = aiDifficulty;
  strcpy(uiState.aiThought, getAIQuote(aiDifficulty, 4));
  strcpy(uiState.lastAIComment, "");
  uiState.candidateCount = 0;
  uiState.aiNodesExplored = 0;
  uiState.aiMaxDepth = 0;

  // Initialize game stats
  strncpy(matchStats.player1, username, MAX_USERNAME - 1);
  matchStats.player1[MAX_USERNAME - 1] = '\0';
  strncpy(matchStats.player2, getAIName(aiDifficulty), MAX_USERNAME - 1);
  matchStats.player2[MAX_USERNAME - 1] = '\0';
  matchStats.totalMoves = 0;
  matchStats.player1Moves = 0;
  matchStats.player2Moves = 0;
  matchStats.aiNodesExplored = 0;
  matchStats.maxDepth = 0;
  matchStats.winner = 'D';

  UI_init();

  // Determine starting turn (0=player, 1=AI)
  int turn = playerStarts ? 0 : 1;
  int nodes = 0, maxDepth = 0;

  // Main game loop
  while (1) {
    // Generate AI analysis if not first turn
    if (matchStats.totalMoves > 0) {
      AICandidate cand[9];
      int candN = AI_explain(&ai, cand, 9);
      uiState.candidateCount = candN;
      for (int i = 0; i < candN; i++) {
        uiState.candidates[i] = cand[i];
      }

      // Get AI stats
      AI_getStats(&ai, &nodes, &maxDepth);
      uiState.aiNodesExplored = nodes;
      uiState.aiMaxDepth = maxDepth;
    } else {
      // First turn - no analysis needed
      uiState.candidateCount = 0;
      uiState.aiNodesExplored = 0;
      uiState.aiMaxDepth = 0;
    }

    // Draw game state
    UI_drawGame(&g, &uiState, turn == 0);
    UI_drawGame(&g, &uiState, turn == 0);

    // Check win conditions
    int state = g.checkWin(&g);
    if (state == 1) {
      // X wins (player)
      snprintf(uiState.lastAIComment, 255, "%s", getAIQuote(aiDifficulty, 2));
      UI_drawGameOver(&g, &uiState, "YOU WIN!");
      matchStats.winner = 'X';
      getchar(); // Wait for enter
      break;
    }
    if (state == -1) {
      // O wins (AI)
      snprintf(uiState.lastAIComment, 255, "%s", getAIQuote(aiDifficulty, 1));
      UI_drawGameOver(&g, &uiState, "AI WINS!");
      matchStats.winner = 'O';
      getchar(); // Wait for enter
      break;
    }
    if (state == 0) {
      // Draw
      snprintf(uiState.lastAIComment, 255, "%s", getAIQuote(aiDifficulty, 3));
      UI_drawGameOver(&g, &uiState, "IT'S A DRAW!");
      matchStats.winner = 'D';
      getchar(); // Wait for enter
      break;
    }

    if (turn == 0) {
      /* ===== PLAYER TURN ===== */
      int r, c;
      if (!UI_getPlayerInput(&r, &c)) {
        printf("Invalid input.\n");
        continue;
      }
      // Validate move
      if (r < 0 || r > 2 || c < 0 || c > 2 || g.board[r][c] != ' ') {
        snprintf(uiState.statusMessage, 255,
                 "Invalid move (row: %d, col: %d), try again.", r, c);
        continue;
      }
      // Make move
      g.makeMove(&g, r, c, 'X');
      matchStats.player1Moves++;
      matchStats.totalMoves++;
      turn = 1;  // Switch to AI turn
    } else {
      /* ===== AI TURN ===== */
      snprintf(uiState.aiThought, 255, "%s", getAIQuote(aiDifficulty, 4));
      UI_drawGame(&g, &uiState, 0);

      sleep(1); // Brief pause to show thinking

      // Get best move
      Move m = AI_findBestMove(&ai);
      g.makeMove(&g, m.row, m.col, 'O');
      snprintf(uiState.lastAIComment, 255, "Placed at (%d, %d)", m.row, m.col);

      matchStats.player2Moves++;
      matchStats.totalMoves++;

      // Update AI statistics
      AI_getStats(&ai, &nodes, &maxDepth);
      matchStats.aiNodesExplored = nodes;
      matchStats.maxDepth = maxDepth;

      turn = 0;  // Switch to player turn
    }
  }

  UI_cleanup();

  // Save game statistics and update leaderboard
  saveGameStats(&matchStats);
  updateLeaderboard(username, matchStats.winner);
}

/**
 * playPlayerVsPlayer - Orchestrate a player vs player game with AI analysis
 * @player1: First player's username (plays as X)
 * @player2: Second player's username (plays as O)
 * @aiDifficulty: AI difficulty for analysis (doesn't play, just analyzes)
 * 
 * Similar to playGame but alternates between two human players
 * AI provides move analysis and recommendations
 */
void playPlayerVsPlayer(const char *player1, const char *player2,
                        int aiDifficulty) {
  Game g;
  AI ai;
  UIGameState uiState;
  GameStats matchStats;

  // Initialize game components
  Game_init(&g);
  AI_init(&ai, &g);
  AI_setDifficulty(&ai, aiDifficulty);
  AI_setVerbose(&ai, 2);

  // Initialize UI state
  snprintf(uiState.username, 64, "%s vs %s", player1, player2);
  strncpy(uiState.aiName, getAIName(aiDifficulty), 63);
  uiState.aiName[63] = '\0';
  uiState.aiDifficulty = aiDifficulty;
  strcpy(uiState.aiThought, "Analyzing positions...");
  strcpy(uiState.lastAIComment, "");
  uiState.candidateCount = 0;
  uiState.aiNodesExplored = 0;
  uiState.aiMaxDepth = 0;

  // Initialize game stats
  strncpy(matchStats.player1, player1, MAX_USERNAME - 1);
  matchStats.player1[MAX_USERNAME - 1] = '\0';
  strncpy(matchStats.player2, player2, MAX_USERNAME - 1);
  matchStats.player2[MAX_USERNAME - 1] = '\0';
  matchStats.totalMoves = 0;
  matchStats.player1Moves = 0;
  matchStats.player2Moves = 0;
  matchStats.aiNodesExplored = 0;
  matchStats.maxDepth = 0;
  matchStats.winner = 'D';

  UI_init();

  int turn = 0;  // 0=player1 (X), 1=player2 (O)
  int nodes = 0, maxDepth = 0;

  // Main game loop
  while (1) {
    // Get AI analysis
    if (matchStats.totalMoves > 0) {
      AICandidate cand[9];
      int candN = AI_explain(&ai, cand, 9);
      uiState.candidateCount = candN;
      for (int i = 0; i < candN; i++) {
        uiState.candidates[i] = cand[i];
      }
      AI_getStats(&ai, &nodes, &maxDepth);
      uiState.aiNodesExplored = nodes;
      uiState.aiMaxDepth = maxDepth;
    }

    // Draw game state
    UI_drawGame(&g, &uiState, 1);

    // Check win conditions
    int state = g.checkWin(&g);
    if (state == 1) {
      UI_drawGameOver(&g, &uiState, "PLAYER X WINS!");
      matchStats.winner = 'X';
      getchar();
      break;
    }
    if (state == -1) {
      UI_drawGameOver(&g, &uiState, "PLAYER O WINS!");
      matchStats.winner = 'O';
      getchar();
      break;
    }
    if (state == 0) {
      UI_drawGameOver(&g, &uiState, "IT'S A DRAW!");
      matchStats.winner = 'D';
      getchar();
      break;
    }

    // Get current player move
    printf("\n%s's turn (%c) - Enter move (row col): ",
           turn == 0 ? player1 : player2, turn == 0 ? 'X' : 'O');

    int r, c;
    if (!UI_getPlayerInput(&r, &c)) {
      printf("Invalid input.\n");
      continue;
    }

    // Validate move
    if (r < 0 || r > 2 || c < 0 || c > 2 || g.board[r][c] != ' ') {
      printf("Invalid move. Try again.\n");
      continue;
    }

    // Make move
    g.makeMove(&g, r, c, turn == 0 ? 'X' : 'O');
    if (turn == 0)
      matchStats.player1Moves++;
    else
      matchStats.player2Moves++;
    matchStats.totalMoves++;

    turn = 1 - turn;  // Switch turns
  }

  UI_cleanup();

  // Save game statistics (no leaderboard update for PvP)
  saveGameStats(&matchStats);
}

/**
 * playAIVsAI - Orchestrate an AI vs AI demonstration game
 * @aiDifficulty1: First AI's difficulty (plays as X)
 * @aiDifficulty2: Second AI's difficulty (plays as O)
 * 
 * Automated game between two AI opponents
 * Shows move-by-move analysis with 2-second delays
 * Useful for observing AI strategies and learning
 */
void playAIVsAI(int aiDifficulty1, int aiDifficulty2) {
  Game g;
  AI ai1, ai2;
  UIGameState uiState;
  GameStats matchStats;

  // Initialize game and both AIs
  Game_init(&g);
  AI_init(&ai1, &g);
  AI_setDifficulty(&ai1, aiDifficulty1);
  AI_setVerbose(&ai1, 2);

  AI_init(&ai2, &g);
  AI_setDifficulty(&ai2, aiDifficulty2);
  AI_setVerbose(&ai2, 2);

  // Initialize UI state
  snprintf(uiState.username, 64, "%s (X) vs %s (O)", getAIName(aiDifficulty1),
           getAIName(aiDifficulty2));
  strcpy(uiState.aiName, "AI Observer");
  uiState.aiDifficulty = 2;
  strcpy(uiState.aiThought, "Watching AIs compete...");
  strcpy(uiState.lastAIComment, "");
  uiState.candidateCount = 0;
  uiState.aiNodesExplored = 0;
  uiState.aiMaxDepth = 0;

  // Initialize game stats
  strncpy(matchStats.player1, getAIName(aiDifficulty1), MAX_USERNAME - 1);
  matchStats.player1[MAX_USERNAME - 1] = '\0';
  strncpy(matchStats.player2, getAIName(aiDifficulty2), MAX_USERNAME - 1);
  matchStats.player2[MAX_USERNAME - 1] = '\0';
  matchStats.totalMoves = 0;
  matchStats.player1Moves = 0;
  matchStats.player2Moves = 0;
  matchStats.aiNodesExplored = 0;
  matchStats.maxDepth = 0;
  matchStats.winner = 'D';

  UI_init();

  int turn = 0;  // 0=AI1 (X), 1=AI2 (O)
  int step = 1;
  int nodes = 0, maxDepth = 0;
  int lastWinnerNodes = 0, lastWinnerDepth = 0;

  printf("Press Enter to start AI vs AI match...");
  getchar();

  // Main game loop
  while (1) {
    // Check win condition at start of each iteration
    int state = g.checkWin(&g);
    if (state == 1) {
      UI_drawGameOver(&g, &uiState, "AI X WINS!");
      matchStats.winner = 'X';
      matchStats.aiNodesExplored = lastWinnerNodes;
      matchStats.maxDepth = lastWinnerDepth;
      printf("\nPress Enter to continue...");
      getchar();
      break;
    }
    if (state == -1) {
      UI_drawGameOver(&g, &uiState, "AI O WINS!");
      matchStats.winner = 'O';
      matchStats.aiNodesExplored = lastWinnerNodes;
      matchStats.maxDepth = lastWinnerDepth;
      printf("\nPress Enter to continue...");
      getchar();
      break;
    }
    if (state == 0) {
      UI_drawGameOver(&g, &uiState, "IT'S A DRAW!");
      matchStats.winner = 'D';
      matchStats.aiNodesExplored = lastWinnerNodes;
      matchStats.maxDepth = lastWinnerDepth;
      printf("\nPress Enter to continue...");
      getchar();
      break;
    }

    // Select current AI
    AI *currentAI = (turn == 0) ? &ai1 : &ai2;

    // Display current state
    printf("\n--- Step %d: %s's turn (%c) ---\n", step++,
           turn == 0 ? getAIName(aiDifficulty1) : getAIName(aiDifficulty2),
           turn == 0 ? 'X' : 'O');

    g.display(&g);

    // Get best move
    Move m = AI_findBestMove(currentAI);
    
    // Get stats from this move
    int moveNodes = 0, moveDepth = 0;
    AI_getStats(currentAI, &moveNodes, &moveDepth);
    lastWinnerNodes = moveNodes;
    lastWinnerDepth = moveDepth;

    printf("\n%s chooses position (%d, %d)\n",
           turn == 0 ? getAIName(aiDifficulty1) : getAIName(aiDifficulty2),
           m.row, m.col);
    printf("  Nodes explored: %d, Max depth: %d\n", moveNodes, moveDepth);

    // Make move
    g.makeMove(&g, m.row, m.col, turn == 0 ? 'X' : 'O');
    if (turn == 0)
      matchStats.player1Moves++;
    else
      matchStats.player2Moves++;
    matchStats.totalMoves++;

    turn = 1 - turn;  // Switch turns

    sleep(2);  // Pause between moves
  }

  UI_cleanup();

  // Save game statistics
  saveGameStats(&matchStats);
}
