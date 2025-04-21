#pragma once

#include "Entity.h" // Include the base class header.
#include <vector>   // Needed for maze data access during movement.
#include <string>   // Needed for maze data type.
#include <random>   // Include for C++ random number generation.

// Represents an enemy character, inheriting from Entity.
// Why Inheritance: Enemy *is an* Entity, sharing position and symbol.
// Adds Enemy-specific behavior (random movement).
class Enemy : public Entity { // 'public' inheritance.
private:
    // C++11 random number generation engine. Each enemy has its own.
    // Why <random>: Preferred over C-style rand() for better statistical properties
    // and control over distributions.
    std::mt19937 rng; // Mersenne Twister engine instance.

public:
    // Constructor for Enemy.
    // Initializes the base Entity part with position and 'X' symbol.
    // Seeds the random number generator.
    Enemy(int startX = 0, int startY = 0);

    // --- Movement ---
    // Implements the enemy's random movement logic.
    // Takes the maze layout (`const&`) to check for valid moves (walls/boundaries).
    // Not 'const' because it modifies the enemy's position.
    void moveRandomly(const std::vector<std::string>& maze);
};