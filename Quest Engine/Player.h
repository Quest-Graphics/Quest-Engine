#pragma once

#include <GL/freeglut.h>

#include "Entity.h"

class Player : public Entity
{
private:
	Camera* _camera;

public:
	float MOVE_DIST;
	Player(Shader* shader, Camera* camera) : Entity(shader) {
		MOVE_DIST = 2.0f;
		_camera = camera;
	}

	void onKeyboard(unsigned char key, int x, int y)
	{
		switch (key) {
		case 'w':
		case 'W':
			position.z -= MOVE_DIST;
			facing = 0;
			_camera->moveWithPlayer(UP, MOVE_DIST);
			break;
		case 'a':
		case 'A':
			position.x -= MOVE_DIST;
			facing = 270;
			_camera->moveWithPlayer(LEFT, MOVE_DIST);
			break;
		case 's':
		case 'S':
			position.z += MOVE_DIST;
			facing = 180;
			_camera->moveWithPlayer(DOWN, MOVE_DIST);
			break;
		case 'd':
		case 'D':
			position.x += MOVE_DIST;
			facing = 90;
			_camera->moveWithPlayer(RIGHT, MOVE_DIST);
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
