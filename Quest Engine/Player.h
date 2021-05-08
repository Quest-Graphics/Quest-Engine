#pragma once

#include <playsoundapi.h>

#include <GL/freeglut.h>

#include "Entity.h"
#include "Coin.h"

class Player : public Entity
{
private:
	Camera* _camera;
	Level* _level;
	Coin* _coin;

public:
	float MOVE_DIST;
	Player(Shader* shader, Camera* camera, Level* level, Coin* coin) : Entity(shader) {
		MOVE_DIST = 1.0f;
		_camera = camera;
		_level = level;
		position.y = 2.5f;
		_coin = coin;
	}

	void onKeyboard(unsigned char key, int x, int y)
	{
		switch (key) {
		case 'w':
		case 'W':
			facing = 0;

			if (position.z - MOVE_DIST > _level->mainStage->extent().minZ) {
				position.z -= MOVE_DIST;
				_camera->moveWithPlayer(UP, MOVE_DIST);
			}
			else {
				bump();
			}

			break;
		case 'a':
		case 'A':
			facing = 270;

			if (position.x - MOVE_DIST > _level->mainStage->extent().minX) {
				position.x -= MOVE_DIST;
				_camera->moveWithPlayer(LEFT, MOVE_DIST);
			}
			else {
				bump();
			}

			break;
		case 's':
		case 'S':
			facing = 180;

			if (position.z + MOVE_DIST + _model->size().z < _level->mainStage->extent().maxZ) {
				position.z += MOVE_DIST;
				_camera->moveWithPlayer(DOWN, MOVE_DIST);
			}
			else {
				bump();
			}

			break;
		case 'd':
		case 'D':
			facing = 90;

			if (position.x + MOVE_DIST + _model->size().x < _level->mainStage->extent().maxX) {
				position.x += MOVE_DIST;
				_camera->moveWithPlayer(RIGHT, MOVE_DIST);
			}
			else {
				bump();
			}

			break;
		case 'e':
		case 'E':
		case 13: // Enter
			// TODO: interact with item
			break;
		}

		if (intersects(_coin)) {
			_coin->hit();
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

private:
	void bump() {
		PlaySound(L"Sounds/bump.wav", NULL, SND_ASYNC | SND_FILENAME);
	}
};
