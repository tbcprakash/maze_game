#include "Game.h"
#include <iostream>
#include <fstream>
#include <conio.h>   // For _getch() [Windows specific non-blocking input]
#include <cstdlib>   // For system() [cls/clear]
#include <vector>
#include <string>
#include <thread>    // Required for std::this_thread::sleep_for [pausing]
#include <chrono>    // Required for std::chrono::seconds [pausing]
#include <cctype>    // Required for toupper()
#include <sstream>   // Required for std::ostringstream (to format strings for centering)

// --- Platform Specific Includes & Defines ---
#ifdef _WIN32 // Only include and use for Windows builds
#define NOMINMAX
#include <windows.h>
#endif
// --- End Platform Specific Includes ---


// --- ANSI Escape Codes for Console Colors ---
// Provide visual feedback directly in the console.
const std::string ANSI_RESET = "\033[0m";      // Resets all text attributes (color, background)
const std::string ANSI_BG_MAGENTA = "\033[45m"; // Magenta background for walls
// Optional: Add more colors if desired
// const std::string ANSI_FG_YELLOW = "\033[93m"; // Bright Yellow text
// const std::string ANSI_FG_CYAN = "\033[96m";   // Bright Cyan text
// const std::string ANSI_FG_RED_BRIGHT = "\033[91m"; // Bright Red text
// const std::string ANSI_FG_GREEN = "\033[92m";  // Bright Green text


// --- Helper function to get console dimensions (Windows specific) ---
struct ConsoleDimensions {
    int Width;
    int Height;
};

ConsoleDimensions GetConsoleWindowSize() {
    ConsoleDimensions dims = { 80, 25 }; // Default values if detection fails
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            // Use window dimensions for better centering if user resizes window
            dims.Width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            dims.Height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        }
    }
#else
    // Basic fallback for non-Windows - requires manual adjustment or
    // using libraries like ncurses or ioctl for Linux/macOS
    // dims = {80, 24}; // Keep default
#endif
    // Ensure minimum width to avoid calculation issues
    if (dims.Width < 1) dims.Width = 1;
    return dims;
}
// --- End Helper Function ---


// Constructor Implementation
// Initializes game settings using a member initializer list.
Game::Game(int numberOfLevels)
    : player(0, 0),           // Player constructor called (initial pos is dummy)
    currentLevel(1),        // Start at level 1
    maxLevels(numberOfLevels),// Set max levels from argument
    gameOver(false),        // Game not over initially
    playerWonLevel(false),  // Haven't won yet
    playerLost(false),      // Haven't lost yet
    exitPos(-1, -1)         // Initialize exitPos to an invalid state until level loaded
{
    // Constructor body can be empty if all initialization is done above.
}

// Platform-specific screen clearing implementation.
void Game::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Loads maze data and initializes level state from a text file.
bool Game::loadLevel(int levelNumber) {
    std::string filename = "level" + std::to_string(levelNumber) + ".txt";
    std::ifstream levelFile(filename);

    if (!levelFile.is_open()) {
        std::cerr << "Error: Could not open level file: " << filename << std::endl;
        return false;
    }

    maze.clear();
    enemies.clear();
    player.reset();
    gameOver = false;
    playerWonLevel = false;
    playerLost = false;
    exitPos = Position(-1, -1);

    std::string line;
    while (getline(levelFile, line)) {
        maze.push_back(line);
    }
    levelFile.close();

    if (maze.empty() || maze[0].empty()) {
        std::cerr << "Error: Maze file is empty or invalid: " << filename << std::endl;
        return false;
    }

    findStartPositions();

    if (player.getPosition() == Position(-1, -1) || exitPos == Position(-1, -1)) {
        std::cerr << "Warning: Player 'P' or Exit 'E' not found in " << filename << ". Level might be unplayable." << std::endl;
    }

    currentLevel = levelNumber;
    return true;
}

// Finds starting positions of 'P', 'E', 'X' in the maze data.
void Game::findStartPositions() {
    Position playerStart(-1, -1);

    for (int y = 0; y < maze.size(); ++y) {
        for (int x = 0; x < maze[y].size(); ++x) {
            char cell = maze[y][x];
            if (cell == 'P') {
                playerStart = Position(x, y);
                maze[y][x] = ' ';
            }
            else if (cell == 'E') {
                exitPos = Position(x, y);
                maze[y][x] = ' ';
            }
            else if (cell == 'X') {
                enemies.emplace_back(x, y);
                maze[y][x] = ' ';
            }
        }
    }
    if (playerStart != Position(-1, -1)) {
        player.setPosition(playerStart);
    }
    else {
        player.setPosition(0, 0);
        std::cerr << "Error: Player 'P' start position not found in level data!" << std::endl;
    }
}


