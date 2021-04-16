#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>
#include <iostream>

#include "util.h"

#include "Level.h"
#include "Player.h"
#include "Overlay.h"

#include "DemoLevel.h"
#include "Shader.h"
#include "Camera.h"

Level* level = nullptr;
Player* player = nullptr;
Overlay* overlay = nullptr;


unsigned int VBO, VAO, EBO;
Shader shader;

float lastFrame = 0.0f;
float deltaTime = 0.0f;
Camera cam1;


/**
 * Allocate space for our data in graphics card memory
 */
void initBuffers() {
	float vertices[] = {
	 0.5f,  0.5f, -100.0f,  // top right
	-0.5f, -0.5f, -100.0f,  // bottom left
	 0.5f, -0.5f, -100.0f,  // bottom right
	-0.5f,  0.5f, -100.0f   // top left 
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int vertexPos = 0;
	glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkError();
}

void initTextures() {
	checkError();
}

void initShaders() {
	shader = Shader("vert.shader", "frag.shader");
	checkError();
}

void initRendering() {
	checkError();
}

void onDisplay() {

	float currentFrame = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(cam1.m_fov), float(1024) / float(768), 0.01f, 1000.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = cam1.viewMatrix();

	shader.use();
	shader.setMat4("Model", model);
	shader.setMat4("View", view);
	shader.setMat4("Projection", projection);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
	checkError("Display");
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
	switch (key)
	{
	case 27:
		exit(0);
	case 'w': case 'W':
		cam1.moveCamera(UP, deltaTime);
		break;
	case 's': case 'S':
		cam1.moveCamera(DOWN, deltaTime);
		break;
	case 'a': case 'A':
		cam1.moveCamera(LEFT, deltaTime);
		break;
	case 'd': case 'D':
		cam1.moveCamera(RIGHT, deltaTime);
		break;
	case 'c': case 'C':
		cam1.lookAt(glm::vec3(0.0f, 0.0f, -100.0f));
		break;
	}
}

void onSpecialInput(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		cam1.keyRotate(UP, deltaTime);
		break;
	case GLUT_KEY_DOWN:
		cam1.keyRotate(DOWN, deltaTime);
		break;
	case GLUT_KEY_LEFT:
		cam1.keyRotate(LEFT, deltaTime);
		break;
	case GLUT_KEY_RIGHT:
		cam1.keyRotate(RIGHT, deltaTime);
		break;
	}
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


	/*
	// Create main player
	player = new Player();
	player->setModel("PLAYER1.obj");

	// Run game
	level = new DemoLevel();
	*/

	// Enter the main loop
	glutMainLoop();
	return 0;
}
