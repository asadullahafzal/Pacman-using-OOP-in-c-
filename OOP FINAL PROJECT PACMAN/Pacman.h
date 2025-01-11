#ifndef PACMAN_H
#define PACMAN_H

#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <string>
using namespace std;

// Forward declarations
class Game;
class Ghost;
class Pacman;

// Constants
const int ROWS = 24;
const int COLS = 71;


// Base Character class for common properties
class Character {
protected:
    int x;
    int y;
    char previousItem;
    HANDLE hConsole;

public:
    Character(int startX, int startY) : x(startX), y(startY), previousItem(' ') {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int newX, int newY) { x = newX; y = newY; }
};

// Pacman class
class Pacman : public Character {
protected:
    int score;
    int lives;
    bool powerUpActive;
    int powerUpTimer;

public:
    Pacman() : Character(9, 31), score(0),lives(1), powerUpActive(false), powerUpTimer(0) {}

    void moveLeft(char maze[ROWS][COLS]);
    void moveRight(char maze[ROWS][COLS]);
    void moveUp(char maze[ROWS][COLS]);
    void moveDown(char maze[ROWS][COLS]);

    int getScore() const { return score; }
    void incrementScore() { score++; }
    int getLives() const { return lives; }
    void decrementLives() { lives--; };
    void resetPosition() { x = 9; y = 31; }

    void activatePowerUp() {
        powerUpActive = true;
        powerUpTimer = 100; // 200 game cycles of power-up
    }
    bool isPowerUpActive() const { return powerUpActive; }
    void updatePowerUp() {
        if (powerUpActive) {
            powerUpTimer--;
            if (powerUpTimer <= 0) {
                powerUpActive = false;
            }
        }
    }
    int getPowerUpTimer() const { return powerUpTimer; }
};


// Ghost base class
class Ghost : public Character {
protected:
    int flag;
    bool isRunningAway;
    int initialX;  // Store initial X position
    int initialY;

public:
    Ghost(int startX, int startY) : Character(startX, startY), flag(0), isRunningAway(false), initialX(startX),
        initialY(startY) {}
    void resetToInitialPosition(char maze[ROWS][COLS]);
    virtual void move(char maze[ROWS][COLS], const Pacman& pacman) = 0;
    virtual void runAway(char maze[ROWS][COLS], const Pacman& pacman) = 0;
};

// Horizontal Ghost
class Blinky : public Ghost {
public:
    Blinky() : Ghost(16, 31) {}
    void move(char maze[ROWS][COLS], const Pacman& pacman) override;
    void runAway(char maze[ROWS][COLS], const Pacman& pacman)override;
};

// Vertical Ghost
class Pinky : public Ghost {
public:
    Pinky() : Ghost(1, 29) {}
    void move(char maze[ROWS][COLS], const Pacman& pacman) override;
    void runAway(char maze[ROWS][COLS], const Pacman& pacman)override;
};

// Random Ghost
class Clyde : public Ghost {
public:
    Clyde() : Ghost(1, 60) {}
    void move(char maze[ROWS][COLS], const Pacman& pacman) override;
    void runAway(char maze[ROWS][COLS], const Pacman& pacman)override;
private:
    int getRandomDirection();
};

// Smart Ghost
class Inky : public Ghost {
public:
    Inky() : Ghost(5, 5) {}
    void move(char maze[ROWS][COLS], const Pacman& pacman) override;
    void runAway(char maze[ROWS][COLS], const Pacman& pacman)override;
};

// Game class to manage the game state
class Game {
private:
    char maze[ROWS][COLS];
    Pacman pacman;
    Blinky ghost1;
    Pinky ghost2;
    Clyde ghost3;
    Inky ghost4;
    bool playingGame;
    static HANDLE hConsole;
    int powerUpCoinX;
    int powerUpCoinY;
    bool powerUpCoinActive;
    int powerUpCoinTimer;

public:
    Game();
    void readMaze();
    void printMaze();
    void play();
    void displayMenu();
    void welcome();
    void displayGameDescription();
    void displayScoreboard();
    void displayGameOver();
    void displayWinner();
    bool checkCollision();
    static void gotoxy(int x, int y);

    void spawnPowerUpCoin();
    void removePowerUpCoin();
    bool checkPowerUpCoinCollision();
    bool checkGhostCollisionInPowerMode(Ghost* ghost1, Ghost* ghost2);
    void resolveGhostCollision(char maze[ROWS][COLS], Ghost* ghost1, Ghost* ghost2);
};

inline void SetConsoleBackground(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Retrieve console screen buffer info
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

    DWORD consoleSize = consoleInfo.dwSize.X * consoleInfo.dwSize.Y; // Total number of cells
    DWORD charsWritten;

    // Fill the console with the background color
    FillConsoleOutputAttribute(hConsole, color, consoleSize, { 0, 0 }, &charsWritten);

    // Fill the console with spaces (to make the background visible)
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, { 0, 0 }, &charsWritten);

    // Reset the cursor position to the top-left corner
    SetConsoleCursorPosition(hConsole, { 0, 0 });
}


#endif // !PACMAN_H