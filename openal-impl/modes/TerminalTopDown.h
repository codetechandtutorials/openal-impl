#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../SoundDevice.h"
#include "../MusicBuffer.h"
#include "../SoundEffectsLibrary.h"
#include "../SoundEffectsPlayer.h"

namespace TTD {
SoundDevice* sd = LISTENER->Get();
int walksound = SE_LOAD("../res/walksounds/stomp.ogg");
int aiwalksound = SE_LOAD("../res/walksounds/Fantozzi-SandL1.ogg");
SoundEffectsPlayer walkplayer;
SoundEffectsPlayer aiwalkplayer;
const constexpr int ROWS = 20, COLUMNS = 20;

enum class GraphicKey { UNSEARCHED, SEARCHED, PLAYER1, PLAYER2 };
GraphicKey map[ROWS][COLUMNS];
bool mapHasChanged = true;  //process a map draw update

struct Actor {
	Actor(glm::vec3 pos) : PP(pos){};
	glm::vec3 prevPP = { 0,0,0 };
	glm::vec3 PP = { 0,0,0 };
	enum class PlayerDirection { PRONE, LEFT, UP, RIGHT, DOWN } direction = PlayerDirection::PRONE;  //character to represent facing direction
	bool hasUnprocessedMoved = false;
};

Actor PLAYER1(glm::vec3(0, 0, 0)), PLAYER2(glm::vec3(ROWS, 0, COLUMNS));

GLFWwindow* window;
bool isRunning() { return (!glfwWindowShouldClose(window)); }
void moveUp(Actor& actor, const float& dt)
{
	if (actor.direction != Actor::PlayerDirection::UP) {
		mapHasChanged = true;
		actor.hasUnprocessedMoved = true;
		actor.direction = Actor::PlayerDirection::UP;
	}

	glm::vec3 velocity = glm::vec3(0, 0, -1) * dt;
	actor.PP += velocity;

	if (actor.PP.z < 0)
	{
		actor.PP -= velocity;
	}
	else
	{
		mapHasChanged = true;
		actor.hasUnprocessedMoved = true;
	}

}
void moveDown(Actor& actor, const float& dt)
{
	if (actor.direction != Actor::PlayerDirection::DOWN) {
		mapHasChanged = true;
		actor.hasUnprocessedMoved = true;
		actor.direction = Actor::PlayerDirection::DOWN;
	}

	glm::vec3 velocity = glm::vec3(0, 0, 1) * dt;
	actor.PP += velocity;

	if (actor.PP.z > ROWS)
	{
		actor.PP -= velocity;
	}
	else
	{
		mapHasChanged = true;
		actor.hasUnprocessedMoved = true;
	}
}
void moveLeft(Actor& actor, const float& dt)
{
	if (actor.direction != Actor::PlayerDirection::DOWN) {
		mapHasChanged = true;
		actor.hasUnprocessedMoved = true;
		actor.direction = Actor::PlayerDirection::DOWN;
	}
	glm::vec3 velocity = glm::vec3(-1, 0, 0) * dt;
	actor.PP += velocity;

	if (actor.PP.x < 0)
	{
		actor.PP -= velocity;
	}
	else
	{
		mapHasChanged = true;
		actor.hasUnprocessedMoved = true;
	}
}
void moveRight(Actor& actor, const float& dt)
{
	if (actor.direction != Actor::PlayerDirection::RIGHT) {
		mapHasChanged = true;
		actor.hasUnprocessedMoved = true;
		actor.direction = Actor::PlayerDirection::RIGHT;
	}
	glm::vec3 velocity = glm::vec3(1, 0, 0) * dt;
	actor.PP += velocity;

	if (actor.PP.x < COLUMNS)
	{
		actor.PP -= velocity;
	}
	else
	{
		mapHasChanged = true;
		actor.hasUnprocessedMoved = true;
	}
}
void loadSquare(){

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	static const GLfloat g_vertex_buffer_data[] = {
		-.5f,-.5f,-.5f, // triangle 1 : begin
		-.5f,-.5f, .5f,
		-.5f, .5f, .5f, // triangle 1 : end
		.5f, .5f,-.5f, // triangle 2 : begin
		-.5f,-.5f,-.5f,
		-.5f, .5f,-.5f, // triangle 2 : end
		.5f,-.5f, .5f,
		-.5f,-.5f,-.5f,
		.5f,-.5f,-.5f,
		.5f, .5f,-.5f,
		.5f,-.5f,-.5f,
		-.5f,-.5f,-.5f,
		-.5f,-.5f,-.5f,
		-.5f, .5f, .5f,
		-.5f, .5f,-.5f,
		.5f,-.5f, .5f,
		-.5f,-.5f, .5f,
		-.5f,-.5f,-.5f,
		-.5f, .5f, .5f,
		-.5f,-.5f, .5f,
		.5f,-.5f, .5f,
		.5f, .5f, .5f,
		.5f,-.5f,-.5f,
		.5f, .5f,-.5f,
		.5f,-.5f,-.5f,
		.5f, .5f, .5f,
		.5f,-.5f, .5f,
		.5f, .5f, .5f,
		.5f, .5f,-.5f,
		-.5f, .5f,-.5f,
		.5f, .5f, .5f,
		-.5f, .5f,-.5f,
		-.5f, .5f, .5f,
		.5f, .5f, .5f,
		-.5f, .5f, .5f,
		.5f,-.5f, .5f
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	//glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	//glDisableVertexAttribArray(0);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			PLAYER1.direction = Actor::PlayerDirection::UP;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{

			PLAYER1.direction = Actor::PlayerDirection::DOWN;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{

			PLAYER1.direction = Actor::PlayerDirection::LEFT;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{

			PLAYER1.direction = Actor::PlayerDirection::RIGHT;
		}
	}
}
void init() {
	if (!glfwInit())
		throw("error in glfw init");
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	window = glfwCreateWindow(800, 600, "terminal top down", nullptr/*mainmonitor*/, nullptr/*share*/);
	if (!window) {
		throw("error with windo init");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw("error init glad");
	glViewport(0, 0, 800, 600);
	glfwSetKeyCallback(window, key_callback);
	loadSquare();
}
void defaultMap()
{
	for (int x = 0; x < ROWS; x++)  //rows
	{
		for (int y = 0; y < COLUMNS; y++)  //columns
		{
			map[x][y] = GraphicKey::UNSEARCHED;
		}
	}
}
void processPlayer(float dt)
{
	if (PLAYER1.hasUnprocessedMoved)
	{
		switch (PLAYER1.direction)
		{
		case Actor::PlayerDirection::UP:
			moveUp(PLAYER1, dt);
			break;
		case Actor::PlayerDirection::DOWN:
			moveDown(PLAYER1, dt);
			break;
		case Actor::PlayerDirection::LEFT:
			moveLeft(PLAYER1, dt);
			break;
		case Actor::PlayerDirection::RIGHT:
			moveRight(PLAYER1, dt);
			break;
		case Actor::PlayerDirection::PRONE:
			break;
		default:
			break;
		}
	}
}
void processAI(float dt) {
	if (PLAYER2.hasUnprocessedMoved)
	{
		switch (PLAYER2.direction)
		{
		case Actor::PlayerDirection::UP:
			moveUp(PLAYER2, dt);
			break;
		case Actor::PlayerDirection::DOWN:
			moveDown(PLAYER2, dt);
			break;
		case Actor::PlayerDirection::LEFT:
			moveLeft(PLAYER2, dt);
			break;
		case Actor::PlayerDirection::RIGHT:
			moveRight(PLAYER2, dt);
			break;
		case Actor::PlayerDirection::PRONE:
			break;
		default:
			break;
		}
	}
	//static const float AISPEED = .2667f;
	//static float AITIMEOUT = 0;
	//AITIMEOUT += dt;
	//if (AITIMEOUT > AISPEED) {
	//	AITIMEOUT = 0;
	//	int cpos = 0;
	//	if (AIP < COLUMNS) // IN TOP ROW DON'T CHECK UP
	//	{
	//	}
	//	else  // NOT IN TOP ROW CHECK UP
	//	{
	//		cpos = AIP - COLUMNS;
	//		if (map[cpos / COLUMNS][cpos < COLUMNS ? cpos : cpos % COLUMNS] == '0')
	//		{
	//			AIP = cpos;
	//			mapHasChanged = true;
	//			AIHasMoved = true;
	//			return;
	//		}
	//	}

	//	if (AIP == 0 || AIP % COLUMNS == 0)  // AGAINST LEFT WALL DON'T CHECK LEFT
	//	{
	//	}
	//	else  // NOT AGAINST LEFT WALL
	//	{
	//		cpos = AIP - 1;
	//		if (map[cpos / COLUMNS][cpos < COLUMNS ? cpos : cpos % COLUMNS] == '0')
	//		{
	//			AIP = cpos;
	//			mapHasChanged = true;
	//			AIHasMoved = true;
	//			return;
	//		}
	//	}


	//	if (AIP % COLUMNS == COLUMNS - 1)  // agianst right wall dont che3ck right
	//	{
	//	}
	//	else // not against right wall
	//	{
	//		cpos = AIP + 1;
	//		if (map[cpos / COLUMNS][cpos < COLUMNS ? cpos : cpos % COLUMNS] == '0')
	//		{
	//			AIP = cpos;
	//			mapHasChanged = true;
	//			AIHasMoved = true;
	//			return;
	//		}
	//	}

	//	if (AIP / COLUMNS == ROWS)  // on last row dont check down
	//	{

	//	}
	//	else // not on last row
	//	{
	//		cpos = AIP + COLUMNS;
	//		if (map[cpos / COLUMNS][cpos < COLUMNS ? cpos : cpos % COLUMNS] == '0')
	//		{
	//			AIP = cpos;
	//			mapHasChanged = true;
	//			AIHasMoved = true;
	//			return;
	//		}
	//	}

	//	//fail case, no 0's around
	//	if (AIP < COLUMNS) // IN TOP ROW DON'T CHECK UP
	//	{
	//	}
	//	else  // NOT IN TOP ROW CHECK UP
	//	{
	//		cpos = AIP - COLUMNS;
	//		AIP = cpos;
	//		mapHasChanged = true;
	//		AIHasMoved = true;
	//		return;
	//	}

	//	if (AIP == 0 || AIP % COLUMNS == 0)  // AGAINST LEFT WALL DON'T CHECK LEFT
	//	{
	//	}
	//	else  // NOT AGAINST LEFT WALL
	//	{
	//		cpos = AIP - 1;
	//		AIP = cpos;
	//		mapHasChanged = true;
	//		AIHasMoved = true;
	//		return;
	//	}


	//	if (AIP % COLUMNS == COLUMNS - 1)  // agianst right wall dont che3ck right
	//	{
	//	}
	//	else // not against right wall
	//	{
	//		cpos = AIP + 1;
	//		AIP = cpos;
	//		mapHasChanged = true;
	//		AIHasMoved = true;
	//		return;
	//	}

	//	if (AIP / COLUMNS == ROWS)  // on last row dont check down
	//	{

	//	}
	//	else // not on last row
	//	{
	//		cpos = AIP + COLUMNS;
	//		AIP = cpos;
	//		mapHasChanged = true;
	//		AIHasMoved = true;
	//		return;
	//	}

	//}
}
void clearScreen()
{
	//system("cls");
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void renderScene()
{
	if (mapHasChanged)
	{
		mapHasChanged = false;
		clearScreen();

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares

		glfwSwapBuffers(window);
	}
	glfwPollEvents();
}
}  // end namespace TTD