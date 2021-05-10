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
	std::vector<Coin*> _coins;

public:
	float MOVE_DIST;
	Player(Shader* shader, Camera* camera, Level* level, std::vector<Coin*> coins) : Entity(shader) {
		MOVE_DIST = 1.0f;
		_camera = camera;
		_level = level;
		position.y = 0.0f;
		_coins = coins;
	}

	void onKeyboard(unsigned char key, int x, int y)
	{
		/*
		for (Coin* coin : _coins) {
			if (intersects(coin)) {
				coin->hit();
			}
		}
		*/
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