// --- displayMaze function with CENTERED HEADER and Double Width Maze ---
void Game::displayMaze() const {
    // 1. Get Console Dimensions
    ConsoleDimensions consoleSize = GetConsoleWindowSize();
    int consoleWidth = consoleSize.Width;
    int consoleHeight = consoleSize.Height;

    // 2. Calculate Content Dimensions (for vertical centering)
    const int headerLines = 5; // Lines for Title, Score, Moves, Instructions, Blank line
    const int footerLines = 1; // Blank line at the bottom
    int mazeHeight = maze.size(); // Number of rows in the maze data
    int totalContentHeight = headerLines + mazeHeight + footerLines;

    // 3. Calculate Vertical Padding
    int topPadding = (consoleHeight > totalContentHeight) ? (consoleHeight - totalContentHeight) / 2 : 0;

    // --- Helper Lambda for Centering Text ---
    // Takes a string and calculates the left padding needed to center it.
    auto getCenteredIndent = [&](const std::string& text) {
        int textLength = text.length();
        int padding = (consoleWidth > textLength) ? (consoleWidth - textLength) / 2 : 0;
        return std::string(padding, ' ');
        };
    // --- End Helper Lambda ---

    // 4. Clear Screen and Apply Top Padding
    clearScreen();
    for (int i = 0; i < topPadding; ++i) {
        std::cout << std::endl;
    }

    // 5. Print CENTERED Header Information
    std::string titleText = "--- Maze Game --- Level: " + std::to_string(currentLevel) + " ---";
    std::cout << getCenteredIndent(titleText) << titleText << "\n";

    // Use ostringstream to format score/moves before centering
    std::ostringstream scoreMovesStream;
    scoreMovesStream << "Score: " << player.getScore() << "   Moves: " << player.getMoves();
    std::string scoreMovesText = scoreMovesStream.str();
    std::cout << getCenteredIndent(scoreMovesText) << scoreMovesText << "\n";

    std::string instructions1 = "Use W, A, S, D to move. Reach 'E' to win! ('Q' to Quit)";
    std::cout << getCenteredIndent(instructions1) << instructions1 << "\n";

    std::string instructions2 = "'#'=Wall(Magenta Block), ' '=Path, '*'=Collectible, 'X'=Enemy, 'P'=Player, 'E'=Exit";
    std::cout << getCenteredIndent(instructions2) << instructions2 << "\n";

    std::cout << "\n"; // Blank line after instructions


    // 6. Prepare and Print the Maze (Centered Horizontally, Double Width)
    if (maze.empty() || maze[0].empty()) {
        std::cout << getCenteredIndent("(Error: Maze data is empty)") << "(Error: Maze data is empty)\n";
        return;
    }

    int mazeDataWidth = maze[0].size();
    int mazeDisplayWidth = mazeDataWidth * 2; // Double width for display
    int leftPaddingMaze = (consoleWidth > mazeDisplayWidth) ? (consoleWidth - mazeDisplayWidth) / 2 : 0;
    std::string mazeIndent(leftPaddingMaze, ' ');


    std::vector<std::string> displayGrid = maze; // Start with base layout

    // Place dynamic entities onto the displayGrid (same logic as before)
    Position pPos = player.getPosition();
    if (pPos.y >= 0 && pPos.y < displayGrid.size() && pPos.x >= 0 && pPos.x < mazeDataWidth) {
        displayGrid[pPos.y][pPos.x] = player.getSymbol();
    }
    for (const auto& enemy : enemies) {
        Position ePos = enemy.getPosition();
        if (ePos.y >= 0 && ePos.y < displayGrid.size() && ePos.x >= 0 && ePos.x < mazeDataWidth) {
            if (displayGrid[ePos.y][ePos.x] != player.getSymbol()) {
                displayGrid[ePos.y][ePos.x] = enemy.getSymbol();
            }
        }
    }
    if (exitPos.y >= 0 && exitPos.y < displayGrid.size() && exitPos.x >= 0 && exitPos.x < mazeDataWidth) {
        char currentCell = displayGrid[exitPos.y][exitPos.x];
        if (currentCell != player.getSymbol() && currentCell != 'X') {
            displayGrid[exitPos.y][exitPos.x] = 'E';
        }
    }

    // Iterate and print MAZE character by character with colors AND DOUBLE WIDTH
    for (const auto& row : displayGrid) {
        std::cout << mazeIndent; // Apply left padding for the maze row
        for (char cell : row) {
            // Check the character and print TWO characters for each cell
            switch (cell) {
            case '#': // Wall
                std::cout << ANSI_BG_MAGENTA << "  " << ANSI_RESET; // Magenta background, TWO spaces
                break;
            case 'P': // Player
                std::cout << " P"; // Space then P (adjust as desired: "P ", "PP")
                break;
            case 'X': // Enemy
                std::cout << " X"; // Space then X
                break;
            case 'E': // Exit
                std::cout << " E"; // Space then E
                break;
            case '*': // Collectible
                std::cout << " *"; // Space then *
                break;
            case ' ': // Path
                std::cout << "  "; // Two spaces for path
                break;
            default:  // Any other unexpected characters
                std::cout << cell << ' '; // Print char and a space
                break;
            }
        }
        std::cout << std::endl; // Newline after printing all characters in a row
    }
    std::cout << std::endl; // Add final blank line (footer)
}
// --- End displayMaze function ---


