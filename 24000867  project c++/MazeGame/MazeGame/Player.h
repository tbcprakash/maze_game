#pragma once

#include "Entity.h" // Include the base class header.
#include <vector>   // Needed for passing the maze data for collision checking.
#include <string>   // Needed for maze data type (std::vector<std::string>).

// Represents the player character, inheriting from Entity.
// Why Inheritance: Player *is an* Entity, sharing position and symbol properties.
// We only need to add Player-specific attributes and behaviors.
class Player : public Entity { // 'public' inheritance means public members of Entity remain public.
private:
    int score; // Player's current score.
    int moves; // Number of moves made by the player.

public:
    // Constructor for Player.
    // Initializes the base Entity part using the Entity constructor in the initializer list.
    Player(int startX = 0, int startY = 0); // Default constructor arguments if needed.

    // --- Getters ---
    // Provide read-only access to Player-specific state.
    // Marked 'const' as they don't modify the Player object.
    int getScore() const;
    int getMoves() const;

    // --- Actions ---
    // Methods specific to modifying the Player's state.
    void increaseScore(int amount); // Adds to the player's score.
    void incrementMoves();        // Increases the move counter.
    void reset();                 // Resets score and moves (e.g., for a new level).

    // --- Movement ---
    // Handles player movement logic.
    // Takes the maze layout (`const&` for efficiency) to check for walls.
    // Returns 'true' if the player successfully moved, 'false' otherwise (e.g., hit a wall).
    // Not marked 'const' because it modifies the player's position and move count.
    bool move(char direction, const std::vector<std::string>& maze);
};