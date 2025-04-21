#include "Game.h"   // Include the Game class definition.
#include <iostream> // Standard Input/Output streams.

// --- Windows Specific Setup for ANSI Colors ---
// Necessary for ANSI escape codes (like colors) to work in standard
// Windows cmd.exe or PowerShell consoles. Does nothing on other platforms.
#ifdef _WIN32
#define NOMINMAX // Prevent conflicts with Windows headers defining min/max macros.
#include <windows.h> // Include Windows API header.
#endif

// Function to attempt enabling virtual terminal processing for ANSI codes on Windows.
void EnableVirtualTerminalProcessing() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // Get the console output handle.
    if (hOut == INVALID_HANDLE_VALUE) {
        // Failed to get handle, cannot proceed.
        return;
    }
    DWORD dwMode = 0;
    // Get the current console mode.
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }
    // Add the ENABLE_VIRTUAL_TERMINAL_PROCESSING flag to the current mode.
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    // Try to set the new console mode.
    if (!SetConsoleMode(hOut, dwMode)) {
        // Setting the mode failed, possibly an older Windows version. Colors might not work.
    }
#endif
}
// --- End of Windows Specific Setup ---


int main() {
    // --- Enable ANSI colors on Windows (MUST be called before printing colors) ---
    EnableVirtualTerminalProcessing();

    // Create the main Game object.
    // Pass the total number of level files (e.g., 6 if you have level1.txt to level6.txt).
    Game mazeGame(6); // Update this number if you add/remove level files!

    // Start the game execution by calling the run() method.
    mazeGame.run();

    // Return 0 to indicate successful program execution to the operating system.
    return 0;
}