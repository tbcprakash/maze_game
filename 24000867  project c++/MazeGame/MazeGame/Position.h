#pragma once // Ensures this header is included only once per compilation unit

// Represents a 2D coordinate in the game world.
// Using a struct as it's primarily a simple data container.
struct Position {
    int x; // The horizontal coordinate.
    int y; // The vertical coordinate.

    // Constructor with default values (0,0).
    // Uses a member initializer list for efficient and correct initialization.
    Position(int startX = 0, int startY = 0) : x(startX), y(startY) {}

    // Overloads the equality operator (==).
    // Why: Allows comparing two Position objects directly (e.g., playerPos == exitPos)
    // which is more readable and less error-prone than comparing x and y individually.
    // Marked 'const' because comparing positions doesn't change either position object.
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    // Optional: Overload the inequality operator for convenience.
    // Often defined in terms of operator==.
    bool operator!=(const Position& other) const {
        return !(*this == other); // Reuse the overloaded == operator
    }
};
