#include "Entity.h"

// Constructor implementation.
// Uses a member initializer list to initialize 'pos' and 'symbol'.
// Why Initializer List: More efficient than assignment in the constructor body,
// and required for initializing const members or reference members (if any).
Entity::Entity(int startX, int startY, char sym) : pos(startX, startY), symbol(sym) {
    // Constructor body is empty as initialization is done in the initializer list.
}

// Getter implementation for Position.
// Marked 'const' because it doesn't change the Entity's state.
Position Entity::getPosition() const {
    return pos;
}

// Getter implementation for Symbol.
// Marked 'const' because it doesn't change the Entity's state.
char Entity::getSymbol() const {
    return symbol;
}

// Setter implementation for Position (using coordinates).
void Entity::setPosition(int newX, int newY) {
    pos.x = newX;
    pos.y = newY;
}

// Setter implementation for Position (using a Position object).
// Passes 'newPos' by 'const reference' for efficiency (avoids copying the Position object).
void Entity::setPosition(const Position& newPos) {
    pos = newPos; // Uses Position's default assignment operator.
}