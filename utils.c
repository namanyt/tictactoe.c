/*
 * utils.c
 * 
 * Utility functions implementation for Tic-Tac-Toe
 * Handles file I/O, leaderboard management, statistics tracking, and AI personalities
 */

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * clearScreen - Clear the terminal screen
 * 
 * Platform-specific implementation:
 * - Windows: Uses "cls" command
 * - Unix/Linux/macOS: Uses "clear" command
 */
void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

/* ==================== LEADERBOARD FUNCTIONS ==================== */

/**
 * savePlayerRecord - Save or update player record in leaderboard file
 * @record: Pointer to PlayerRecord to save
 * 
 * Process:
 * 1. Read all existing records from file
 * 2. Update record if username exists, otherwise add new record
 * 3. Write all records back to file
 * 
 * File format: "username wins losses draws totalGames" (space-separated)
 * Maximum 100 players supported
 */
void savePlayerRecord(const PlayerRecord *record) {
  FILE *file = fopen(LEADERBOARD_FILE, "r");
  PlayerRecord records[100];  // Max 100 players
  int count = 0;
  int found = 0;

  // Read existing records from file
  if (file != NULL) {
    while (count < 100 &&
           fscanf(file, "%49s %d %d %d %d", records[count].username,
                  &records[count].wins, &records[count].losses,
                  &records[count].draws, &records[count].totalGames) == 5) {
      // Check if this is the record to update
      if (strcmp(records[count].username, record->username) == 0) {
        records[count] = *record;  // Update existing record
        found = 1;
      }
      count++;
    }
    fclose(file);
  }

  // Add new record if username not found
  if (!found && count < 100) {
    records[count++] = *record;
  }

  // Write all records back to file
  file = fopen(LEADERBOARD_FILE, "w");
  if (file == NULL) {
    printf("Error: Could not save leaderboard data.\n");
    return;
  }

  for (int i = 0; i < count; i++) {
    fprintf(file, "%s %d %d %d %d\n", records[i].username, records[i].wins,
            records[i].losses, records[i].draws, records[i].totalGames);
  }

  fclose(file);
}

/**
 * loadPlayerRecord - Load player record from leaderboard file
 * @username: Player's username to search for
 * @record: Pointer to PlayerRecord to fill with loaded data
 * 
 * Returns: 1 if player found (returning player), 0 if not found (new player)
 * 
 * If player not found, initializes record with username and zero stats
 */
int loadPlayerRecord(const char *username, PlayerRecord *record) {
  FILE *file = fopen(LEADERBOARD_FILE, "r");
  if (file == NULL) {
    // File doesn't exist - initialize new player
    strncpy(record->username, username, MAX_USERNAME - 1);
    record->username[MAX_USERNAME - 1] = '\0';
    record->wins = 0;
    record->losses = 0;
    record->draws = 0;
    record->totalGames = 0;
    return 0; // New player
  }

  // Search file for matching username
  PlayerRecord temp;
  while (fscanf(file, "%49s %d %d %d %d", temp.username, &temp.wins,
                &temp.losses, &temp.draws, &temp.totalGames) == 5) {
    if (strcmp(temp.username, username) == 0) {
      *record = temp;  // Found - copy data
      fclose(file);
      return 1; // Returning player
    }
  }

  fclose(file);

  // Not found in file - initialize new player
  strncpy(record->username, username, MAX_USERNAME - 1);
  record->username[MAX_USERNAME - 1] = '\0';
  record->wins = 0;
  record->losses = 0;
  record->draws = 0;
  record->totalGames = 0;
  return 0; // New player
}

/**
 * displayLeaderboard - Display ranked list of all players
 * 
 * Shows table with:
 * - Player name
 * - Total games played
 * - Wins, losses, draws
 * - Win rate percentage
 * 
 * Players sorted by win count (descending)
 */
