#include "Pacman.h"
using namespace std;

HANDLE Game::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Definition

void Pacman::moveLeft(char maze[ROWS][COLS]) {
    if (maze[x][y - 1] == ' ' || maze[x][y - 1] == '.' || maze[x][y - 1] == '*') {
        if (maze[x][y - 1] == '.') {    //agr tu khali space ya dot hai tu wo move krey or dot say score increment ho
            incrementScore();
        }
        if (maze[x][y - 1] == '*') {
            activatePowerUp();
        }
        maze[x][y] = ' ';
        Game::gotoxy(y, x);
        cout << ' ';
        y--;
        SetConsoleTextAttribute(hConsole, 11);
        Game::gotoxy(y, x);
        cout << '>';
        SetConsoleTextAttribute(hConsole, 15);
    }
}

void Blinky::move(char maze[ROWS][COLS], const Pacman& pacman) {
    if (!pacman.isPowerUpActive()) {
        char pre;
        if ((maze[x][y - 1] == ' ' || maze[x][y - 1] == '.') && flag == 1) {
            pre = maze[x][y];
            maze[x][y] = pre;
            Game::gotoxy(y, x);
            cout << pre;
            y--;
            SetConsoleTextAttribute(hConsole, 04);
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
        }
        else {
            flag = 0;
        }

        if ((maze[x][y + 1] == ' ' || maze[x][y + 1] == '.') && flag == 0) {
            pre = maze[x][y];
            maze[x][y] = pre;
            Game::gotoxy(y, x);
            cout << pre;
            y++;
            SetConsoleTextAttribute(hConsole, 04);
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
        }
        else {
            flag = 1;
        }
    }
    else {
        runAway(maze, pacman);
    }

}

void Blinky::runAway(char maze[ROWS][COLS], const Pacman& pacman) {
    isRunningAway = true;
    char pre;

    // Determine direction to run away from Pacman
    int dx = (pacman.getX() > x) ? -1 : 1;
    int dy = (pacman.getY() > y) ? -1 : 1;

    // Try to move away horizontally first
    if (maze[x][y + dy] == ' ' || maze[x][y + dy] == '.') {
        pre = maze[x][y];
        maze[x][y] = pre;
        Game::gotoxy(y, x);
        cout << pre;
        y += dy;
        SetConsoleTextAttribute(hConsole, 12); // Bright red when running
        Game::gotoxy(y, x);
        cout << '0';
        SetConsoleTextAttribute(hConsole, 15);
    }
    // If horizontal movement fails, try vertical
    else if (maze[x + dx][y] == ' ' || maze[x + dx][y] == '.') {
        pre = maze[x][y];
        maze[x][y] = pre;
        Game::gotoxy(y, x);
        cout << pre;
        x += dx;
        SetConsoleTextAttribute(hConsole, 12); // Bright red when running
        Game::gotoxy(y, x);
        cout << '0';
        SetConsoleTextAttribute(hConsole, 15);
    }
}



Game::Game() : playingGame(true), powerUpCoinActive(false), powerUpCoinTimer(0) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    readMaze();
}

void Ghost::resetToInitialPosition(char maze[ROWS][COLS]) {
    x = initialX;
    y = initialY;
    isRunningAway = false;
    flag = 0;

    // Update maze and display
    Game::gotoxy(y, x);
    SetConsoleTextAttribute(hConsole, 15);
    cout << '0';
}


