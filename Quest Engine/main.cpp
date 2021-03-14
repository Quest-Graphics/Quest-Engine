#include "Game.h"
#include "DemoLevel.h"

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	// Create main player
	Player player;
	player.setModel("Models/bernie.obj");

	// Run game
	Game::setPlayerCharacter(&player);
	Game::loadLevel(new DemoLevel());
	Game::run("Dungeon Quest");
}