void displayLeaderboard() {
  FILE *file = fopen(LEADERBOARD_FILE, "r");
  if (file == NULL) {
    printf("\n=== LEADERBOARD ===\n");
    printf("No games played yet!\n");
    printf("===================\n\n");
    return;
  }

  PlayerRecord records[100];
  int count = 0;

  // Read all records from file
  while (count < 100 &&
         fscanf(file, "%49s %d %d %d %d", records[count].username,
                &records[count].wins, &records[count].losses,
                &records[count].draws, &records[count].totalGames) == 5) {
    count++;
  }
  fclose(file);

  // Sort by wins (descending) using bubble sort
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      if (records[j].wins < records[j + 1].wins) {
        PlayerRecord temp = records[j];
        records[j] = records[j + 1];
        records[j + 1] = temp;
      }
    }
  }

  // Display formatted table
  printf("\n=================================== LEADERBOARD "
         "===================================\n");
  printf("%-20s | %5s | %6s | %5s | %6s | Win Rate\n", "Player", "Games",
         "Wins", "Loss", "Draws");
  printf(
      "------------------------------------------------------------------------"
      "-----------\n");

  for (int i = 0; i < count; i++) {
    // Calculate win rate percentage
    double winRate = records[i].totalGames > 0
                         ? (records[i].wins * 100.0) / records[i].totalGames
                         : 0.0;
    printf("%-20s | %5d | %6d | %5d | %6d | %.1f%%\n", records[i].username,
           records[i].totalGames, records[i].wins, records[i].losses,
           records[i].draws, winRate);
  }
  printf(
      "========================================================================"
      "===========\n\n");
}

/**
 * updateLeaderboard - Update player record after game completion
 * @username: Player's username
 * @winner: Game result ('X'=player won, 'O'=player lost, 'D'=draw)
 * 
 * Process:
 * 1. Load player's current record
 * 2. Increment total games
 * 3. Update appropriate counter (wins/losses/draws)
 * 4. Save updated record back to file
 */
void updateLeaderboard(const char *username, char winner) {
  PlayerRecord record;
  loadPlayerRecord(username, &record);

  record.totalGames++;
  if (winner == 'X')      // Player wins
    record.wins++;
  else if (winner == 'O') // AI wins (player loses)
    record.losses++;
  else                    // Draw
    record.draws++;

  savePlayerRecord(&record);
}

/* ==================== STATISTICS FUNCTIONS ==================== */

/**
 * saveGameStats - Append game statistics to history file
 * @stats: Pointer to GameStats containing match details
 * 
 * Logs to file with format:
 * "Timestamp | Match: P1 vs P2 | Moves: N (P1: X, P2: Y) | AI Nodes: N | Depth: D | Winner: W"
 * 
 * File is appended (not overwritten) to maintain complete history
 */
void saveGameStats(const GameStats *stats) {
  FILE *file = fopen(STATS_FILE, "a");  // Append mode
  if (file == NULL) {
    printf("Error: Could not save game statistics.\n");
    return;
  }

  // Get current timestamp
  time_t now = time(NULL);
  char *timeStr = ctime(&now);
  timeStr[strlen(timeStr) - 1] = '\0'; // Remove newline from timestamp

  // Write formatted game statistics
  fprintf(file,
          "%s | Match: %s vs %s | Moves: %d (%s: %d, %s: %d) | AI Nodes: %d | "
          "Depth: %d | Winner: %c\n",
          timeStr, stats->player1, stats->player2, stats->totalMoves,
          stats->player1, stats->player1Moves, stats->player2,
          stats->player2Moves, stats->aiNodesExplored, stats->maxDepth,
          stats->winner);

  fclose(file);
}

/**
 * displayAllStats - Display complete game history
 * 
 * Shows all games with full details from statistics file
 * Games numbered sequentially for easy reference
 */
void displayAllStats() {
  FILE *file = fopen(STATS_FILE, "r");
  if (file == NULL) {
    printf("\n=== GAME STATISTICS ===\n");
    printf("No game statistics available yet!\n");
    printf("=======================\n\n");
    return;
  }

  printf("\n================================= GAME STATISTICS "
         "=================================\n");

  char line[512];
  int gameNum = 1;
  
  // Read and display each line from file
  while (fgets(line, sizeof(line), file) != NULL) {
    printf("Game %d: %s", gameNum++, line);
  }

  printf(
      "========================================================================"
      "===========\n\n");
  fclose(file);
}