void Game::play() {
    system("cls");
    SetConsoleBackground(BACKGROUND_GREEN | BACKGROUND_RED);
    printMaze();
    int powerUpSpawnCounter = 0;
    gotoxy(pacman.getY(), pacman.getX());
    cout << 'P';

    while (playingGame) {
        Sleep(100);

        pacman.updatePowerUp();

        powerUpSpawnCounter++;
        if (!powerUpCoinActive && powerUpSpawnCounter >= 500) {
            spawnPowerUpCoin();
            powerUpSpawnCounter = 0;
        }

        // Check power-up coin collision
        checkPowerUpCoinCollision();

        ghost1.move(maze, pacman);
        ghost2.move(maze, pacman);
        ghost3.move(maze, pacman);
        ghost4.move(maze, pacman);

        displayScoreboard();

        if (checkCollision()) {
            playingGame = false;
        }

        if (GetAsyncKeyState(VK_LEFT)) {
            pacman.moveLeft(maze);
        }
        if (GetAsyncKeyState(VK_RIGHT)) {
            pacman.moveRight(maze);
        }
        if (GetAsyncKeyState(VK_UP)) {
            pacman.moveUp(maze);
        }
        if (GetAsyncKeyState(VK_DOWN)) {
            pacman.moveDown(maze);
        }
        if (GetAsyncKeyState(VK_ESCAPE)) {
            playingGame = false;
        }
        if (pacman.getScore() == 100) {
            displayWinner();
            playingGame = false;
        }
        if (pacman.isPowerUpActive() && pacman.getPowerUpTimer() == 0) {
            ghost1.resetToInitialPosition(maze);
            ghost2.resetToInitialPosition(maze);
            ghost3.resetToInitialPosition(maze);
            ghost4.resetToInitialPosition(maze);
        }
    }
}


void Game::gotoxy(int x, int y) {
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

void Game::readMaze() {
    try {
    fstream file;
    string line;
    file.open("maze.txt", ios::in);
    if (!file.is_open()) throw std::runtime_error("Could not open maze.txt");
    for (int x = 0; x < ROWS; x++) {
        getline(file, line);
        for (int y = 0; y < COLS; y++) {
            maze[x][y] = line[y];
        }
    }
    file.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Error reading maze: " << e.what() << std::endl;
    }
}

void Game::printMaze() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == '#') {
                SetConsoleTextAttribute(hConsole, 10); // Green color
                cout << '#';
                SetConsoleTextAttribute(hConsole, 15);
            }
            else if (maze[i][j] == '.') {
                SetConsoleTextAttribute(hConsole, 14); // Yellow color
                cout << '.';
                SetConsoleTextAttribute(hConsole, 15);
            }
            else if (maze[i][j] == '*') {
                SetConsoleTextAttribute(hConsole, 13); // Magenta color
                cout << '*';
                SetConsoleTextAttribute(hConsole, 15);
            }
            else {
                cout << maze[i][j];
            }
        }
        cout << endl;
    }
}

void Game::displayScoreboard() {
    gotoxy(90, 10);
    cout << "Score : " << pacman.getScore() << endl;
    gotoxy(90, 11);
    cout << "Lives : " << pacman.getLives() << endl;

    if (pacman.isPowerUpActive()) {
        gotoxy(90, 12);
        SetConsoleTextAttribute(hConsole, 13); // Magenta for power-up
        int remainingSeconds = pacman.getPowerUpTimer() / 10; // Convert back to seconds
        cout << "Power-Up: " << remainingSeconds << " sec   ";
    }

}

bool Game::checkCollision() {
    if (pacman.getX() == ghost1.getX() && pacman.getY() == ghost1.getY()) {
        if (pacman.isPowerUpActive()) {
            // Eat ghost
            ghost1.resetToInitialPosition(maze);
            pacman.incrementScore(); // Bonus points for eating ghost
        }
        else {
            pacman.decrementLives();
            pacman.resetPosition();
        }
    }

    // Similar checks for other ghosts
    if (pacman.getX() == ghost2.getX() && pacman.getY() == ghost2.getY()) {
        if (pacman.isPowerUpActive()) {
            ghost2.resetToInitialPosition(maze);
            pacman.incrementScore();
        }
        else {
            pacman.decrementLives();
            pacman.resetPosition();
        }
    }

    // Check ghost-ghost collision during power mode
    if (pacman.isPowerUpActive()) {
        if (checkGhostCollisionInPowerMode(&ghost1, &ghost2)) {
            resolveGhostCollision(maze, &ghost1, &ghost2);
        }
        // Add similar checks for other ghost pairs
        if (checkGhostCollisionInPowerMode(&ghost1, &ghost3)) {
            resolveGhostCollision(maze, &ghost1, &ghost3);
        }
        if (checkGhostCollisionInPowerMode(&ghost1, &ghost4)) {
            resolveGhostCollision(maze, &ghost1, &ghost4);
        }
        if (checkGhostCollisionInPowerMode(&ghost2, &ghost3)) {
            resolveGhostCollision(maze, &ghost2, &ghost3);
        }
        if (checkGhostCollisionInPowerMode(&ghost2, &ghost4)) {
            resolveGhostCollision(maze, &ghost2, &ghost4);
        }
        if (checkGhostCollisionInPowerMode(&ghost3, &ghost4)) {
            resolveGhostCollision(maze, &ghost3, &ghost4);
        }
    }

    if (pacman.getLives() == 0) {
        displayGameOver();
        return true;
    }

    return false;
}

