/*
 * game.h
 * 
 * Core game logic header for Tic-Tac-Toe
 * Defines the game board structure and operations using object-oriented style in C
 */

#ifndef GAME_H
#define GAME_H

/* Pure C header — no C++ guards */

/**
 * Move structure
 * Represents a single move on the board with row and column coordinates
 */
typedef struct {
  int row;  // Row index (0-2): 0=top, 1=middle, 2=bottom
  int col;  // Column index (0-2): 0=left, 1=center, 2=right
} Move;

/* Forward declaration for self-referential function pointers */
typedef struct Game Game;

/**
 * Game structure
 * Encapsulates the Tic-Tac-Toe game state and operations
 * Uses function pointers to simulate object-oriented method calls
 */
struct Game {
  char board[3][3];  // 3x3 game board: ' '=empty, 'X'=player X, 'O'=player O
  
  /* Method pointers for OOP-like style */
  void (*display)(Game *self);                                    // Display the board
  void (*makeMove)(Game *self, int row, int col, char symbol);   // Place a symbol on the board
  int (*checkWin)(Game *self);                                   // Check for winner: 1=X wins, -1=O wins, 0=draw, 2=ongoing
  int (*isMovesLeft)(Game *self);                                // Check if moves available: 1=yes, 0=no
};

/**
 * Game_init - Initialize the Game object and set method pointers
 * @g: Pointer to Game structure to initialize
 * 
 * Clears the board and assigns function pointers for all game operations
 */
void Game_init(Game *g);

#endif // GAME_H
