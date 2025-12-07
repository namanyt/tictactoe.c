/*
 * game.c
 * 
 * Core game logic implementation for Tic-Tac-Toe
 * Implements board management, win detection, and move validation
 */

#include "game.h"
#include <stdio.h>
#include <string.h>

/**
 * Game_display - Display the current game board
 * @self: Pointer to Game structure
 * 
 * Renders the 3x3 board with separators
 * Shows X, O, or space for each cell
 */
static void Game_display(Game *self) {
  printf("-------------\n");
  for (int i = 0; i < 3; ++i) {
    printf("| ");
    for (int j = 0; j < 3; ++j) {
      printf("%c | ", self->board[i][j]);
    }
    printf("\n-------------\n");
  }
}

/**
 * Game_isMovesLeft - Check if any empty cells remain on board
 * @self: Pointer to Game structure
 * 
 * Returns: 1 if at least one empty cell exists, 0 if board is full
 * Scans entire board for space characters
 */
static int Game_isMovesLeft(Game *self) {
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      if (self->board[i][j] == ' ')
        return 1;
  return 0;
}

/**
 * Game_checkWin - Determine game state and winner
 * @self: Pointer to Game structure
 * 
 * Returns:
 *   1  - X wins (three X's in a row/column/diagonal)
 *  -1  - O wins (three O's in a row/column/diagonal)
 *   0  - Draw (board full, no winner)
 *   2  - Game ongoing (moves available, no winner yet)
 * 
 * Checks all possible winning combinations:
 * - All 3 rows
 * - All 3 columns
 * - Both diagonals (top-left to bottom-right, top-right to bottom-left)
 */
static int Game_checkWin(Game *self) {
  char (*board)[3] = self->board;
  
  // Check all rows for three-in-a-row
  for (int i = 0; i < 3; ++i) {
    if (board[i][0] != ' ' && board[i][0] == board[i][1] &&
        board[i][1] == board[i][2]) {
      return (board[i][0] == 'X') ? 1 : -1;
    }
  }
  
  // Check all columns for three-in-a-column
  for (int j = 0; j < 3; ++j) {
    if (board[0][j] != ' ' && board[0][j] == board[1][j] &&
        board[1][j] == board[2][j]) {
      return (board[0][j] == 'X') ? 1 : -1;
    }
  }
  
  // Check diagonal: top-left to bottom-right
  if (board[0][0] != ' ' && board[0][0] == board[1][1] &&
      board[1][1] == board[2][2])
    return (board[0][0] == 'X') ? 1 : -1;
  
  // Check diagonal: top-right to bottom-left
  if (board[0][2] != ' ' && board[0][2] == board[1][1] &&
      board[1][1] == board[2][0])
    return (board[0][2] == 'X') ? 1 : -1;

  // No winner found - check if game is draw or ongoing
  if (!Game_isMovesLeft(self))
    return 0; // Draw: board full, no winner
  return 2;   // Ongoing: moves still available
}

/**
 * Game_makeMove - Place a symbol on the board
 * @self: Pointer to Game structure
 * @row: Row index (0-2)
 * @col: Column index (0-2)
 * @symbol: Symbol to place ('X' or 'O')
 * 
 * Note: Does not validate if cell is empty - caller must validate
 */
static void Game_makeMove(Game *self, int row, int col, char symbol) {
  self->board[row][col] = symbol;
}

/**
 * Game_init - Initialize Game structure
 * @g: Pointer to Game structure to initialize
 * 
 * Clears the board (sets all cells to space) and assigns
 * function pointers for all game operations
 */
void Game_init(Game *g) {
  memset(g->board, ' ', sizeof(g->board));  // Clear board with spaces
  g->display = Game_display;                // Assign display function
  g->makeMove = Game_makeMove;              // Assign move function
  g->checkWin = Game_checkWin;              // Assign win check function
  g->isMovesLeft = Game_isMovesLeft;        // Assign moves left check function
}