void Game::welcome()
{
    cout << endl;
    int row = 12;
    int col = 24;
    for (int i = 1; i <= row; i++)
    {
        cout << "\t\t\t";
        for (int j = 1; j <= col; j++)
        {
            if (i == 1 || j == 1 || i == row || j == col)
            {
                SetConsoleTextAttribute(hConsole, 6);
                cout << "- ";
                Sleep(100);
            }
            else
                cout << "  ";

        }
        if (i == row / 2)
        {
            cout << endl;
            Sleep(200);
            SetConsoleTextAttribute(hConsole, 7);
            cout << "\t\t\t|- - - - - - - - - - - - - - - - - - - - - - -|" << endl << endl;;
            Sleep(100);
            SetConsoleTextAttribute(hConsole, 11);
            cout << "\t\t\t|          PACMAN GAME USING OOP              | " << endl << endl;
            Sleep(300);
            SetConsoleTextAttribute(hConsole, 7);
            cout << "\t\t\t| - - - - - - - - - - - - - - - -  - - - - - -|" << endl;
            Sleep(100);

        }

        if (i == (row / 2))
        {
            cout << endl;
            Sleep(200);
            cout << endl;
            SetConsoleTextAttribute(hConsole, 7);
            cout << "\t\t\t|- - - - - - - - - - - - - - - - - - - - - - -|" << endl << endl;
            Sleep(100);
            SetConsoleTextAttribute(hConsole, 11);
            cout << "\t\t\t|              Designed by:  Asadullah        | " << endl;
            cout << "\t\t\t|                             Ali Hasan       |           " << endl << endl;
            Sleep(200);
            SetConsoleTextAttribute(hConsole, 11);
            cout << "\t\t\t|              Roll no   : 23F-0693           | " << endl;
            cout << "\t\t\t|                           23F-0610          | " << endl;
            Sleep(300);
            SetConsoleTextAttribute(hConsole, 7);
            cout << "\t\t\t| - - - - - - - - - - - - - - - -  - - - - - -|" << endl;
            Sleep(100);

        }
        cout << endl;

    }
}

void Game::displayMenu() {
    cout << endl << endl;
    SetConsoleBackground(BACKGROUND_GREEN | BACKGROUND_RED);
    SetConsoleTextAttribute(hConsole, 12);
    cout << "\t\t\t\t\t\t*********************" << endl;
    cout << "\t\t\t\t\t\t*      Pac-Man      *" << endl;
    cout << "\t\t\t\t\t\t*********************" << endl;
    cout << endl << endl;

    SetConsoleTextAttribute(hConsole, 3);
    cout << " -------------------------------" << endl;
    cout << " | Select one option from menu |" << endl;
    cout << " -------------------------------" << endl;
    cout << endl;

    SetConsoleTextAttribute(hConsole, 11);
    cout << "\t1. Play" << endl;
    cout << "\t2. Game Description" << endl;
    cout << "\t3. Exit" << endl;
    cout << endl;

    SetConsoleTextAttribute(hConsole, 14);
    cout << " Your option_";
    int option;
    cin >> option;

    while (option < 3) {
        if (option == 1) {
            SetConsoleTextAttribute(hConsole, 14);
            playingGame = true;
            play();
        }
        else if (option == 2) {
            SetConsoleBackground(BACKGROUND_GREEN | BACKGROUND_RED);
            displayGameDescription();
        }
        else if (option == 3) {
            exit(0);
        }
        playingGame = true;
        cout << endl;
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\tPress any key to continue_";
        _getch();
        system("cls");
        displayMenu();
        SetConsoleTextAttribute(hConsole, 15);

    }
}


