#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <string>

#include "util.h"

#include "Level.h"
#include "Player.h"
#include "Overlay.h"

#include "DemoLevel.h"

Level* level = nullptr;
Player* player = nullptr;
Overlay* overlay = nullptr;

/**
 * Allocate space for our data in graphics card memory
 */
void initBuffers() {
	checkError();
}

void initTextures() {
	checkError();
}

void initShaders() {
	checkError();
}

void initRendering() {
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	checkError();
}

void onDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (overlay)
	{
		overlay->render();
	}
	else
	{
		if (level)
		{
			level->render();
		}

		if (player)
		{
			player->render();
		}
	}

	checkError();
}

void onIdle() {
	// Background tasks

	checkError();

	// Redraw
	glutPostRedisplay();
}

void onReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	checkError();
}

void onKeyboard(unsigned char key, int x, int y) {
	if (overlay) {
		// An overlay is shown, so send this keyboard event to it
		overlay->onKeyboard(key, x, y);
	}
	else if (player) {
		// The player will handle this keyboard event
		player->onKeyboard(key, x, y);
	}
	else {
		fprintf(stderr, "Unhandled keyboard event: (%c, %d, %d)\n", key, x, y);
	}

	checkError();
}

void onSpecialInput(int key, int x, int y) {
	if (overlay) {
		// An overlay is shown, so send this special input event to it
		overlay->onSpecialInput(key, x, y);
	}
	else if (player) {
		// The player will handle this special input event
		player->onSpecialInput(key, x, y);
	}
	else {
		fprintf(stderr, "Unhandled special input event: (%c, %d, %d)\n", key, x, y);
	}

	checkError();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Dungeon Quest");
	glewInit();
	initTextures();
	initBuffers();
	initShaders();
	initRendering();

	// Register event handlers
	glutDisplayFunc(onDisplay);
	glutIdleFunc(onIdle);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecialInput);
	checkError("main/event");

	// Create main player
	player = new Player();
	player->setModel("PLAYER1.obj");

	// Run game
	level = new DemoLevel();

	// Enter the main loop
	glutMainLoop();
	return 0;
}
