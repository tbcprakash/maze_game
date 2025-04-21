#pragma once // Standard include guard.

#include <vector>   // For std::vector (maze, enemies).
#include <string>   // For std::string (maze rows).
#include "Player.h" // Include Player class definition.
#include "Enemy.h"  // Include Enemy class definition.
#include "Position.h" // Include Position struct definition.

// Manages the overall game state, logic, and interaction.
// Acts as the central controller for the maze game.
class Game {
private:
    // --- Game Data ---
    std::vector<std::string> maze; // Stores the static layout of the current level (walls, paths, original collectible locations).
    Player player;                 // The player object (contains position, score, moves).
    std::vector<Enemy> enemies;    // A list holding all enemy objects for the current level.
    Position exitPos;              // Coordinates of the level's exit 'E'.
    int currentLevel;              // Tracks the current level number (e.g., 1, 2, ...).
    int maxLevels;                 // The total number of levels available.
    bool gameOver;                 // Flag indicating if the current level loop should end (due to win, loss, or quit).
    bool playerWonLevel;           // Flag set specifically when the player reaches the exit.
    bool playerLost;               // Flag set specifically when the player collides with an enemy or quits.

    // --- Private Helper Methods ---
    // Encapsulate internal logic, not meant to be called directly from outside the Game class.

    // Clears the console screen (platform-dependent). Marked 'const'.
    void clearScreen() const;

    // Renders the current state of the maze, player, enemies, etc., to the console. Marked 'const'.
    void displayMaze() const;

    // Handles reading player input from the keyboard.
    void handleInput();

    // Updates the game state after player input (e.g., moves enemies, checks for collisions).
    void updateGame();

    // Loads the specified level number from its corresponding file (e.g., "level1.txt").
    // Returns true on success, false on failure (e.g., file not found).
    bool loadLevel(int levelNumber);

    // Scans the loaded maze data to find initial positions of 'P', 'E', 'X'
    // and configures the player/enemies/exitPos accordingly.
    void findStartPositions();

public:
    // --- Public Interface ---

    // Constructor: Initializes the game, potentially setting the max number of levels.
    Game(int numberOfLevels = 6); // Default to 6 levels if not specified.

    // Starts and manages the main game loop, coordinating level loading and gameplay.
    void run();

    // Potential future methods (not implemented based on current spec):
    // void saveGameState(const std::string& filename); // Saves player pos, score, level etc.
    // void loadGameState(const std::string& filename); // Loads a saved game state.
};