void Game::displayGameDescription() {
    system("cls");
    SetConsoleBackground(BACKGROUND_GREEN | BACKGROUND_RED);
    cout << endl << endl;
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\t\t\t\t\t       ----------------------------" << endl;
    cout << "\t\t\t\t\t       |     Game Description     |" << endl;
    cout << "\t\t\t\t\t       ----------------------------" << endl;
    cout << endl << endl;

    SetConsoleTextAttribute(hConsole, 12);
    cout << "\t\t How to Play :  ";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "Pac Man is a human-computer game, so only 1 player can play at a time" << endl;
    cout << "\t\t\t        The player controls Pac-Man, who must eat all the dots inside an" << endl;
    cout << "\t\t\t        enclosed maze while avoiding four colored ghosts. Eating dots called" << endl;
    cout << "\t\t\t        (Food Pellets) causes Pac-Man to earn bonus points. Pac-Man can only" << endl;
    cout << "\t\t\t        move inside the maze, cannot go outside the walls of maze. Pac-Man has" << endl;
    cout << "\t\t\t        to avoid ghosts. Colliding with the ghosts result in -1 from lives." << endl;
    cout << "\t\t\t        There are total three lives of Pac-Man. If Pac-Man collides with the" << endl;
    cout << "\t\t\t        ghosts more than three times, then the game will be over. If score" << endl;
    cout << "\t\t\t        becomes above 100, game stops and player wins" << endl;
    cout << endl;

    SetConsoleTextAttribute(hConsole, 12);
    cout << "\t\tPlayers Required : ";
    SetConsoleTextAttribute(hConsole, 11);
    cout << " 1 " << endl << endl;

    SetConsoleTextAttribute(hConsole, 12);
    cout << "\t\tMade By :";
    SetConsoleTextAttribute(hConsole, 11);
    cout << "  Asadullah Afzal 23F-0693  " << endl;
    cout << " \t\t\t\t Ali Hasan 23F-0610  " << endl;
    cout << " \t\t\t\t Enjoy the Game :)  " << endl;
}


void Game::displayGameOver() {

        system("cls");
        SetConsoleBackground(BACKGROUND_RED | BACKGROUND_GREEN);
        cout << endl << endl;
        SetConsoleTextAttribute(hConsole, 12);
        cout << "\t\t\t\t\t\t*********************" << endl;
        cout << "\t\t\t\t\t\t*      Pac-Man      *" << endl;
        cout << "\t\t\t\t\t\t*********************" << endl;
        cout << endl << endl << "\n\n\n";

        SetConsoleTextAttribute(hConsole, 13);
        cout << "\t\t\t\t\t-----------------------------------" << endl;
        cout << "\t\t\t\t\t-----------------------------------" << endl;
        cout << "\t\t\t\t\t|            Game Over            |" << endl;
        cout << "\t\t\t\t\t-----------------------------------" << endl;
        cout << "\t\t\t\t\t-----------------------------------" << endl;

        SetConsoleTextAttribute(hConsole, 15);
        cout << "\n\n Enter any key to continue_";
        int option;
        cin >> option;
        if (option == 1) {
            SetConsoleTextAttribute(hConsole, 14);
            playingGame = true;
            play();
        }
        else if (option == 2) {
            SetConsoleBackground(BACKGROUND_GREEN | BACKGROUND_RED);
            displayGameDescription();
        }
        else if (option == 3) {
            exit(0);
        }

}



void Game::displayWinner() {
    system("cls");
    SetConsoleBackground(BACKGROUND_RED | BACKGROUND_GREEN);
    cout << endl << endl;
    SetConsoleTextAttribute(hConsole, 12);
    cout << "\t\t\t\t\t\t*********************" << endl;
    cout << "\t\t\t\t\t\t*      Pac-Man      *" << endl;
    cout << "\t\t\t\t\t\t*********************" << endl;
    cout << endl << endl;

    SetConsoleTextAttribute(hConsole, 13);
    cout << "\t\t\t\t\t------------------------------------" << endl;
    cout << "\t\t\t\t\t------------------------------------" << endl;
    cout << "\t\t\t\t\t|              WINNER              |" << endl;
    cout << "\t\t\t\t\t------------------------------------" << endl;
    cout << "\t\t\t\t\t------------------------------------" << endl;

    SetConsoleTextAttribute(hConsole, 15);
    cout << "\n\n Enter any key to continue_";
    int option;
    cin >> option;
    if (option == 1) {
        SetConsoleTextAttribute(hConsole, 14);
        playingGame = true;
        play();
    }
    else if (option == 2) {
        SetConsoleBackground(BACKGROUND_GREEN | BACKGROUND_RED);
        displayGameDescription();
    }
    else if (option == 3) {
        exit(0);
    }
}


