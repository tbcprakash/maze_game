#include "Enemy.h"
#include <chrono> // Needed for seeding the random number generator using time.

// Constructor implementation.
// Calls the base Entity constructor to set position and symbol ('X').
// Seeds the enemy's unique random number generator (rng).
// Why Seed: Ensures that enemy movement is different each time the game runs.
// Using time provides a reasonably unpredictable seed value.
Enemy::Enemy(int startX, int startY) : Entity(startX, startY, 'X') {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(seed);
}

// Enemy random movement logic.
void Enemy::moveRandomly(const std::vector<std::string>& maze) {
    // Define a distribution to generate random integers between 0 and 3 (inclusive),
    // representing the four possible move directions (Up, Down, Left, Right).
    std::uniform_int_distribution<int> distribution(0, 3);

    // Limit attempts to prevent potential infinite loops if an enemy gets completely boxed in.
    const int MAX_MOVE_ATTEMPTS = 10;
    for (int attempts = 0; attempts < MAX_MOVE_ATTEMPTS; ++attempts) {

        int direction = distribution(rng); // Generate a random direction (0-3).
        int nextX = pos.x; // Start with current position.
        int nextY = pos.y;

        // Calculate potential next position based on the random direction.
        switch (direction) {
        case 0: nextY--; break; // Up
        case 1: nextY++; break; // Down
        case 2: nextX--; break; // Left
        case 3: nextX++; break; // Right
        }

        // --- Collision and Boundary Detection ---

        // 1. Check boundaries.
        if (nextY < 0 || nextY >= maze.size() || nextX < 0 || nextX >= maze[0].size()) {
            continue; // Invalid move (out of bounds), try another direction.
        }

        // 2. Check target cell type.
        char targetCell = maze[nextY][nextX];
        if (targetCell == '#') { // Check for wall collision.
            continue; // Invalid move (wall), try another direction.
        }

        // Allow movement only onto empty path tiles (' ') for this simple AI.
        // Could be expanded later to allow moving onto collectibles ('*') if desired.
        if (targetCell == ' ') {
            setPosition(nextX, nextY); // Update position using base class setter.
            return; // Successful move, exit the function.
        }

        // If the target cell wasn't empty space or a wall (e.g., could be '*', 'E' in future),
        // this attempt is considered invalid for this simple AI. Try again.
    }
    // If loop finishes (MAX_MOVE_ATTEMPTS reached), the enemy didn't find a valid move this turn.
}