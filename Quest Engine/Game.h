#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>

#include "util.h"

#include "Level.h"
#include "Player.h"
#include "Overlay.h"

using namespace std;

/**
 * Game is a static/singleton class because the glut*Func
 * functions require a static function reference.
 * The only drawback to this approach is that multiple threads
 * of an application cannot run different Game objects.
 */
class Game {
public:
	static void run(string name)
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

	static void loadLevel(Level* level)
	{
		_level = level;
	}

	static void setPlayerCharacter(Player* player)
	{
		_player = player;
	}

	static void displayOverlay(Overlay* overlay)
	{
		_overlay = overlay;
	}



private:
	Game();

	static void onDisplay(void)
	{
		if (_overlay)
		{
			_overlay->render();
		}
		else
		{
			if (_level)
			{
				_level->render();
			}

			if (_player)
			{
				_player->render();
			}
		}
	}

	static void onIdle(void)
	{
		// Background tasks
	}

	static void onKeyboard(unsigned char key, int x, int y)
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

	static void onSpecialInput(int key, int x, int y)
	{
		if (_overlay) {
			// An overlay is shown, so send this special input event to it
			_overlay->onSpecialInput(key, x, y);
		}
		else {
			// The player will handle this special input event
			_player->onSpecialInput(key, x, y);
		}
	}

	static void onReshape(int w, int h)
	{
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
		checkError();
	}

	static Level* _level;
	static Player* _player;
	static Overlay* _overlay;
};