void Pacman::moveRight(char maze[ROWS][COLS]) {
    if (maze[x][y + 1] == ' ' || maze[x][y + 1] == '.' || maze[x][y + 1] == '*') {
        if (maze[x][y + 1] == '.') {
            incrementScore();
        }
        if (maze[x][y + 1] == '*') {
            activatePowerUp();
        }
        maze[x][y] = ' ';
        Game::gotoxy(y, x);
        cout << ' ';
        y++;
        SetConsoleTextAttribute(hConsole, 11);
        Game::gotoxy(y, x);
        cout << '<';
        SetConsoleTextAttribute(hConsole, 15);
    }
}

void Pacman::moveUp(char maze[ROWS][COLS]) {
    if (maze[x - 1][y] == ' ' || maze[x - 1][y] == '.' || maze[x - 1][y] == '*') {
        if (maze[x - 1][y] == '.') {
            incrementScore();
        }
        if (maze[x - 1][y] == '*') {
            activatePowerUp();
        }
        maze[x][y] = ' ';
        Game::gotoxy(y, x);
        cout << ' ';
        x--;
        SetConsoleTextAttribute(hConsole, 11);
        Game::gotoxy(y, x);
        cout << 'v';
        SetConsoleTextAttribute(hConsole, 15);
    }
}

void Pacman::moveDown(char maze[ROWS][COLS]) {
    if (maze[x + 1][y] == ' ' || maze[x + 1][y] == '.' || maze[x - 1][y] == '*') {
        if (maze[x + 1][y] == '.') {
            incrementScore();
        }
        if (maze[x - 1][y] == '*') {
            activatePowerUp();
        }
        maze[x][y] = ' ';
        Game::gotoxy(y, x);
        cout << ' ';
        x++;
        SetConsoleTextAttribute(hConsole, 11);
        Game::gotoxy(y, x);
        cout << '^';
        SetConsoleTextAttribute(hConsole, 15);
    }
}

void Pinky::move(char maze[ROWS][COLS], const Pacman& pacman) {
    if (!pacman.isPowerUpActive()) {
        char pre;
        if ((maze[x - 1][y] == ' ' || maze[x - 1][y] == '.') && flag == 1) {
            pre = maze[x][y];
            maze[x][y] = pre;
            Game::gotoxy(y, x);
            cout << pre;
            x--;
            SetConsoleTextAttribute(hConsole, 13);
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
        }
        else {
            flag = 0;
        }

        if ((maze[x + 1][y] == ' ' || maze[x + 1][y] == '.') && flag == 0) {
            pre = maze[x][y];
            maze[x][y] = pre;
            Game::gotoxy(y, x);
            cout << pre;
            x++;
            SetConsoleTextAttribute(hConsole, 13);
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
        }
        else {
            flag = 1;
        }
    }
    else {
        runAway(maze, pacman);
    }
}

void Pinky::runAway(char maze[ROWS][COLS], const Pacman& pacman) {
    isRunningAway = true;
    char pre;

    // Determine direction to run away from Pacman
    int dx = (pacman.getX() > x) ? -1 : 1;
    int dy = (pacman.getY() > y) ? -1 : 1;

    // Try to move away vertically first
    if (maze[x + dx][y] == ' ' || maze[x + dx][y] == '.') {
        pre = maze[x][y];
        maze[x][y] = pre;
        Game::gotoxy(y, x);
        cout << pre;
        x += dx;
        SetConsoleTextAttribute(hConsole, 12); // Bright red when running
        Game::gotoxy(y, x);
        cout << '0';
        SetConsoleTextAttribute(hConsole, 15);
    }
    // If vertical movement fails, try horizontal
    else if (maze[x][y + dy] == ' ' || maze[x][y + dy] == '.') {
        pre = maze[x][y];
        maze[x][y] = pre;
        Game::gotoxy(y, x);
        cout << pre;
        y += dy;
        SetConsoleTextAttribute(hConsole, 12); // Bright red when running
        Game::gotoxy(y, x);
        cout << '0';
        SetConsoleTextAttribute(hConsole, 15);
    }
}

