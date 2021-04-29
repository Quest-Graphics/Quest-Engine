#pragma once

#include <GL/freeglut.h>

#include "Entity.h"

static const float MOVE_DIST = 10.0f;

class Player : public Entity
{
public:
	Player(Shader* shader) : Entity(shader) {
	}

	void onKeyboard(unsigned char key, int x, int y)
	{
		switch (key) {
		case 'w':
		case 'W':
			position.z -= MOVE_DIST;
			facing = 0;
			break;
		case 'a':
		case 'A':
			position.x -= MOVE_DIST;
			facing = 270;
			break;
		case 's':
		case 'S':
			position.z += MOVE_DIST;
			facing = 180;
			break;
		case 'd':
		case 'D':
			position.x += MOVE_DIST;
			facing = 90;
			break;
		case 'e':
		case 'E':
		case 13: // Enter
			// TODO: interact with item
			break;
		}
	}

	void onSpecialInput(int key, int x, int y)
	{
		switch (key) {
		case GLUT_KEY_UP:
			break;
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_DOWN:
			break;
		case GLUT_KEY_RIGHT:
			break;
		}
	}
};
