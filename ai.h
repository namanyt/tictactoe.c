/*
 * ai.h
 * 
 * AI opponent header for Tic-Tac-Toe
 * Implements minimax algorithm with multiple difficulty levels
 */

#ifndef AI_H
#define AI_H

#include "game.h"

/* Forward declaration for self-referential function pointers */
typedef struct AI AI;

/**
 * AI structure
 * Encapsulates the AI player's state and strategy
 * Supports three difficulty levels with different algorithms
 */
struct AI {
  Game *game;                      // Pointer to the game state being analyzed
  Move (*findBestMove)(AI *self);  // Function pointer for move selection strategy
  int difficulty;                  // Difficulty level: 0=Easy (random), 1=Medium (limited), 2=Hard (optimal)
  int verbose;                     // Verbosity level: 0=silent, 1=brief, 2=detailed explanations
};

/**
 * AI_init - Initialize AI structure
 * @ai: Pointer to AI structure to initialize
 * @game: Pointer to game state for AI to analyze
 * 
 * Sets up the AI with default difficulty (Hard) and verbosity (detailed)
 */
void AI_init(AI *ai, Game *game);

/**
 * AI_findBestMove - Calculate the best move for current board state
 * @ai: Pointer to AI structure
 * 
 * Returns: Move structure with row/col of recommended move
 * Algorithm varies by difficulty level
 */
Move AI_findBestMove(AI *ai);

/**
 * AI_setVerbose - Set AI verbosity level
 * @ai: Pointer to AI structure
 * @v: Verbosity level (0=silent, 1=brief, 2=detailed)
 */
void AI_setVerbose(AI *ai, int v);

/**
 * AI_setDifficulty - Set AI difficulty level
 * @ai: Pointer to AI structure
 * @level: Difficulty (0=Easy, 1=Medium, 2=Hard)
 */
void AI_setDifficulty(AI *ai, int level);

/**
 * AI_getPrediction - Get AI's evaluation of current position
 * @ai: Pointer to AI structure
 * 
 * Returns: Score indicating position evaluation
 *          Positive = AI advantage, Negative = opponent advantage, 0 = draw expected
 */
int AI_getPrediction(AI *ai);

/**
 * AICandidate structure
 * Represents a candidate move with its evaluation score
 */
typedef struct {
  int row;    // Row of candidate move
  int col;    // Column of candidate move
  int score;  // Minimax evaluation score
} AICandidate;

/**
 * AI_explain - Generate detailed analysis of candidate moves
 * @ai: Pointer to AI structure
 * @out: Output array for candidate moves
 * @maxOut: Maximum number of candidates to return
 * 
 * Returns: Number of candidates found (up to maxOut)
 * Fills output array with top candidate moves sorted by score
 */
int AI_explain(AI *ai, AICandidate *out, int maxOut);

/**
 * AI_getStats - Retrieve performance metrics from last search
 * @ai: Pointer to AI structure
 * @nodes: Output pointer for number of nodes explored
 * @maxDepth: Output pointer for maximum search depth reached
 * 
 * Provides statistics about the AI's last minimax search
 */
void AI_getStats(AI *ai, int *nodes, int *maxDepth);

/**
 * AI_resetStats - Reset performance counters
 * @ai: Pointer to AI structure
 * 
 * Clears nodes explored and max depth counters for fresh analysis
 */
void AI_resetStats(AI *ai);

#endif // AI_H