/**
 * displayPlayerStats - Display game history for specific player
 * @username: Player's username to filter by
 * 
 * Shows only games where specified player participated
 * Searches for username in both player1 and player2 positions
 */
void displayPlayerStats(const char *username) {
  FILE *file = fopen(STATS_FILE, "r");
  if (file == NULL) {
    printf("\n=== STATISTICS FOR %s ===\n", username);
    printf("No game statistics available yet!\n");
    printf("============================\n\n");
    return;
  }

  printf("\n=========================== STATISTICS FOR %s "
         "===========================\n",
         username);

  char line[512];
  int gameNum = 1;
  int found = 0;

  // Read each line and check if username appears
  while (fgets(line, sizeof(line), file) != NULL) {
    // Create search patterns for both player positions
    char searchPattern1[MAX_USERNAME + 10];
    char searchPattern2[MAX_USERNAME + 10];
    snprintf(searchPattern1, sizeof(searchPattern1), "Match: %s vs", username);
    snprintf(searchPattern2, sizeof(searchPattern2), "vs %s |", username);

    // Check if username appears in either position
    if (strstr(line, searchPattern1) != NULL ||
        strstr(line, searchPattern2) != NULL) {
      printf("Game %d: %s", gameNum++, line);
      found = 1;
    }
  }

  if (!found) {
    printf("No games found for player: %s\n", username);
  }

  printf(
      "========================================================================"
      "===\n\n");
  fclose(file);
}

/* ==================== AI PERSONALITY FUNCTIONS ==================== */

/**
 * getAIName - Get AI opponent's name based on difficulty
 * @difficulty: Difficulty level (0=Easy, 1=Medium, 2=Hard)
 * 
 * Returns: AI name string
 * - 0: "Kitty" (Easy - playful, learning)
 * - 1: "Cop" (Medium - strategic, fair)
 * - 2: "Sera" (Hard - perfectionist, ruthless)
 */
const char *getAIName(int difficulty) {
  switch (difficulty) {
  case 0:
    return "Kitty";
  case 1:
    return "Cop";
  case 2:
    return "Sera";
  default:
    return "Unknown";
  }
}

/**
 * getAIQuote - Get contextual personality quote from AI
 * @difficulty: AI difficulty level (0-2)
 * @occasion: Context (0=intro, 1=win, 2=lose, 3=draw, 4=thinking)
 * 
 * Returns: Personality-appropriate quote string
 * 
 * Each AI has unique dialogue reflecting their character:
 * - Kitty: Cute, enthusiastic, still learning
 * - Cop: Professional, serious, law-and-order themed
 * - Sera: Confident, analytical, perfectionist
 */
const char *getAIQuote(int difficulty, int occasion) {
  // Occasion codes: 0=intro, 1=win, 2=lose, 3=draw, 4=thinking
  
  // Kitty (Easy) - Cute and playful personality
  static const char *kittyQuotes[5] = {
      "Meow~ Let's play! I'm still learning...", 
      "Yay! I won! *purrs happily*",
      "Aww... you beat me! Good game~", 
      "A tie? That's paw-some!",
      "Hmm... let me think... *paw on chin*"};

  // Cop (Medium) - Professional law enforcement theme
  static const char *copQuotes[5] = {
      "You have the right to make a move. I'll make mine.",
      "Justice served. Better luck next time, citizen.",
      "Hmph. Not bad. You win this round.",
      "A draw. I respect that. Fair play.",
      "Analyzing the situation... *adjusts sunglasses*"};

  // Sera (Hard) - Confident perfectionist personality
  static const char *seraQuotes[5] = {
      "Prepare yourself. I don't make mistakes.",
      "Predictable. Victory was inevitable.",
      "Impossible... You actually defeated me?",
      "A draw. Acceptable, but barely.",
      "Calculating optimal move... Child's play."};

  // Validate occasion parameter
  if (occasion < 0 || occasion > 4)
    occasion = 0;

  // Return appropriate quote based on difficulty
  switch (difficulty) {
  case 0:
    return kittyQuotes[occasion];
  case 1:
    return copQuotes[occasion];
  case 2:
    return seraQuotes[occasion];
  default:
    return "...";
  }
}
