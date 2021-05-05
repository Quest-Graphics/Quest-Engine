#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <string>

#include "util.h"

#include "Camera.h"
#include "Level.h"
#include "Player.h"
#include "Overlay.h"

#include "DemoLevel.h"

glm::ivec2 viewport = glm::ivec2(1024, 768);

Camera* camera = nullptr;
Level* level = nullptr;
Player* player = nullptr;
Overlay* overlay = nullptr; // For 2D UI

static float deltaTime = 0;

void initRendering() {
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	checkError();
}

void onDisplay() {
	static float lastFrame = 0;
	float currentFrame = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (overlay)
	{
		overlay->render();
	}
	else
	{
		auto view = camera->viewMatrix();
		auto projection = glm::perspective(camera->m_fov, (float) viewport.x / viewport.y, 1.0f, 100.0f);

		if (level)
		{
			level->render(view, &projection);
		}

		if (player)
		{
			player->render(view, &projection);
		}
	}

	glutSwapBuffers();
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
	viewport.x = w;
	viewport.y = h;
	checkError();
}

void onKeyboard(unsigned char key, int x, int y) {
	if (overlay) {
		// An overlay is shown, so send this keyboard event to it
		overlay->onKeyboard(key, x, y);
	}
	else {
		switch (key) {
		case 27:
			exit(0);
			break;
		case 'c':
		case 'C':
			camera->lookAt(glm::vec3(0.0f, 0.0f, -100.0f));
			break;
		}

		if (player) {
			player->onKeyboard(key, x, y);
		}
	}

	checkError();
}

void onSpecialInput(int key, int x, int y) {
	if (overlay) {
		// An overlay is shown, so send this special input event to it
		overlay->onSpecialInput(key, x, y);
	}
	else {
		switch (key)
		{
		case GLUT_KEY_UP:
			camera->moveCamera(UP, deltaTime);
			//camera->keyRotate(UP, deltaTime);
			break;
		case GLUT_KEY_DOWN:
			camera->moveCamera(DOWN, deltaTime);
			//camera->keyRotate(DOWN, deltaTime);
			break;
		case GLUT_KEY_LEFT:
			camera->moveCamera(LEFT, deltaTime);
			//camera->keyRotate(LEFT, deltaTime);
			break;
		case GLUT_KEY_RIGHT:
			camera->moveCamera(RIGHT, deltaTime);
			//camera->keyRotate(RIGHT, deltaTime);
			break;
		}

		if (player) {
			player->onSpecialInput(key, x, y);
		}
	}

	checkError();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(viewport.x, viewport.y);
	glutInitWindowPosition(100, 100);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutCreateWindow("Dungeon Quest");
	glewInit();
	initRendering();

	// Register event handlers
	glutDisplayFunc(onDisplay);
	glutIdleFunc(onIdle);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecialInput);
	checkError("main/event");

	// Create shaders
	Shader* playerShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");

	// Run game
	camera = new Camera();
	level = new DemoLevel();

	// Create main player
	player = new Player(playerShader, camera, level);
	player->setModel("PLAYER1.obj");

	// Enter the main loop
	glutMainLoop();
	return 0;
}