bool Game::checkGhostCollisionInPowerMode(Ghost* ghost1, Ghost* ghost2) {
    // Check if ghosts are in the same position during power mode
    return (ghost1->getX() == ghost2->getX() && ghost1->getY() == ghost2->getY());
}

void Game::resolveGhostCollision(char maze[ROWS][COLS], Ghost* ghost1, Ghost* ghost2) {
    // Try to move ghosts in different directions
    int directions[4][2] = { {0,1}, {0,-1}, {1,0}, {-1,0} };

    for (int i = 0; i < 4; i++) {
        int newX1 = ghost1->getX() + directions[i][0];
        int newY1 = ghost1->getY() + directions[i][1];

        for (int j = 0; j < 4; j++) {
            int newX2 = ghost2->getX() + directions[j][0];
            int newY2 = ghost2->getY() + directions[j][1];

            // Check if both new positions are valid and different
            if ((maze[newX1][newY1] == ' ' || maze[newX1][newY1] == '.') &&
                (maze[newX2][newY2] == ' ' || maze[newX2][newY2] == '.') &&
                (newX1 != newX2 || newY1 != newY2)) {

                // Move ghosts
                ghost1->setPosition(newX1, newY1);
                ghost2->setPosition(newX2, newY2);

                // Redraw ghosts
                gotoxy(ghost1->getY(), ghost1->getX());
                SetConsoleTextAttribute(hConsole, 12);
                cout << '0';

                gotoxy(ghost2->getY(), ghost2->getX());
                SetConsoleTextAttribute(hConsole, 12);
                cout << '0';

                SetConsoleTextAttribute(hConsole, 15);
                return;
            }
        }
    }
}


int Clyde::getRandomDirection() {
    srand(time(0));
    return 1 + (rand() % 4);
}

void Clyde::move(char maze[ROWS][COLS], const Pacman& pacman) {
    if (!pacman.isPowerUpActive()) {
        int direction = getRandomDirection();
        char pre;

        switch (direction) {
        case 1: // Right
            if (maze[x][y + 1] == ' ' || maze[x][y + 1] == '.') {
                pre = maze[x][y];
                Game::gotoxy(y, x);
                cout << pre;
                y++;
                SetConsoleTextAttribute(hConsole, 06);
                Game::gotoxy(y, x);
                cout << '0';
                SetConsoleTextAttribute(hConsole, 15);
            }
            break;
        case 2: // Left
            if (maze[x][y - 1] == ' ' || maze[x][y - 1] == '.') {
                pre = maze[x][y];
                Game::gotoxy(y, x);
                cout << pre;
                y--;
                SetConsoleTextAttribute(hConsole, 06);
                Game::gotoxy(y, x);
                cout << '0';
                SetConsoleTextAttribute(hConsole, 15);
            }
            break;
        case 3: // Up
            if (maze[x - 1][y] == ' ' || maze[x - 1][y] == '.') {
                pre = maze[x][y];
                Game::gotoxy(y, x);
                cout << pre;
                x--;
                SetConsoleTextAttribute(hConsole, 06);
                Game::gotoxy(y, x);
                cout << '0';
                SetConsoleTextAttribute(hConsole, 15);
            }
            break;
        case 4: // Down
            if (maze[x + 1][y] == ' ' || maze[x + 1][y] == '.') {
                pre = maze[x][y];
                Game::gotoxy(y, x);
                cout << pre;
                x++;
                SetConsoleTextAttribute(hConsole, 06);
                Game::gotoxy(y, x);
                cout << '0';
                SetConsoleTextAttribute(hConsole, 15);
            }
            break;
        }
    }
    else {
        runAway(maze, pacman);
    }
}

