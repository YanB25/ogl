#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <cmath>
GLFWwindow *window;

#include <glm/glm.hpp>
#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "include/config.hpp"
#include "include/controllPoint.h"
#include "include/BesselCurve.h"
#include <fstream>
using namespace glm;
using std::cout;
using std::endl;
using std::vector;
double cursor_x = 0;
double cursor_y = 0;
static void glfw_error_callback(int error, const char* description);
static void key_call_back(GLFWwindow* windowk, int key, int scanCode, int action, int mod);
static void cursor_position_callback(GLFWwindow* windowk, double x, double y);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

const char* filename = "data/cow.obj";

class Drawer {
public:
	Drawer(int programID) : programID(programID) {
		readfile();
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &vertexbufferID);
		// glGenBuffers(1, &colorbufferID);
		glGenBuffers(1, &indexbufferID);
		printf("ok here\n");
		fflush(stdout);
	}
	void update() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GL_FLOAT) * vertexes.size(),
			vertexes.data(),
			GL_STATIC_DRAW
		);
		printf("move size is %lu\n", sizeof(GL_FLOAT) * vertexes.size());
		// glBindBuffer(GL_ARRAY_BUFFER, colorbufferID);
		// glBufferData(
		// 	GL_ARRAY_BUFFER,
		// 	sizeof(color_data),
		// 	color_data,
		// 	GL_STATIC_DRAW
		// );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbufferID);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			sizeof(GL_UNSIGNED_INT) * indexes.size(),
			indexes.data(),
			GL_STATIC_DRAW
		);
	}
	void initAttribute() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GL_FLOAT) * 3,
			(void*) 0
		);
		printf("size is %lu\n", sizeof(GL_FLOAT)*3);
		glEnableVertexAttribArray(0);
		// glBindBuffer(GL_ARRAY_BUFFER, colorbufferID);
		// glVertexAttribPointer(
		// 	1,
		// 	3,
		// 	GL_FLOAT,
		// 	GL_FALSE,
		// 	0, // use first item forever
		// 	(void*) 0
		// );
		// glEnableVertexAttribArray(1);
	}
	void draw() {
		fflush(stdout);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
		GLint colorID = glGetAttribLocation(programID, "oc");
		glUniform4f(colorID, 1.0f, 1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, 0);
	}
	void show() {
		for (int i = 0; i < 1000; ++i) {
			printf("%lf\n", vertexes.data()[i]);
		}
	}
private:
	void readfile() {
		std::ifstream fin(filename);
		std::string line;
		while (std::getline(fin, line)) {
			if (line[0] == 'v') {
				float x, y, z;
				sscanf(line.c_str(), "%*c %f %f %f", &x, &y, &z);
				vertexes.push_back(x);
				vertexes.push_back(y);
				vertexes.push_back(z);
			} else {
				int one, two, three;
				sscanf(line.c_str(), "%*c %d %d %d", &one, &two, &three);
				indexes.push_back(one);
				indexes.push_back(two);
				indexes.push_back(three);
			}
		}
	}
	int programID;
	vector<GLfloat> vertexes;
	vector<GLuint> indexes;
	GLuint VAO;
	GLuint vertexbufferID;
	GLuint colorbufferID;
	GLuint indexbufferID;
	GLfloat color_data[3] = {
		1.0f, 1.0f, 1.0f
	};
};

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{	glBindVertexArray(0);
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	// set error callback function, that only print error msg.
	glfwSetErrorCallback(glfw_error_callback);
	

	// init stuffs
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// required GLFW version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bessel", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	// set key callback that do nothing
	glfwSetKeyCallback(window, key_call_back);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; //NOTICE: no this, segment fault
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader", "GeometryShader.glsl");
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");


	Drawer drawer(programID);
	drawer.update();
	drawer.initAttribute();
	//drawer.show();

	do
	{

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programID);


		// TODO:continue here
		drawer.update();
		drawer.draw();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS && 
			glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glDeleteProgram(programID);
	glfwTerminate();

	return 0;
}

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Error in glfw, msg is: %s\n", description);
	fprintf(stderr, "Error in glfw, code is: %d\n", error);
}

static void key_call_back(GLFWwindow* windowk, int key, int scanCode, int action, int mod) {
	printf("key callback, key %d, scancode %d, action %d, mod %d \n", key, scanCode, action, mod);
	//printf("key pressed\n");
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	cursor_x = xpos / (WINDOW_WIDTH/2) - 1;
	cursor_y = -(ypos / (WINDOW_HEIGHT/2) - 1);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	if (action == 1) {
		printf("mouse button change button %d, action %d, mods %d\n", button ,action, mods);
		printf("position is (%lf, %lf)\n", cursor_x, cursor_y);
	}
}