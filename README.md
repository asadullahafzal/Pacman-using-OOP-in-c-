1. Introduction
The Pacman game is a console-based implementation of the classic arcade game developed using C++ and Object-Oriented Programming (OOP) principles. The project leverages core OOP features such as inheritance, polymorphism, encapsulation, and abstraction to create a modular, scalable, and efficient game. The game demonstrates the practical application of OOP concepts in game development and includes AI for enemy behavior, dynamic updates, and interactive gameplay.

Purpose of the Project
•	To demonstrate the use of OOP concepts in creating an interactive application.
•	To simulate an engaging game environment using text-based graphics.
•	To explore AI behavior in controlling enemy movements.
________________________________________
2. Game Overview
Welcome Message
•	Upon launching the game, a welcome screen is displayed.
•	The screen introduces the user to the game with a brief, visually appealing message using console text formatting.
 
Options Menu
•	After the welcome message, the user is presented with a menu to choose from:
1.	Start Game: Begin a new game.
2.	Instructions: View detailed gameplay instructions.
3.	Exit: Exit the game.
•	The menu is navigated using numeric or keyboard input.
 
Instructions
If the user selects "Instructions," the game provides detailed guidelines on how to play:
•	Objective: Collect all coins in the maze while avoiding ghosts.
•	Controls:
o	Use the arrow keys to navigate Pacman.
o	Move strategically to avoid being caught.
•	Special Features:
o	Collecting a power-up allows Pacman to defeat ghosts temporarily.
•	Winning and Losing:
o	Win: Successfully collect all coins in the maze.
o	Lose: Lose all lives after being caught by ghosts.

Gameplay
•	After the user selects "Start Game," the actual gameplay begins.
•	Maze: The maze is displayed on the console screen, showing:
o	Walls (#) to navigate around.
o	Coins (.) to collect.
o	Power-ups (*) for special abilities.
•	Characters:
o	Pacman: Controlled by the player.
o	Ghosts: Move automatically and chase Pacman with distinct behaviors.
•	HUD (Heads-Up Display):
o	The console shows:
	Score: Tracks points collected.
	Lives: Displays remaining lives.
	Power-Up Timer: Shows remaining time for power-ups when active.
 

Gameplay
•	Objective: Guide Pacman through a maze, collect all coins, and avoid being caught by ghosts.
•	Maze: The maze consists of walls (#), coins (.), and power-ups (*).
•	Enemies: Ghosts pursue Pacman, with each having unique movement logic.
•	Lives: Pacman starts with three lives. Losing all lives ends the game.
•	Score: Points are awarded for collecting coins.
 
 
 
Key Features
•	Dynamic Maze: The maze is loaded from a file and can be customized.
•	Ghost Types:
o	Blinky: Moves horizontally.
o	Pinky: Moves vertically.
o	Clyde: Moves randomly.
o	Inky: Uses a smarter AI to track Pacman’s position.
•	Power-ups: Temporarily allows Pacman to defeat ghosts.
 
 
________________________________________
3. Implementation Details
Core Features
Object-Oriented Design
•	Character Base Class:
o	Manages common attributes such as position (x, y) and interactions.
•	Derived Classes:
o	Pacman manages player-specific attributes like score and lives.
o	Ghost is an abstract class for ghost-specific behavior, with four derived classes.

Maze Representation
•	Implemented as a 2D character array (char maze[ROWS][COLS]).
•	Symbols:
o	#: Wall.
o	.: Coin.
o	*: Power-up.

Collision Detection
•	Pacman interacts with the maze to collect coins and power-ups.
•	Ghosts detect Pacman’s proximity for chase logic.

Console Rendering
•	Used Windows API (SetConsoleCursorPosition) for efficient screen updates.
•	gotoxy() moves the cursor to update specific parts of the console.

Power-up Mechanic
•	Power-ups activate temporary immunity and enable Pacman to defeat ghosts.
•	Includes a countdown timer for deactivating power-ups.
________________________________________

Additional Features (Optional)

Ghost Behavior
•	Advanced AI (Inky) calculates its path based on Pacman’s location.
•	Randomized behaviors (Clyde) add unpredictability.
Scoreboard
•	Displays the score and remaining lives during gameplay.

Game States
•	Welcome Screen: Introduces the game and provides controls.
•	Game Over: Ends the game when all lives are lost.
•	Victory: Declares Pacman the winner upon collecting all coins.
