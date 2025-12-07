/*
 * ai.c
 * 
 * AI opponent implementation for Tic-Tac-Toe
 * Implements minimax algorithm with three difficulty levels:
 * - Easy (0): Random move selection
 * - Medium (1): Limited minimax with randomness
 * - Hard (2): Full optimal minimax algorithm
 */

#include "ai.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * AI_evaluate - Evaluate terminal game state for AI (playing as O)
 * @g: Pointer to Game structure
 * 
 * Returns:
 *  +10 - AI (O) wins
 *  -10 - Player (X) wins
 *    0 - Draw or ongoing game
 * 
 * Note: AI plays as 'O', so O winning returns positive score
 */
static int AI_evaluate(Game *g) {
  int s = g->checkWin(g);
  if (s == -1)  // O wins
    return 10;
  if (s == 1)   // X wins
    return -10;
  return 0;     // Draw or ongoing
}

/* Global instrumentation counters for performance tracking */
static int g_nodesSearched = 0;    // Total nodes explored in minimax tree
static int g_maxDepthReached = 0;  // Maximum depth reached in search

/**
 * AI_minimax - Minimax algorithm with depth-biased scoring
 * @g: Pointer to Game structure
 * @depth: Current search depth (0 at root)
 * @isMax: 1 if maximizing player (AI/O), 0 if minimizing player (X)
 * 
 * Returns: Best score achievable from current position
 * 
 * Algorithm:
 * 1. Check if game is over (win/loss/draw) - return score
 * 2. If maximizing (AI turn): try all moves, pick maximum score
 * 3. If minimizing (opponent turn): try all moves, pick minimum score
 * 4. Use depth bias to prefer quicker wins and delay losses
 * 
 * Depth bias: Winning sooner is better (score - depth)
 *             Losing later is better (score + depth)
 */
static int AI_minimax(Game *g, int depth, int isMax) {
  // Track instrumentation
  if (depth > g_maxDepthReached)
    g_maxDepthReached = depth;
  g_nodesSearched++;

  // Check for terminal state
  int score = AI_evaluate(g);

  if (score == 10)  // AI wins - prefer quicker wins
    return score - depth;
  if (score == -10) // Player wins - prefer delaying loss
    return score + depth;
  if (!g->isMovesLeft(g))  // Draw
    return 0;

  // Maximizing player (AI playing as 'O')
  if (isMax) {
    int best = -INT_MAX;
    // Try all possible moves
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        if (g->board[i][j] == ' ') {
          // Make move and recurse
          g->makeMove(g, i, j, 'O');
          int val = AI_minimax(g, depth + 1, 0);
          // Undo move
          g->makeMove(g, i, j, ' ');
          // Update best score
          if (val > best)
            best = val;
        }
      }
    }
    return best;
  } 
  // Minimizing player (opponent playing as 'X')
  else {
    int best = INT_MAX;
    // Try all possible moves
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        if (g->board[i][j] == ' ') {
          // Make move and recurse
          g->makeMove(g, i, j, 'X');
          int val = AI_minimax(g, depth + 1, 1);
          // Undo move
          g->makeMove(g, i, j, ' ');
          // Update best score (minimize)
          if (val < best)
            best = val;
        }
      }
    }
    return best;
  }
}

/**
 * Candidate structure (internal)
 * Temporary structure to hold move candidates with scores
 */
typedef struct {
  int r, c;    // Row and column
  int score;   // Minimax score
} Candidate;

/**
 * AI_findBestMove_impl - Internal implementation of move finding
 * @self: Pointer to AI structure
 * 
 * Returns: Best move for current position
 * 
 * Process:
 * 1. Reset performance counters
 * 2. Evaluate all empty positions using minimax
 * 3. Apply difficulty-based move selection:
 *    - Hard: Choose optimal move (highest score)
 *    - Medium: Choose randomly among good moves (score >= threshold)
 *    - Easy: Choose randomly among all legal moves
 * 4. Display prediction if verbose mode enabled
 */
static Move AI_findBestMove_impl(AI *self) {
  /* Reset instrumentation counters for this search */
  g_nodesSearched = 0;
  g_maxDepthReached = 0;

  Game *g = self->game;
  int bestVal = -INT_MAX;
  Move bestMove = {-1, -1};
  Candidate cand[9];  // Max 9 possible moves
  int n = 0;          // Number of legal moves found
  
  // Evaluate all empty positions
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (g->board[i][j] == ' ') {
        // Try this move
        g->makeMove(g, i, j, 'O');
        int moveVal = AI_minimax(g, 0, 0);
        g->makeMove(g, i, j, ' ');

        // Store candidate
        cand[n].r = i;
        cand[n].c = j;
        cand[n].score = moveVal;
        n++;
        
        // Display score in verbose mode
        if (self->verbose >= 2)
          printf("AI score for move (%d,%d) = %d\n", i, j, moveVal);
        
        // Track best move
        if (moveVal > bestVal) {
          bestVal = moveVal;
          bestMove.row = i;
          bestMove.col = j;
        }
      }
    }
  }

  /* Select move based on difficulty level */
  if (n == 0)  // No legal moves (shouldn't happen)
    return bestMove;
    
  if (self->difficulty == 2) {
    /* Hard: Use pure best move (already set in bestMove) */
    /* No randomness - always optimal play */
  } 
  else if (self->difficulty == 1) {
    /* Medium: Choose randomly among moves with score >= bestVal - 2 */
    /* Allows some suboptimal play for competitive but beatable AI */
    int threshold = bestVal - 2;
    if (threshold < -10)
      threshold = -10;
    
    // Build pool of acceptable moves
    int pool[9], pn = 0;
    for (int k = 0; k < n; ++k)
      if (cand[k].score >= threshold)
        pool[pn++] = k;
    
    // Pick randomly from pool
    if (pn > 0) {
      int pick = pool[rand() % pn];
      bestMove.row = cand[pick].r;
      bestMove.col = cand[pick].c;
      bestVal = cand[pick].score;
    }
  } 
  else {
    /* Easy: Pick randomly among all legal moves */
    /* No strategy - purely random play */
    int pool[9], pn = 0;
    for (int k = 0; k < n; ++k)
      pool[pn++] = k;
    
    int pick = pool[rand() % pn];
    bestMove.row = cand[pick].r;
    bestMove.col = cand[pick].c;
    bestVal = cand[pick].score;
  }

  // Display prediction in verbose mode
  if (self->verbose >= 1) {
    if (bestVal > 0)
      printf("AI prediction: AI (O) will win (score=%d)\n", bestVal);
    else if (bestVal < 0)
      printf("AI prediction: Player (X) will win (score=%d)\n", bestVal);
    else
      printf("AI prediction: Game will be a draw (score=0)\n");
  }

  return bestMove;
}

