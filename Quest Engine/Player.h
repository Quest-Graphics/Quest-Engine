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
		position.y = 0.0f;
		_coin = coin;
	}

	void onKeyboard(unsigned char key, int x, int y)
	{
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
