#pragma once

#include "Position.h" // Entity requires Position to store its location.

// Represents the base class for all movable objects in the game.
// Why Inheritance: Avoids duplicating position and symbol logic in Player and Enemy.
// Provides a common interface (though minimal here) for game entities.
class Entity {
protected: // 'protected' allows derived classes (Player, Enemy) to access these directly.
    Position pos;    // The entity's current position on the map.
    char symbol;   // The character symbol used to represent the entity on screen.

public:
    // Constructor for the base Entity.
    // Takes starting coordinates and the display symbol.
    // Uses a member initializer list.
    Entity(int startX, int startY, char sym);

    // Virtual destructor.
    // Why virtual: Ensures that if a derived class object (like Player or Enemy)
    // is deleted through a pointer to the base class (Entity*), the correct
    // derived class destructor is called first. Essential for preventing memory leaks
    // if derived classes managed resources (though not strictly needed here, it's crucial base class practice).
    // '= default' means the compiler generates the standard destructor behavior.
    virtual ~Entity() = default;

    // --- Getters ---
    // Provide read-only access to the entity's state.
    // Marked 'const' because they do not modify the Entity object's state.

    Position getPosition() const; // Returns the current position.
    char getSymbol() const;     // Returns the display symbol.

    // --- Setters ---
    // Allow modifying the entity's position.

    void setPosition(int newX, int newY);        // Set position using coordinates.
    void setPosition(const Position& newPos); // Set position using another Position object.
};