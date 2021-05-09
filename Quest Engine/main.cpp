#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "util.h"

#include "Camera.h"
#include "Level.h"
#include "Player.h"
#include "Overlay.h"

#include "Geometry.h"

#include "DemoLevel.h"
#include "Coin.h"

glm::ivec2 viewport = glm::ivec2(1024, 768);

Camera* camera = nullptr;
Level* level = nullptr;
Player* player = nullptr;
Overlay* overlay = nullptr; // For 2D UI
Coin* coin = nullptr;
Cube* enemy = nullptr;

Shader* playerShader;
Shader* planeShader;
Shader* coinShader;
Quad* plane;

static float deltaTime = 0;

unsigned int planeVAO;

float enemyPositions[] = {
	 50.0f, 2.5f,  50.0f, 
	 50.0f, 2.5f, -50.0f,
	-50.0f, 2.5f,  50.0f,
	-50.0f, 2.5f, -50.0f
};


void initRendering() {
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	checkError("Init");
}

void onDisplay() {
	//camera->lookAt(glm::vec3(0.0f));

	static float lastFrame = 0;
	float currentFrame = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera->viewMatrix();
	glm::mat4 projection = glm::perspective(camera->m_fov, (float) viewport.x / viewport.y, 1.0f, 1000.0f);
	glm::mat4 model = glm::scale(glm::vec3(100.0f, 1.0f, 100.0f)) * glm::rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec3 color;

	planeShader->use();
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	planeShader->setMat4("Model", model);
	planeShader->setMat4("View", view);
	planeShader->setMat4("Projection", projection);
	planeShader->setVec3("objColor", color);
	plane->render();

	playerShader->use();
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	playerShader->setVec3("objColor", color);
	playerShader->setMat4("View", view);
	playerShader->setMat4("Projection", projection);
	playerShader->setVec3("objColor", color);
	for (int i = 0; i < 12; i += 3)
	{ 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(enemyPositions[i], enemyPositions[i+1], enemyPositions[i+2]));
		model = glm::scale(model, glm::vec3(5.0f)); 
		
		playerShader->setMat4("Model", model);
		enemy->render();
	}
	
	color = glm::vec3(0.0f, 0.0f, 1.0f);
	playerShader->setVec3("objColor", color);

	model = glm::mat4(1.0f);
	player->render(model, view, projection);

	coinShader->use();
	color = glm::vec3(1.0f, 1.0f, 0.0f);
	coinShader->setVec3("objColor", color);
	coinShader->setMat4("View", view);
	coinShader->setMat4("Projection", projection);
	coinShader->setMat4("Model", model);
	coin->render(model, view, projection);
	

	glutSwapBuffers();
	checkError("Display");
}

void onIdle() {
	// Background tasks

	checkError("Idle");

	// Redraw
	glutPostRedisplay();
}

void onReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	viewport.x = w;
	viewport.y = h;
	checkError("Reshape");
}

void onKeyboard(unsigned char key, int x, int y) {
	float mvmtDist = deltaTime * 100.0f;
	glm::vec3 mvmtDir(0.0f);

	switch (key) {
	case 27:
		exit(0);
		break;
	case 'w':case 'W':
		mvmtDir = glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f)) * mvmtDist;
		player->position += mvmtDir;
		camera->m_position += mvmtDir;

		break;
	case 'a':case 'A':
		mvmtDir = glm::normalize(glm::vec3(-1.0f, 0.0f, 1.0f)) * mvmtDist;
		player->position += mvmtDir;
		camera->m_position += mvmtDir;

		break;
	case 's':case 'S':
		mvmtDir = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)) * mvmtDist;
		player->position += mvmtDir;
		camera->m_position += mvmtDir;

		break;
	case 'd':case 'D':
		mvmtDir = glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)) * mvmtDist;
		player->position += mvmtDir;
		camera->m_position += mvmtDir;

		break;
	case 'c':case'C':
		//std::cout << glm::to_string(camera->m_forward) << std::endl;
		break;
	}

	checkError("Key");
}

void onSpecialInput(int key, int x, int y) {
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

	checkError("Special Key");
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
	playerShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
	planeShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
	coinShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");

	// Run game
	camera = new Camera();

	level = new DemoLevel();
	coin = new Coin(coinShader, static_cast<DemoLevel*>(level));

	// Create main player
	player = new Player(playerShader, camera, level, coin);
	player->setModel("PLAYER1.obj");

	camera->m_position = glm::vec3(15.0f, 15.0f, 15.0f) + player->position;
	camera->lookAt(player->position);

	plane = new Quad();
	enemy = new Cube();

	// Enter the main loop
	glutMainLoop();
	return 0;
}

