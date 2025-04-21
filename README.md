# Maze Game

A simple 2D maze game built in C++ that runs in the console. Control the player, collect items, avoid enemies, and reach the exit to win!

---

## 🎮 Game Overview

This project is a text-based 2D maze game developed using C++. The game runs directly in the terminal window. The goal is to guide your player (`P`) through different mazes, collect items (`*`), avoid enemies (`X`), and reach the exit (`E`) to complete each level.

---

## 🕹️ How to Play

- **W**: Move Up  
- **A**: Move Left  
- **S**: Move Down  
- **D**: Move Right  
- **Q**: Quit the game

---

## 🧱 Symbols in the Game

| Symbol | Meaning                                      |
|--------|----------------------------------------------|
| `#`    | Wall (Displayed as magenta colored blocks)   |
| (space)| Path (walkable area)                         |
| `P`    | Player (you)                                 |
| `*`    | Collectibles (increase your score)           |
| `X`    | Enemy (random movement — avoid them!)        |
| `E`    | Exit (reach this to complete the level)      |

---

## 📺 Game Display

While playing, the following will be shown on screen:

- The maze map with all characters/symbols
- Your current **Score**
- Total number of **Moves**
- The current **Level**

---

## 🔁 Game Flow

- **Complete a level** by reaching the exit (`E`)
- If there are more levels, the next level loads automatically
- **Winning**: Finish the last level to win the game
- **Losing**: Touch an enemy (`X`) or press `Q` to quit

---

## 🛠️ How It Works (Code Overview)

- **Language**: C++
- **Structure**:
  - Separated into `.h` (header) and `.cpp` (source) files for better organization
  - Uses **classes** to represent game components like `Player`, `Enemy`, and the overall `Game`
  - Inheritance is used: `Player` and `Enemy` inherit from a base `Entity` class
- **Level Design**:
  - Each maze/level is loaded from a `.txt` file
  - Easy to add new levels by just creating new map files
- **Console Output**:
  - Colored walls (magenta background)
  - Screen clearing and centering to improve visuals

---

## 📁 Project Files

