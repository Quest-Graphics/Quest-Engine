#include "Game.h"

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	// Create main player
	Player player;

	// Create demo level
	Level demo("Demo Level");

	// Run game
	Game::setPlayerCharacter(&player);
	Game::loadLevel(&demo);
	Game::run("Dungeon Quest");
}