void Clyde::runAway(char maze[ROWS][COLS], const Pacman& pacman) {
    isRunningAway = true;
    char pre;

    // Determine direction to run away from Pacman
    int dx = (pacman.getX() > x) ? -1 : 1;
    int dy = (pacman.getY() > y) ? -1 : 1;

    // Try multiple directions to run away
    int directions[4][2] = { {dx, 0}, {0, dy}, {-dx, 0}, {0, -dy} };

    for (int i = 0; i < 4; i++) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        if (maze[newX][newY] == ' ' || maze[newX][newY] == '.') {
            pre = maze[x][y];
            maze[x][y] = pre;
            Game::gotoxy(y, x);
            cout << pre;
            x = newX;
            y = newY;
            SetConsoleTextAttribute(hConsole, 12); // Bright red when running
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
            break;
        }
    }
}

void Inky::move(char maze[ROWS][COLS], const Pacman& pacman) {
    if (!pacman.isPowerUpActive()) {
        // Original smart movement towards Pacman
        char pre;
        Sleep(50);

        // Move towards Pacman
        if (pacman.getX() > x && (maze[x + 1][y] == ' ' || maze[x + 1][y] == '.')) {
            pre = maze[x][y];
            Game::gotoxy(y, x);
            cout << pre;
            x++;
            SetConsoleTextAttribute(hConsole, 03);
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
        }
        else if (pacman.getX() < x && (maze[x - 1][y] == ' ' || maze[x - 1][y] == '.')) {
            pre = maze[x][y];
            Game::gotoxy(y, x);
            cout << pre;
            x--;
            SetConsoleTextAttribute(hConsole, 03);
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
        }
        else if (pacman.getY() > y && (maze[x][y + 1] == ' ' || maze[x][y + 1] == '.')) {
            pre = maze[x][y];
            Game::gotoxy(y, x);
            cout << pre;
            y++;
            SetConsoleTextAttribute(hConsole, 03);
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
        }
        else if (pacman.getY() < y && (maze[x][y - 1] == ' ' || maze[x][y - 1] == '.')) {
            pre = maze[x][y];
            Game::gotoxy(y, x);
            cout << pre;
            y--;
            SetConsoleTextAttribute(hConsole, 03);
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
        }
    }
    else {
        runAway(maze, pacman);
    }
}

void Inky::runAway(char maze[ROWS][COLS], const Pacman& pacman) {
    isRunningAway = true;
    char pre;

    // Determine direction to run away from Pacman
    int dx = (pacman.getX() > x) ? -1 : 1;
    int dy = (pacman.getY() > y) ? -1 : 1;

    // Try multiple directions to run away, prioritizing opposite of Pacman's direction
    int directions[4][2] = { {dx, 0}, {0, dy}, {-dx, 0}, {0, -dy} };

    for (int i = 0; i < 4; i++) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        if (maze[newX][newY] == ' ' || maze[newX][newY] == '.') {
            pre = maze[x][y];
            maze[x][y] = pre;
            Game::gotoxy(y, x);
            cout << pre;
            x = newX;
            y = newY;
            SetConsoleTextAttribute(hConsole, 12); // Bright red when running
            Game::gotoxy(y, x);
            cout << '0';
            SetConsoleTextAttribute(hConsole, 15);
            break;
        }
    }
}

void Game::spawnPowerUpCoin() {
    // Randomly spawn power-up coin in empty space
    do {
        powerUpCoinX = 1 + rand() % (ROWS - 2);
        powerUpCoinY = 1 + rand() % (COLS - 2);
    } while (maze[powerUpCoinX][powerUpCoinY] != ' ');

    powerUpCoinActive = true;
    powerUpCoinTimer = 300; // Active for 300 game cycles


}

void Game::removePowerUpCoin() {
    if (powerUpCoinActive) {
        maze[powerUpCoinX][powerUpCoinY] = ' ';
        gotoxy(powerUpCoinY, powerUpCoinX);
        cout << ' ';
        powerUpCoinActive = false;
    }
}

bool Game::checkPowerUpCoinCollision() {
    if (powerUpCoinActive &&
        pacman.getX() == powerUpCoinX &&
        pacman.getY() == powerUpCoinY) {
        pacman.activatePowerUp();
        removePowerUpCoin();
        return true;
    }
    return false;
}