/**
 * AI_init - Initialize AI structure
 * @ai: Pointer to AI structure to initialize
 * @game: Pointer to game state for AI to analyze
 * 
 * Sets default values:
 * - Difficulty: Hard (2)
 * - Verbosity: Silent (0)
 * Seeds random number generator for difficulty modes using randomness
 */
void AI_init(AI *ai, Game *game) {
  ai->game = game;
  ai->verbose = 0;
  ai->findBestMove = AI_findBestMove_impl;
  ai->difficulty = 2; /* Default: hard */
  /* Seed RNG for difficulty modes that use randomness */
  srand((unsigned)time(NULL));
}

/**
 * AI_findBestMove - Public wrapper for move finding
 * @ai: Pointer to AI structure
 * 
 * Returns: Best move for current position based on difficulty level
 */
Move AI_findBestMove(AI *ai) { 
  return ai->findBestMove(ai); 
}

/**
 * AI_setVerbose - Set AI verbosity level
 * @ai: Pointer to AI structure
 * @v: Verbosity level (0=silent, 1=brief, 2=detailed)
 * 
 * Clamps value to valid range [0, 2]
 */
void AI_setVerbose(AI *ai, int v) {
  if (v < 0)
    v = 0;
  if (v > 2)
    v = 2;
  ai->verbose = v;
}

/**
 * AI_setDifficulty - Set AI difficulty level
 * @ai: Pointer to AI structure
 * @level: Difficulty (0=Easy, 1=Medium, 2=Hard)
 * 
 * Clamps value to valid range [0, 2]
 */
void AI_setDifficulty(AI *ai, int level) {
  if (level < 0)
    level = 0;
  if (level > 2)
    level = 2;
  ai->difficulty = level;
}

/**
 * AI_getPrediction - Get AI's evaluation of current position
 * @ai: Pointer to AI structure
 * 
 * Returns:
 *  -1 - AI will win with optimal play
 *   1 - Player will win with optimal play
 *   0 - Game will be a draw with optimal play
 * 
 * Note: Computes without displaying verbose output
 */
int AI_getPrediction(AI *ai) {
  /* Compute bestVal without printing */
  Game *g = ai->game;
  int bestVal = -INT_MAX;
  
  // Evaluate all possible moves
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      if (g->board[i][j] == ' ') {
        g->makeMove(g, i, j, 'O');
        int mv = AI_minimax(g, 0, 0);
        g->makeMove(g, i, j, ' ');
        if (mv > bestVal)
          bestVal = mv;
      }
  
  // Interpret score
  if (bestVal > 0)
    return -1; /* AI will win */
  if (bestVal < 0)
    return 1;  /* Player will win */
  return 0;    /* Draw */
}

/**
 * AI_explain - Generate detailed analysis of candidate moves
 * @ai: Pointer to AI structure
 * @out: Output array for candidate moves
 * @maxOut: Maximum number of candidates to return
 * 
 * Returns: Number of candidates found
 * 
 * Fills output array with all legal moves and their minimax scores
 * Resets performance counters before analysis
 */
int AI_explain(AI *ai, AICandidate *out, int maxOut) {
  /* Reset counters before explain run */
  g_nodesSearched = 0;
  g_maxDepthReached = 0;
  
  Game *g = ai->game;
  int n = 0;
  
  // Evaluate all empty positions
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (g->board[i][j] == ' ') {
        g->makeMove(g, i, j, 'O');
        int mv = AI_minimax(g, 0, 0);
        g->makeMove(g, i, j, ' ');
        
        // Store in output array if space available
        if (n < maxOut) {
          out[n].row = i;
          out[n].col = j;
          out[n].score = mv;
        }
        n++;
      }
    }
  }
  return n;
}

/**
 * AI_getStats - Retrieve performance metrics from last search
 * @ai: Pointer to AI structure (unused, counters are global)
 * @nodes: Output pointer for nodes explored count
 * @maxDepth: Output pointer for maximum depth reached
 * 
 * Provides statistics about computational effort of last minimax search
 */
void AI_getStats(AI *ai, int *nodes, int *maxDepth) {
  (void)ai;  // Unused parameter
  if (nodes)
    *nodes = g_nodesSearched;
  if (maxDepth)
    *maxDepth = g_maxDepthReached;
}

/**
 * AI_resetStats - Reset performance counters
 * @ai: Pointer to AI structure (unused, counters are global)
 * 
 * Clears nodes explored and max depth for fresh analysis
 */
void AI_resetStats(AI *ai) {
  (void)ai;  // Unused parameter
  g_nodesSearched = 0;
  g_maxDepthReached = 0;
}
