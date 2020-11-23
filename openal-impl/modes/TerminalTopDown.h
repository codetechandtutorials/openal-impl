#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "../SoundDevice.h"
#include "../MusicBuffer.h"
#include "../SoundEffectsLibrary.h"
#include "../SoundEffectsPlayer.h"
#include <fstream>
#include <sstream>

namespace TTD {
SoundDevice* sd = LISTENER->Get();
int walksound = SE_LOAD("../res/walksounds/stomp.ogg");
int aiwalksound = SE_LOAD("../res/walksounds/Fantozzi-SandL1.ogg");
SoundEffectsPlayer walkplayer;
SoundEffectsPlayer aiwalkplayer;
const constexpr int ROWS = 20, COLUMNS = 20;
const constexpr int SCALESIZE = 30;
enum class GraphicKey { UNSEARCHED, SEARCHED, PLAYER1, PLAYER2 };
GraphicKey map[ROWS][COLUMNS];
bool mapHasChanged = true;  //process a map draw update
GLuint programID;
float aColor[3] = { 0,1,0 };

struct Actor {
	Actor(glm::vec3 pos) : PP(pos) {};
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
		//actor.hasUnprocessedMoved = true;
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
		//actor.hasUnprocessedMoved = true;
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
void loadSquare() {

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
GLuint loadShader(const char* vertexshader, const char* fragmentshader) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//// Read the Vertex Shader code from the file
	//std::string VertexShaderCode;
	//std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	//if(VertexShaderStream.is_open()){
	//	std::stringstream sstr;
	//	sstr << VertexShaderStream.rdbuf();
	//	VertexShaderCode = sstr.str();
	//	VertexShaderStream.close();
	//}else{
	//	printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
	//	getchar();
	//	return 0;
	//}

	//// Read the Fragment Shader code from the file
	//std::string FragmentShaderCode;
	//std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	//if(FragmentShaderStream.is_open()){
	//	std::stringstream sstr;
	//	sstr << FragmentShaderStream.rdbuf();
	//	FragmentShaderCode = sstr.str();
	//	FragmentShaderStream.close();
	//}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	//printf("Compiling shader : %s\n", vertex_file_path);
	//char const * VertexSourcePointer = VertexShaderCode.c_str();
	//glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glShaderSource(VertexShaderID, 1, &vertexshader, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	//printf("Compiling shader : %s\n", fragment_file_path);
	//char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	//glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glShaderSource(FragmentShaderID, 1, &fragmentshader, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		PLAYER1.direction = Actor::PlayerDirection::UP;
		PLAYER1.hasUnprocessedMoved = true;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		PLAYER1.direction = Actor::PlayerDirection::DOWN;
		PLAYER1.hasUnprocessedMoved = true;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		PLAYER1.direction = Actor::PlayerDirection::LEFT;
		PLAYER1.hasUnprocessedMoved = true;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		PLAYER1.direction = Actor::PlayerDirection::RIGHT;
		PLAYER1.hasUnprocessedMoved = true;
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
	window = glfwCreateWindow(COLUMNS * SCALESIZE, ROWS * SCALESIZE, "terminal top down", nullptr/*mainmonitor*/, nullptr/*share*/);
	if (!window) {
		throw("error with windo init");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw("error init glad");
	glViewport(0, 0, COLUMNS * SCALESIZE, ROWS * SCALESIZE);
	glfwSetKeyCallback(window, key_callback);
	loadSquare();
	const char* vertshader =
		"#version 330 core                                                  \n"
		"layout(location = 0) in vec3 vertexPosition_modelspace;            \n"
		"uniform mat4 modelmatrix;                                          \n"
		"void main(){                                                       \n"
		"  gl_Position = modelmatrix * vec4(vertexPosition_modelspace, 1.0);\n"
		"}";
	const char* fragshader =
		"#version 330 core   \n"
		"out vec3 color;     \n"
		"uniform vec3 ucolor;\n"
		"void main() {       \n"
		"  color = ucolor;   \n"
		"}";
	programID = loadShader(vertshader, fragshader);
	glUniform1iv(glGetUniformLocation(programID, "scalesize"), 1, &SCALESIZE);

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
		PLAYER1.hasUnprocessedMoved = false;
		mapHasChanged = true;
		switch (PLAYER1.direction)
		{
		case Actor::PlayerDirection::UP:
			//moveUp(PLAYER1, dt);
			aColor[0] = .4f;
			aColor[1] = .2f;
			aColor[2] = .6f;
			break;
		case Actor::PlayerDirection::DOWN:
			//moveDown(PLAYER1, dt);
			aColor[0] = .8f;
			aColor[1] = .1f;
			aColor[2] = .2f;
			break;
		case Actor::PlayerDirection::LEFT:
			//moveLeft(PLAYER1, dt);
			aColor[0] = .2f;
			aColor[1] = .3f;
			aColor[2] = .9f;
			break;
		case Actor::PlayerDirection::RIGHT:
			//moveRight(PLAYER1, dt);
			aColor[0] = .1f;
			aColor[1] = .6f;
			aColor[2] = .1f;
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
}
void clearScreen()
{
	//system("cls");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void renderScene()
{
	if (mapHasChanged)
	{
		mapHasChanged = false;
		clearScreen();

		glUseProgram(programID);

		//glUniform1iv(glGetUniformLocation(programID, "scalesize"), 1, &SCALESIZE);

		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLUMNS; c++)
			{
				glm::mat4 ModelMatrix(1);

				glm::vec3 Translate(c*(SCALESIZE*.00173)-.491f, r*(SCALESIZE*.00173)-.491f, 0);
				ModelMatrix = glm::translate(ModelMatrix, Translate);
				glm::vec3 Scale(1);
				ModelMatrix = glm::scale(ModelMatrix, Scale);
				static const glm::vec3 rot_ax_x(1, 0, 0);
				static const glm::vec3 rot_ax_y(0, 1, 0);
				static const glm::vec3 rot_ax_z(0, 0, 1);
				glm::vec3 Rotation(0);
				ModelMatrix = glm::rotate(ModelMatrix, Rotation.x, rot_ax_x);
				ModelMatrix = glm::rotate(ModelMatrix, Rotation.y, rot_ax_y);
				ModelMatrix = glm::rotate(ModelMatrix, Rotation.z, rot_ax_z);

				glUniformMatrix4fv(glGetUniformLocation(programID, "modelmatrix"), 1, false, &ModelMatrix[0][0]);

				aColor[0] = r * .03f;
				aColor[1] = c * .03f;
				aColor[2] = r * .03f;

				glUniform3fv(glGetUniformLocation(programID, "ucolor"), 1, &aColor[0]);

				glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
			}
		}

		glfwSwapBuffers(window);
	}
	glfwPollEvents();
}
}  // end namespace TTD