// Handles player keyboard input.
void Game::handleInput() {
    char input = _getch();
    char direction = std::toupper(input);

    if (direction == 'W' || direction == 'A' || direction == 'S' || direction == 'D') {
        bool moved = player.move(direction, maze);
        if (moved) {
            Position newPos = player.getPosition();
            if (!maze.empty() && !maze[0].empty() &&
                newPos.y >= 0 && newPos.y < maze.size() && newPos.x >= 0 && newPos.x < maze[0].size() &&
                maze[newPos.y][newPos.x] == '*')
            {
                player.increaseScore(10);
                maze[newPos.y][newPos.x] = ' ';
            }
        }
    }
    else if (direction == 'Q') {
        gameOver = true;
        playerLost = true;
    }
}

// Updates game state after player input, e.g., enemy movement, collision checks.
void Game::updateGame() {
    for (auto& enemy : enemies) {
        enemy.moveRandomly(maze);
    }

    Position playerPos = player.getPosition();

    for (const auto& enemy : enemies) {
        if (playerPos == enemy.getPosition()) {
            gameOver = true;
            playerLost = true;
            return;
        }
    }

    if (playerPos == exitPos) {
        playerWonLevel = true;
        gameOver = true;
        return;
    }
}

// The main execution function that orchestrates the game flow.
void Game::run() {
    while (currentLevel <= maxLevels) {
        if (!loadLevel(currentLevel)) {
            std::cerr << "Critical Error: Failed to load level " << currentLevel << ". Exiting game." << std::endl;
            std::cout << "Press any key to exit." << std::endl;
            _getch();
            return;
        }

        gameOver = false;
        playerWonLevel = false;
        playerLost = false;

        while (!gameOver) {
            displayMaze();
            handleInput();
            if (!gameOver) {
                updateGame();
            }
        }

        displayMaze(); // Show final state

        if (playerWonLevel) {
            std::cout << "\n*******************************\n";
            std::cout << "*      Level " << currentLevel << " Cleared!      *\n";
            std::cout << "*******************************\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));

            if (currentLevel < maxLevels) {
                std::cout << "Press any key to start Level " << (currentLevel + 1) << "..." << std::endl;
                _getch();
                currentLevel++;
            }
            else {
                std::cout << "\n****************************************\n";
                std::cout << "* Congratulations! You beat the game!  *\n";
                std::cout << "****************************************\n";
                std::cout << "Final Score: " << player.getScore() << ", Total Moves: " << player.getMoves() << std::endl;
                break;
            }
        }
        else if (playerLost) {
            std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            std::cout << "!          GAME OVER              !\n";
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            std::cout << "Final Score: " << player.getScore() << ", Moves: " << player.getMoves() << std::endl;
            break;
        }
    } // End of outer game loop

    std::cout << "\nThanks for playing!\n";
    std::cout << "Press any key to exit." << std::endl;
    _getch();
}