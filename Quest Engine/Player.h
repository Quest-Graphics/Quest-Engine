#pragma once

#include <GL/freeglut.h>

#include "Entity.h"

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
			break;
		case 'a':
		case 'A':
			break;
		case 's':
		case 'S':
			break;
		case 'd':
		case 'D':
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
