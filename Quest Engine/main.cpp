#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <vector>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "util.h"

#include "Camera.h"
#include "Level.h"
#include "Player.h"
#include "Overlay.h"

#include "Geometry.h"
#include "Light.h"

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
Shader* simpleShader;
Quad* plane;
std::vector<Light*> lights;

glm::mat4 playerModel;
glm::mat4 cubeModel;
glm::mat4 planeModel;
glm::mat4 coinModel;

glm::mat4 view;
glm::mat4 projection;

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

	view = camera->viewMatrix();
	projection = glm::perspective(camera->m_fov, (float) viewport.x / viewport.y, 1.0f, 1000.0f);
	planeModel = glm::scale(glm::vec3(100.0f, 1.0f, 100.0f)) * glm::rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec3 color;

	//all the lights
	playerShader->use();
	playerShader->setMat4("View", view);
	playerShader->setMat4("Projection", projection);
	playerShader->setVec3("viewPos", camera->m_position);
	playerShader->setInt("numLights", lights.size());
	playerShader->setFloat("shininess", 20.0f);

	for (int i = 0; i < lights.size(); i++)
	{
		playerShader->setVec3("pointLights[" + std::to_string(i) + "].position", lights[i]->position);
		playerShader->setVec3("pointLights[" + std::to_string(i) + "].color", lights[i]->color);

		playerShader->setVec3("pointLights[" + std::to_string(i) + "].ambient", lights[i]->ambient);
		playerShader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", lights[i]->diffuse);
		playerShader->setVec3("pointLights[" + std::to_string(i) + "].specular", lights[i]->specular);

		playerShader->setFloat("pointLights[" + std::to_string(i) + "].constant", lights[i]->constant);
		playerShader->setFloat("pointLights[" + std::to_string(i) + "].linear", lights[i]->linear);
		playerShader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", lights[i]->quadratic);
	}


	//plane
	playerShader->use();
	color = glm::vec3(0.0f, 1.0f, 0.0f);
	playerShader->setMat4("Model", planeModel);
	playerShader->setMat4("View", view);
	playerShader->setMat4("Projection", projection);
	playerShader->setVec3("objColor", color);
	plane->render();

	//enemy cubes
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	playerShader->setVec3("objColor", color);
	playerShader->setMat4("View", view);
	playerShader->setMat4("Projection", projection);
	playerShader->setVec3("objColor", color);
	for (int i = 0; i < 12; i += 3)
	{ 
		cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, glm::vec3(enemyPositions[i], enemyPositions[i+1], enemyPositions[i+2]));
		cubeModel = glm::scale(cubeModel, glm::vec3(5.0f)); 
		
		playerShader->setMat4("Model", cubeModel);
		enemy->render();
	}


	//coin
	color = glm::vec3(1.0f, 1.0f, 0.0f);
	coinModel = glm::mat4(1.0f);
	playerShader->setVec3("objColor", color);
	playerShader->setMat4("View", view);
	playerShader->setMat4("Projection", projection);
	playerShader->setMat4("Model", coinModel);
	coin->render(coinModel, view, projection);
	
	//player
	playerShader->use();
	color = glm::vec3(0.0f, 0.0f, 1.0f);
	//playerModel = glm::rotate(player->facing, glm::vec3(0.0f, 1.0f, 0.0f));
	playerShader->setMat4("Model", playerModel);
	playerShader->setVec3("objColor", color);
	player->render(playerModel, view, projection);


	//cube light
	simpleShader->use();
	for (int i = 0; i < lights.size(); i++)
	{
		cubeModel = glm::translate(lights[i]->position);
		simpleShader->setMat4("Model", cubeModel);
		simpleShader->setMat4("View", view);
		simpleShader->setMat4("Projection", projection);
		simpleShader->setVec3("objColor", lights[0]->color);
		enemy->render();
	}

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
		player->facing = 225.0f;
		mvmtDir = glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f)) * mvmtDist;
		player->position += mvmtDir;
		camera->m_position += mvmtDir;

		break;
	case 'a':case 'A':
		player->facing = 315.0f;
		mvmtDir = glm::normalize(glm::vec3(-1.0f, 0.0f, 1.0f)) * mvmtDist;
		player->position += mvmtDir;
		camera->m_position += mvmtDir;

		break;
	case 's':case 'S':
		player->facing = 45.0f;
		mvmtDir = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)) * mvmtDist;
		player->position += mvmtDir;
		camera->m_position += mvmtDir;

		break;
	case 'd':case 'D':
		player->facing = 135.0;
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
	playerShader = new Shader("Shaders/multiLightsVert.shader", "Shaders/multiLightsFrag.shader");
	//planeShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");
	simpleShader = new Shader("Shaders/simpleModelVert.shader", "Shaders/simpleModelFrag.shader");

	// Run game
	camera = new Camera();

	level = new DemoLevel();
	coin = new Coin(playerShader, static_cast<DemoLevel*>(level));

	// Create main player
	player = new Player(playerShader, camera, level, coin);
	player->setModel("PLAYER1.obj");

	camera->m_position = glm::vec3(15.0f, 15.0f, 15.0f) + player->position;
	camera->lookAt(player->position);

	plane = new Quad();
	enemy = new Cube();

	//Here's what you should do with every light
	Light* light = new Light();
	light->position = glm::vec3(0.0f, 10.0f, 0.0f);
	light->color = glm::vec3(1.0f);

	light->ambient = glm::vec3(0.5f);
	light->diffuse = glm::vec3(0.75f);
	light->specular = glm::vec3(1.0f);

	light->constant = 0.5f;
	light->linear = 0.032f;
	light->quadratic = 0.01f;
	lights.push_back(light);

	//second light
	light = new Light();
	light->position = glm::vec3(10.0f, 10.0f, 10.0f);
	light->color = glm::vec3(1.0f);

	light->ambient = glm::vec3(0.5f);
	light->diffuse = glm::vec3(0.75f);
	light->specular = glm::vec3(1.0f);

	light->constant = 0.5f;
	light->linear = 0.032f;
	light->quadratic = 0.01f;
	lights.push_back(light);

	// Enter the main loop
	glutMainLoop();
	return 0;
}

