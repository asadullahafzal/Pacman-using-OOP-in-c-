#include "Pacman.h"
#pragma comment(lib, "Winmm.lib")

int main() {

    PlaySound(TEXT("pacman-sound-720-publer.io.wav"), NULL, SND_FILENAME | SND_ASYNC);
    Game game;
    game.welcome();
    system("cls");
    game.displayMenu();

    return 0;
}