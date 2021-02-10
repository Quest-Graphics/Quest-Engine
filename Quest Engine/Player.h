#pragma once

#include <GL/freeglut.h>

#include "Entity.h"

class Player :
	public Entity
{
public:
	void onKeyboard(unsigned char, int, int);
	void onSpecialInput(int, int, int);
};
