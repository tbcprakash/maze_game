#include "Player.h"
#include <iostream> // Included for potential debugging, can be removed if not used.
#include <cctype>   // Included for toupper()

// Constructor implementation.
// Calls the base class (Entity) constructor using the initializer list
// to set the starting position and the Player's specific symbol ('P').
// Also initializes score and moves to 0.
Player::Player(int startX, int startY) : Entity(startX, startY, 'P'), score(0), moves(0) {}

// Getter for score. Marked 'const'.
int Player::getScore() const {
    return score;
}

// Getter for moves. Marked 'const'.
int Player::getMoves() const {
    return moves;
}

// Increases the player's score.
void Player::increaseScore(int amount) {
    // Basic addition, could add checks later (e.g., prevent negative scores).
    score += amount;
}

// Increments the player's move count.
void Player::incrementMoves() {
    moves++;
}

// Resets player-specific stats, typically called when starting a new level.
// Position is usually reset by the Game class when loading the level.
void Player::reset() {
    score = 0;
    moves = 0;
}

// Player movement logic.
// Takes direction ('W','A','S','D') and the maze map.
bool Player::move(char direction, const std::vector<std::string>& maze) {
    int nextX = pos.x; // Start with current position.
    int nextY = pos.y;

    // Calculate the potential target coordinates based on input direction.
    // using toupper to handle both lowercase and uppercase input.
    switch (std::toupper(direction)) {
    case 'W': nextY--; break; // Move Up
    case 'S': nextY++; break; // Move Down
    case 'A': nextX--; break; // Move Left
    case 'D': nextX++; break; // Move Right
    default:
        return false; // Invalid direction character received.
    }

    // --- Collision and Boundary Detection ---

    // 1. Check if the calculated next position is outside the maze boundaries.
    // Uses .size() for rows (y) and maze[0].size() for columns (x), assuming a rectangular maze.
    if (nextY < 0 || nextY >= maze.size() || nextX < 0 || nextX >= maze[0].size()) {
        return false; // Cannot move outside the map.
    }

    // 2. Check if the target cell in the maze contains a wall ('#').
    // Accessing maze data using calculated nextY and nextX coordinates.
    if (maze[nextY][nextX] == '#') {
        return false; // Cannot move into a wall.
    }

    // --- Update State ---
    // If all checks pass, the move is valid.

    // Update the player's position using the base class setter.
    setPosition(nextX, nextY);
    // Increment the move counter for this successful move.
    incrementMoves();
    // Return true indicating the player moved.
    return true;
}