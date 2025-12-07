/*
 * ui.c
 * 
 * User interface implementation for Tic-Tac-Toe
 * Handles terminal display using ANSI escape codes and input processing
 */

#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

/* Terminal dimensions (default 80x24, updated at init) */
static int termWidth = 80, termHeight = 24;

/**
 * UI_init - Initialize UI system and detect terminal size
 * 
 * Queries terminal for actual dimensions if supported
 * Falls back to 80x24 if ioctl unavailable
 */
void UI_init(void) {
#ifdef TIOCGWINSZ
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1) {
    termWidth = ws.ws_col;
    termHeight = ws.ws_row;
  }
#endif
}

/**
 * UI_cleanup - Clean up UI resources
 * 
 * Currently no resources to free, placeholder for future use
 */
void UI_cleanup(void) {}

/**
 * clearScreen - Clear terminal screen using ANSI escape codes
 * 
 * Uses: \033[2J (clear entire screen)
 *       \033[H  (move cursor to home position)
 */
static void clearScreen(void) {
  printf("\033[2J\033[H");
  fflush(stdout);
}

/**
 * drawBoardColumn - Render the game board on left side of screen
 * @g: Pointer to Game structure
 * @startCol: Starting column position for drawing
 * 
 * Displays:
 * - Current board state (3x3 grid)
 * - Position reference guide showing (row,col) coordinates
 */
static void drawBoardColumn(Game *g, int startCol) {
  int row = 2;

  // Title
  printf("\033[%d;%dH", row++, startCol);
  printf("GAME BOARD");

  row++;

  // Row 0 of board
  printf("\033[%d;%dH", row++, startCol);
  printf("%c | %c | %c", g->board[0][0], g->board[0][1], g->board[0][2]);

  // Separator
  printf("\033[%d;%dH", row++, startCol);
  printf("--+---+--");

  // Row 1 of board
  printf("\033[%d;%dH", row++, startCol);
  printf("%c | %c | %c", g->board[1][0], g->board[1][1], g->board[1][2]);

  // Separator
  printf("\033[%d;%dH", row++, startCol);
  printf("--+---+--");

  // Row 2 of board
  printf("\033[%d;%dH", row++, startCol);
  printf("%c | %c | %c", g->board[2][0], g->board[2][1], g->board[2][2]);

  row++;

  // Position reference guide
  printf("\033[%d;%dH", row++, startCol);
  printf("Positions:");
  printf("\033[%d;%dH", row++, startCol);
  printf("(0,0) (0,1) (0,2)");
  printf("\033[%d;%dH", row++, startCol);
  printf("(1,0) (1,1) (1,2)");
  printf("\033[%d;%dH", row++, startCol);
  printf("(2,0) (2,1) (2,2)");
}

/**
 * drawAIColumn - Render AI analysis on right side of screen
 * @state: Pointer to UIGameState containing AI information
 * @startCol: Starting column position for drawing
 * 
 * Displays:
 * - AI name and difficulty level
 * - Current AI thought/reasoning
 * - Top candidate moves with scores
 * - AI personality comments
 * - Performance statistics (nodes explored, search depth)
 */
static void drawAIColumn(UIGameState *state, int startCol) {
  int row = 2;

  // AI name and difficulty
  printf("\033[%d;%dH", row++, startCol);
  printf("%s (Lvl %d)", state->aiName, state->aiDifficulty);

  row++;

  // Display AI's current thought
  if (strlen(state->aiThought) > 0) {
    printf("\033[%d;%dH", row++, startCol);
    printf("Thinking: %.50s", state->aiThought);
  }

  row++;

  // Display top candidate moves
  printf("\033[%d;%dH", row++, startCol);
  printf("Top Moves:");
  for (int i = 0; i < state->candidateCount && i < 4; i++) {
    printf("\033[%d;%dH", row++, startCol);
    printf("  (%d,%d) score:%d", state->candidates[i].row,
           state->candidates[i].col, state->candidates[i].score);
  }

  row++;

  // Display AI personality comment
  if (strlen(state->lastAIComment) > 0) {
    printf("\033[%d;%dH", row++, startCol);
    printf("Comment: %.50s", state->lastAIComment);
  }

  row++;

  // Display performance metrics
  printf("\033[%d;%dH", row++, startCol);
  printf("Nodes: %d", state->aiNodesExplored);
  printf("\033[%d;%dH", row++, startCol);
  printf("Depth: %d", state->aiMaxDepth);
}

/**
 * UI_drawGame - Render complete game state to screen
 * @g: Pointer to Game structure
 * @state: Pointer to UIGameState with display information
 * @isPlayerTurn: 1 if waiting for player input, 0 if AI is thinking
 * 
 * Creates two-column layout:
 * - Left: Game board and position guide
 * - Right: AI analysis and statistics
 * Bottom: Input prompt or status message
 */
void UI_drawGame(Game *g, UIGameState *state, int isPlayerTurn) {
  clearScreen();

  // Header showing players
  printf("\033[1;1HTC-TAC-TOE: %s vs %s", state->username, state->aiName);

  // Calculate column positions (split screen in half)
  int colWidth = termWidth / 2;

  // Draw both columns
  drawBoardColumn(g, 2);
  drawAIColumn(state, colWidth + 2);

  // Bottom prompt
  printf("\033[%d;1H", termHeight - 2);
  if (isPlayerTurn) {
    printf("YOUR TURN - Enter move (row col): ");
    fflush(stdout);
  } else {
    printf("AI is thinking...");
    fflush(stdout);
  }
}

/**
 * UI_drawGameOver - Display game over screen
 * @g: Pointer to Game structure (for final board state)
 * @state: Pointer to UIGameState (unused, for future enhancements)
 * @result: Result string to display ("YOU WIN!", "AI WINS!", etc.)
 * 
 * Shows final board state and game result
 * Waits for user to press Enter before continuing
 */
void UI_drawGameOver(Game *g, UIGameState *state __attribute__((unused)),
                     const char *result) {
  clearScreen();

  // Title
  printf("\033[1;1HGAME OVER");

  int row = 3;

  // Display final board state
  printf("\033[%d;2H", row++);
  printf("%c | %c | %c", g->board[0][0], g->board[0][1], g->board[0][2]);

  printf("\033[%d;2H", row++);
  printf("--+---+--");

  printf("\033[%d;2H", row++);
  printf("%c | %c | %c", g->board[1][0], g->board[1][1], g->board[1][2]);

  printf("\033[%d;2H", row++);
  printf("--+---+--");

  printf("\033[%d;2H", row++);
  printf("%c | %c | %c", g->board[2][0], g->board[2][1], g->board[2][2]);

  row += 2;

  // Display result message
  printf("\033[%d;2H", row++);
  printf("%s", result);

  // Prompt to continue
  printf("\033[%d;1H", termHeight - 2);
  printf("Press Enter to continue...");
  fflush(stdout);
}

/**
 * UI_getPlayerInput - Get player move input from keyboard
 * @row: Pointer to store row coordinate (0-2)
 * @col: Pointer to store column coordinate (0-2)
 * 
 * Returns: 1 if valid input received, 0 if invalid
 * 
 * Expects input format: "row col" (two integers separated by space)
 * Clears input buffer after reading
 */
int UI_getPlayerInput(int *row, int *col) {
  // Read two integers
  if (scanf("%d %d", row, col) != 2) {
    // Invalid input - clear buffer and return error
    while (getchar() != '\n')
      ;
    return 0;
  }
  // Clear remaining input buffer
  while (getchar() != '\n')
    ;

  return 1;
}
