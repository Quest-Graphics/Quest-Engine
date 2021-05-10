#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <vector>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "util.h"
#include "Helper.h"

#include "Camera.h"
#include "Level.h"
#include "Player.h"
#include "Overlay.h"
#include "Projectile.h"

#include "Geometry.h"
#include "Light.h"

#include "DemoLevel.h"
#include "Coin.h"

glm::ivec2 viewport = glm::ivec2(1024, 768);

Camera* camera = nullptr;
Level* level = nullptr;
Player* player = nullptr;
Overlay* overlay = nullptr; // For 2D UI
Cube* cube = nullptr;

Shader* playerShader;
Shader* planeShader;
Shader* simpleShader;
Quad* plane;
std::vector<Light*> lights;
std::vector<Projectile*> projectiles;
std::vector<Coin*> coins;
std::vector<Enemy*> enemies;

glm::mat4 playerModel;
glm::mat4 projectileModel;
glm::mat4 enemyModel;
glm::mat4 planeModel;
glm::mat4 coinModel;

glm::mat4 view;
glm::mat4 projection;

static float deltaTime = 0;
static float currentFrame = 0;

unsigned int planeVAO;
float angle = 0;
int maxLights = 20;

glm::vec2 upperBound(50.0f, 50.0f);
glm::vec2 lowerBound(-50.0f, -50.0f);

float enemyPositions[] = {
	 50.0f, 1.0f,  50.0f, 
	 50.0f, 1.0f, -50.0f,
	-50.0f, 1.0f,  50.0f,
	-50.0f, 1.0f, -50.0f
};

//creates projectiles and attaches lights to them
void shootProjectiles(float currentFrame)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->fire(currentFrame))
		{
			glm::vec3 trajectory = glm::normalize(player->position - enemies[i]->position);
			trajectory.y = 0.0f;
			glm::vec3 projPos = enemies[i]->position;
			projPos.y = 1.0f;

			Projectile* projectile = new Projectile(projPos, trajectory, simpleShader);
			projectiles.push_back(projectile);

			//create new light and tie it to the projectile
			Light* newLight = light(glm::vec3(0.0f), randomColor(), glm::vec3(0.4f), glm::vec3(0.75f), glm::vec3(1.0f), 0.6f, 0.25f, 0.0035f);
	
			lights.push_back(newLight);
		}
	}
}

//Erases projectiles, updates their positions, generates new ones
void handleProjectiles()
{
	int idx = -1;
	idx = checkProjectiles(projectiles);
	if (idx != -1)
	{
		projectiles.erase(projectiles.begin() + idx);
		lights.erase(lights.begin() + idx);
	}

	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->updatePos(deltaTime);
		lights[i]->position = projectiles[i]->position;
	}

	idx = -1;
	idx = checkCollision(coins, player->position);
	if (idx != -1)
	{
		coins[idx]->hit();
		coins.erase(coins.begin() + idx);
	}

	idx = -1;
	idx = checkCollision(projectiles, player->position);
	if (idx != -1)
	{
		std::cout << "GAME OVER" << std::endl;
		exit(0);
	}

	if (lights.size() <= maxLights)
		shootProjectiles(currentFrame);
}

void initRendering() {
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	checkError("Init");
}

