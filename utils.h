/*
 * utils.h
 * 
 * Utility functions header for Tic-Tac-Toe
 * Provides file I/O, leaderboard management, statistics, and AI personality
 */

#ifndef UTILS_H
#define UTILS_H

/* File and configuration constants */
#define MAX_USERNAME 50              // Maximum length for player usernames
#define LEADERBOARD_FILE "leaderboard.txt"  // File storing player rankings
#define STATS_FILE "game_stats.txt"         // File storing game history

/**
 * PlayerRecord structure
 * Stores a player's game statistics for the leaderboard
 */
typedef struct {
  char username[MAX_USERNAME];  // Player's unique username
  int wins;                     // Number of games won
  int losses;                   // Number of games lost
  int draws;                    // Number of games drawn
  int totalGames;              // Total games played (wins + losses + draws)
} PlayerRecord;

/**
 * GameStats structure
 * Represents detailed statistics for a single match
 * Used for game history and analysis
 */
typedef struct {
  char player1[MAX_USERNAME];   // Player X (typically human player)
  char player2[MAX_USERNAME];   // Player O (typically AI opponent)
  int totalMoves;               // Total moves made in the game
  int player1Moves;             // Number of X moves
  int player2Moves;             // Number of O moves
  int aiNodesExplored;          // Nodes explored by AI (minimax performance)
  int maxDepth;                 // Maximum search depth reached by AI
  char winner;                  // 'X' for player1 wins, 'O' for player2 wins, 'D' for draw
} GameStats;

/**
 * clearScreen - Clear the terminal screen
 * 
 * Platform-independent screen clearing function
 */
void clearScreen();

/* ========== Leaderboard Functions ========== */

/**
 * savePlayerRecord - Save or update a player's record to leaderboard file
 * @record: Pointer to PlayerRecord to save
 * 
 * Updates existing record or appends new record to leaderboard.txt
 */
void savePlayerRecord(const PlayerRecord *record);

/**
 * loadPlayerRecord - Load a player's record from leaderboard file
 * @username: Player's username to search for
 * @record: Pointer to PlayerRecord to fill with loaded data
 * 
 * Returns: 1 if player found, 0 if new player (record initialized to zeros)
 */
int loadPlayerRecord(const char *username, PlayerRecord *record);

/**
 * displayLeaderboard - Display ranked list of all players
 * 
 * Shows all players sorted by win count with statistics table
 */
void displayLeaderboard();

/**
 * updateLeaderboard - Update a player's record after game completion
 * @username: Player's username
 * @winner: Game result ('X'=player won, 'O'=opponent won, 'D'=draw)
 * 
 * Loads player record, updates appropriate counter, and saves back
 */
void updateLeaderboard(const char *username, char winner);

/* ========== Statistics Functions ========== */

/**
 * saveGameStats - Log game statistics to history file
 * @stats: Pointer to GameStats containing match details
 * 
 * Appends game record to game_stats.txt with timestamp
 */
void saveGameStats(const GameStats *stats);

/**
 * displayAllStats - Display complete game history
 * 
 * Shows all recorded games with full details
 */
void displayAllStats();

/**
 * displayPlayerStats - Display game history for specific player
 * @username: Player's username to filter by
 * 
 * Shows only games where specified player participated
 */
void displayPlayerStats(const char *username);

/* ========== AI Personality Functions ========== */

/**
 * getAIName - Get AI opponent's name based on difficulty
 * @difficulty: Difficulty level (0=Easy, 1=Medium, 2=Hard)
 * 
 * Returns: AI name string ("Kitty", "Cop", or "Sera")
 */
const char *getAIName(int difficulty);

/**
 * getAIQuote - Get contextual personality quote from AI
 * @difficulty: AI difficulty level (0-2)
 * @occasion: Context (0=intro, 1=win, 2=lose, 3=draw, 4=thinking)
 * 
 * Returns: Personality-appropriate quote string
 * Each AI has unique dialogue for different situations
 */
const char *getAIQuote(int difficulty, int occasion);

#endif // UTILS_H
