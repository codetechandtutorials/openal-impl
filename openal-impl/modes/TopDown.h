#pragma once
#include "../SoundDevice.h"
#include "../SoundEffectsPlayer.h"
#include "../SoundEffectsLibrary.h"
#include <glm/ext/vector_float3.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

namespace GridGame {
SoundDevice* sd = LISTENER->Get();
int SciFiSound = SE_LOAD("../res/soundeffects/sci-fidrone.ogg");
SoundEffectsPlayer sound_effects_player_forSciFiSound;
const constexpr int ROWS = 10, COLUMNS = 10;
const constexpr float CELLSIZE = 40;  // assumes ROWS == COLUMNS for simplicity
enum class GraphicKey { UNSEARCHED, SEARCHED, PLAYER1, PLAYER2 };
GraphicKey map[ROWS][COLUMNS];
bool mapHasChanged = true;  //process a map draw update
GLuint programID;
float aColor[3] = { 0,1,0 };

struct Actor {
	Actor() {};
	Actor(int x, int y) { mapPos[0] = x; mapPos[1] = y;};
	glm::vec3 Pos = { 0,0,0 };
	int mapPos[2] = { 0,0 };
	enum class Orientation { PRONE, LEFT, UP, RIGHT, DOWN } direction = Orientation::PRONE;  //character to represent facing direction
	bool hasUnprocessedMoved = false;
};
Actor PLAYER1, PLAYER2((int)(ROWS-1), (int)(COLUMNS-1));
GLFWwindow* window;
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
		PLAYER1.direction = Actor::Orientation::UP;
		PLAYER1.hasUnprocessedMoved = true;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		PLAYER1.direction = Actor::Orientation::DOWN;
		PLAYER1.hasUnprocessedMoved = true;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		PLAYER1.direction = Actor::Orientation::LEFT;
		PLAYER1.hasUnprocessedMoved = true;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		PLAYER1.direction = Actor::Orientation::RIGHT;
		PLAYER1.hasUnprocessedMoved = true;
	}
}
void setPLAYER1ListenerLoc()
{
	sd->SetLocation(
		(float)PLAYER1.mapPos[0],
		(float)PLAYER1.mapPos[1],
		0
	);
	switch (PLAYER1.direction)
	{
		case Actor::Orientation::LEFT:
		sd->SetOrientation(-1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
		break;
		case Actor::Orientation::RIGHT:
			sd->SetOrientation(1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
		break;
		case Actor::Orientation::UP:
			sd->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
		break;
		case Actor::Orientation::DOWN:
			sd->SetOrientation(0.f, -1.f, 0.f, 0.f, 0.f, 1.f);
		break;
		default:
		break;
	}
};
void init() {
	if (!glfwInit())
		throw("error in glfw init");
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
	window = glfwCreateWindow(COLUMNS * CELLSIZE, ROWS * CELLSIZE, "terminal top down", nullptr/*mainmonitor*/, nullptr/*share*/);
	if (!window) {
		throw("error with window init");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw("error with glad init");
	glViewport(0, 0, COLUMNS * CELLSIZE, ROWS * CELLSIZE);
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
	ALint attunation = AL_INVERSE_DISTANCE_CLAMPED;
	sd->SetAttunation(attunation);
	sd->SetLocation(0.f, 0.f, 0.f);
	sd->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

	sound_effects_player_forSciFiSound.SetLooping(true);
	sound_effects_player_forSciFiSound.SetPosition(ROWS-1, COLUMNS-1, 0);
	sound_effects_player_forSciFiSound.Play(SciFiSound);
}
void processMapChanges()
{
	for (int x = 0; x < ROWS; x++)  //rows
	{
		for (int y = 0; y < COLUMNS; y++)  //columns
		{
			if (x == PLAYER1.mapPos[0] && y == PLAYER1.mapPos[1])
				map[x][y] = GraphicKey::PLAYER1;
			else if (x == PLAYER2.mapPos[0] && y == PLAYER2.mapPos[1])
				map[x][y] = GraphicKey::PLAYER2;
			else
				map[x][y] = GraphicKey::UNSEARCHED;
		}
	}
}
void processPlayer(float dt)
{
	static const float MOVECD = .1667f;
	static float stopwatch = .1667f;
	stopwatch += dt;
	if (stopwatch > MOVECD)
	{
		if (PLAYER1.hasUnprocessedMoved)
		{
			stopwatch = 0.f;
			PLAYER1.hasUnprocessedMoved = false;
			mapHasChanged = true;
			switch (PLAYER1.direction)
			{
			case Actor::Orientation::UP:
				if (PLAYER1.mapPos[1] < ROWS - 1)
					PLAYER1.mapPos[1]++;
				break;
			case Actor::Orientation::DOWN:
				if (PLAYER1.mapPos[1] > 0)
					PLAYER1.mapPos[1]--;
				break;
			case Actor::Orientation::LEFT:
				if (PLAYER1.mapPos[0] > 0)
					PLAYER1.mapPos[0]--;
				break;
			case Actor::Orientation::RIGHT:
				if (PLAYER1.mapPos[0] < COLUMNS -1)
					PLAYER1.mapPos[0]++;
				break;
			case Actor::Orientation::PRONE:
				break;
			default:
				break;
			}
			setPLAYER1ListenerLoc();
		}
	}
}
void processAI(float dt) {
	if (PLAYER2.hasUnprocessedMoved)
	{
		switch (PLAYER2.direction)
		{
		case Actor::Orientation::UP:
			break;
		case Actor::Orientation::DOWN:
			break;
		case Actor::Orientation::LEFT:
			break;
		case Actor::Orientation::RIGHT:
			break;
		case Actor::Orientation::PRONE:
			break;
		default:
			break;
		}
	}
}
void setCellDrawColor(const int& x, const int& y) {
	processMapChanges();
	switch (map[x][y]) {
	case GraphicKey::UNSEARCHED:
		aColor[0] = 0.1f;
		aColor[1] = 0.4f;
		aColor[2] = 0.1f;
		break;
	case GraphicKey::SEARCHED:
		aColor[0] = 0.1f;
		aColor[1] = 0.1f;
		aColor[2] = 0.1f;
		break;
	case GraphicKey::PLAYER1:
		aColor[0] = 0.1f;
		aColor[1] = 0.1f;
		aColor[2] = 0.9f;
		break;
	case GraphicKey::PLAYER2:
		aColor[0] = 0.9f;
		aColor[1] = 0.1f;
		aColor[2] = 0.1f;
		break;
	default:
		break;
	}
}
void clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void renderScene()
{
	if (mapHasChanged)
	{
		mapHasChanged = false;
		clearScreen();
		glUseProgram(programID);
		/// <summary>
		/// Where map[0][0] is the Bottom Left, and map[ROWS-1][COLUMNS-1] is the Top Right.
		/// </summary>
		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLUMNS; c++)
			{
				setCellDrawColor(r, c);
				glUniform3fv(glGetUniformLocation(programID, "ucolor"), 1, &aColor[0]);
				glm::mat4 ModelMatrix(1);
				glm::vec3 Translate((float)((float)r / (float)COLUMNS) * 2.f - .9f, (float)((float)c / (float)ROWS) * 2.f - .9f, 0.f);

				//std::cout << "crl: " << Translate.x << ", " << Translate.y << '|';
				ModelMatrix = glm::translate(ModelMatrix, Translate);
				glm::vec3 Scale((float)ROWS / (float)CELLSIZE - .08f);
				ModelMatrix = glm::scale(ModelMatrix, Scale);
				//static const glm::vec3 rot_ax_x(1, 0, 0);
				//static const glm::vec3 rot_ax_y(0, 1, 0);
				//static const glm::vec3 rot_ax_z(0, 0, 1);
				//glm::vec3 Rotation(0);
				//ModelMatrix = glm::rotate(ModelMatrix, Rotation.x, rot_ax_x);
				//ModelMatrix = glm::rotate(ModelMatrix, Rotation.y, rot_ax_y);
				//ModelMatrix = glm::rotate(ModelMatrix, Rotation.z, rot_ax_z);
				glUniformMatrix4fv(glGetUniformLocation(programID, "modelmatrix"), 1, false, &ModelMatrix[0][0]);
				glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
			}
		}
		glfwSwapBuffers(window);
	}
	glfwPollEvents();
}
bool isRunning() { return (!glfwWindowShouldClose(window)); }
}  // end namespace TTD