void onDisplay() {
	static float lastFrame = 0;
	currentFrame = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	handleProjectiles();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = camera->viewMatrix();
	projection = glm::perspective(camera->m_fov, (float) viewport.x / viewport.y, 1.0f, 1000.0f);
	glm::vec2 scaleFactor = upperBound - lowerBound;
	planeModel = glm::translate(upperBound.x + lowerBound.x, 0.0f, upperBound.y + lowerBound.y) * glm::scale(scaleFactor.x, 1.0f, scaleFactor.y) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	playerModel = glm::mat4(1.0f);

	//plane
	planeShader->use();
	planeShader->setMat4("Model", planeModel);
	planeShader->setMat4("View", view);
	planeShader->setMat4("Projection", projection);
	plane->render();
	planeShader->unuse();

	glm::vec3 color;
	
	//all the lights
	playerShader->use();
	playerShader->setMat4("View", view);
	playerShader->setMat4("Projection", projection);
	playerShader->setVec3("viewPos", camera->m_position);
	playerShader->setInt("numLights", lights.size());
	playerShader->setFloat("shininess", 50.0f);

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
	playerShader->unuse();

	//all the lights
	planeShader->use();
	planeShader->setMat4("View", view);
	planeShader->setMat4("Projection", projection);
	planeShader->setVec3("viewPos", camera->m_position);
	planeShader->setInt("numLights", lights.size());
	planeShader->setFloat("shininess", 50.0f);

	for (int i = 0; i < lights.size(); i++)
	{
		planeShader->setVec3("pointLights[" + std::to_string(i) + "].position", lights[i]->position);
		planeShader->setVec3("pointLights[" + std::to_string(i) + "].color", lights[i]->color);
		
		planeShader->setVec3("pointLights[" + std::to_string(i) + "].ambient", lights[i]->ambient);
		planeShader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", lights[i]->diffuse);
		planeShader->setVec3("pointLights[" + std::to_string(i) + "].specular", lights[i]->specular);
		
		planeShader->setFloat("pointLights[" + std::to_string(i) + "].constant", lights[i]->constant);
		planeShader->setFloat("pointLights[" + std::to_string(i) + "].linear", lights[i]->linear);
		planeShader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", lights[i]->quadratic);
	}
	planeShader->unuse();

	//projectile light
	simpleShader->use();
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectileModel = glm::mat4(1.0f);
		simpleShader->setVec3("objColor", lights[i]->color);
		projectiles[i]->render(projectileModel, view, projection);
	}
	simpleShader->unuse();


	//coins
	playerShader->use();
	color = glm::vec3(0.9f, 0.9f, 0.0f);
	playerShader->setVec3("objColor", color);
	playerShader->setFloat("shininess", 250.0f);
	coinModel = glm::mat4(1.0f);
	for (auto coin : coins) {
		coinModel = glm::translate(coin->position + glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(angle, 0.0f, 1.0f, 0.0f) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f) * glm::translate(-coin->position);
		coin->render(coinModel, view, projection);
	}
	playerShader->unuse();
	
	//player
	playerShader->use();
	color = glm::vec3(0.5f, 0.5f, 0.6f);
	playerShader->setVec3("objColor", color);
	playerModel = glm::mat4(1.0f);
	player->render(playerModel, view, projection);
	playerShader->unuse();

	
	//enemy cubes
	planeShader->use();
	color = glm::vec3(1.0f, 0.0f, 0.0f);
	planeShader->setVec3("objColor", color);
	for (int i = 0; i < enemies.size(); i++)
	{
		enemyModel = glm::mat4(1.0f);
		enemyModel = glm::translate(enemyModel, enemies[i]->position);
		enemyModel = glm::rotate(enemyModel, angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		enemyModel = glm::rotate(enemyModel, 45.0f, glm::vec3(1.0f, 0.0f, 1.0f));
		enemyModel = glm::scale(enemyModel, glm::vec3(5.0f));

		playerShader->setMat4("Model", enemyModel);
		cube->render();
	}
	planeShader->unuse();
	

	glutSwapBuffers();
	checkError("Display");

	angle += 1.5f;
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

//implements WASD movement for the character
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
		break;

	case 'a':case 'A':
		player->facing = 315.0f;
		mvmtDir = glm::normalize(glm::vec3(-1.0f, 0.0f, 1.0f)) * mvmtDist;
		break;

	case 's':case 'S':
		player->facing = 45.0f;
		mvmtDir = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)) * mvmtDist;
		break;

	case 'd':case 'D':
		player->facing = 135.0;
		mvmtDir = glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f)) * mvmtDist;
		break;
	}

	glm::vec2 afterPos(player->position.x + mvmtDir.x, player->position.z + mvmtDir.z);

	if (afterPos.x > upperBound.x || afterPos.x < lowerBound.x)
		return;
	if (afterPos.y > upperBound.y || afterPos.y < lowerBound.y)
		return;

	player->position += mvmtDir;
	//player->onKeyboard(key, x, y);
	camera->m_position += mvmtDir;

	checkError("Key");
}

//implements camera movement with the arrow keys
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
	planeShader = new Shader("Shaders/textureLightsVert.shader", "Shaders/textureLightsFrag.shader");
	simpleShader = new Shader("Shaders/solidVert.shader", "Shaders/solidFrag.shader");

	// Run game
	camera = new Camera();

	level = new DemoLevel();

	//generate coins
	for (int i = 0; i < DemoLevel::NUM_COINS; i++) {
		Coin* coin = new Coin(playerShader, static_cast<DemoLevel*>(level));
		coin->position = randomPosition(upperBound, lowerBound);
		coins.push_back(coin);
	}

	// Create main player
	player = new Player(playerShader, camera, level, coins);
	player->setModel("PLAYER1.obj");

	//setup camera
	camera->m_position = glm::vec3(25.0f, 25.0f, 25.0f) + player->position;
	camera->lookAt(player->position);

	plane = new Quad("Textures/lava.jpg");

	cube = new Cube("Textures/sauron.png");

	//generate enemies
	for (int i = 0; i < sizeof(enemyPositions)/sizeof(float); i+=3)
	{
		Enemy* enemy = new Enemy(glm::vec3(enemyPositions[i], enemyPositions[i+1], enemyPositions[i+2]), 2.0f);
		enemies.push_back(enemy);
	}

	// Enter the main loop
	glutMainLoop();
	return 0;
}