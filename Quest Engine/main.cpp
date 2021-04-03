#include "Game.h"
#include "DemoLevel.h"

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	// Create main player
	Player player;
	player.setModel("bernie.obj");

	// Run game
	Game::setPlayerCharacter(&player);
	Game::loadLevel(new DemoLevel());
	Game::run("Dungeon Quest", glm::vec3(0.1f, 0.1f, 0.1f));
}
