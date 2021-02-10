#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>

#include "Level.h"
#include "Player.h"
#include "Overlay.h"

using namespace std;

class Game {
public:
	static void run(string name);
	static void loadLevel(Level* level);
	static void setPlayerCharacter(Player* player);
	static void displayOverlay(Overlay* overlay);

private:
	Game();
	static void onDisplay(void);
	static void onIdle(void);
	static void onKeyboard(unsigned char, int, int);
	static void onSpecialInput(int, int, int);
	static void onReshape(int, int);
	static Level* _level;
	static Player* _player;
	static Overlay* _overlay;
};
