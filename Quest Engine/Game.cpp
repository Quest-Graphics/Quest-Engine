#include "Game.h"
#include "util.h"

Level* Game::_level;
Player* Game::_player;
Overlay* Game::_overlay;

void Game::run(std::string name)
{
	// Create glut window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(name.c_str());
	checkError("run/window");

	// Register event handlers
	glewInit();
	glutDisplayFunc(onDisplay);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecialInput);
	glutReshapeFunc(onReshape);
	checkError("run/event");

	// Enter the main loop
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}

void Game::loadLevel(Level* level)
{
	_level = level;
}

void Game::setPlayerCharacter(Player* player)
{
	_player = player;
}

void Game::displayOverlay(Overlay* overlay)
{
	_overlay = overlay;
}

void Game::onDisplay(void)
{
	// Render
}

void Game::onIdle(void)
{
	// Background tasks
}

void Game::onReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	checkError();
}

void Game::onKeyboard(unsigned char key, int x, int y)
{
	if (_overlay) {
		// An overlay is shown, so send this keyboard event to it
		_overlay->onKeyboard(key, x, y);
	}
	else {
		// The player will handle this keyboard event
		_player->onKeyboard(key, x, y);
	}
}

void Game::onSpecialInput(int key, int x, int y) {
	if (_overlay) {
		// An overlay is shown, so send this special input event to it
		_overlay->onSpecialInput(key, x, y);
	}
	else {
		// The player will handle this special input event
		_player->onSpecialInput(key, x, y);
	}
}
