/*
 * ui.h
 * 
 * User interface header for Tic-Tac-Toe
 * Handles all display and input operations
 */

#ifndef UI_H
#define UI_H

#include "ai.h"
#include "game.h"

/**
 * UIGameState structure
 * Encapsulates all state information needed for UI display
 * Contains AI analysis, player info, and status messages
 */
typedef struct {
  char aiName[64];              // Name of AI opponent (e.g., "Kitty", "Cop", "Sera")
  int aiDifficulty;             // AI difficulty level (0-2)
  char username[64];            // Current player's username
  AICandidate candidates[9];    // Array of candidate moves with scores
  int candidateCount;           // Number of valid candidates (0-9)
  char aiThought[256];          // AI's current thought/analysis
  char lastAIComment[256];      // AI's personality comment
  int aiNodesExplored;          // Number of nodes explored in last search
  int aiMaxDepth;               // Maximum depth reached in last search
  char statusMessage[256];      // Current status message to display
} UIGameState;

/**
 * UI_init - Initialize the UI system
 * 
 * Sets up terminal display and any necessary UI resources
 */
void UI_init(void);

/**
 * UI_drawGame - Render the game board and current state
 * @g: Pointer to Game structure
 * @state: Pointer to UIGameState with display information
 * @isPlayerTurn: 1 if waiting for player input, 0 if AI is thinking
 * 
 * Displays:
 * - Current board state
 * - AI candidate moves and analysis
 * - AI personality comments
 * - Performance statistics
 * - Status messages
 */
void UI_drawGame(Game *g, UIGameState *state, int isPlayerTurn);

/**
 * UI_drawGameOver - Display the end-game screen
 * @g: Pointer to Game structure
 * @state: Pointer to UIGameState
 * @result: Result string ("YOU WIN!", "AI WINS!", "IT'S A DRAW!")
 * 
 * Shows final board state, result, and AI reaction
 */
void UI_drawGameOver(Game *g, UIGameState *state, const char *result);

/**
 * UI_getPlayerInput - Get and validate player move input
 * @row: Pointer to store row coordinate (0-2)
 * @col: Pointer to store column coordinate (0-2)
 * 
 * Returns: 1 if valid input received, 0 if invalid
 * Prompts user for "row col" input and validates coordinates
 */
int UI_getPlayerInput(int *row, int *col);

/**
 * UI_cleanup - Clean up UI resources
 * 
 * Releases any UI resources allocated during initialization
 */
void UI_cleanup(void);

#endif // UI_H
