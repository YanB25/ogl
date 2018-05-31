#include <stdio.h>
#include <stdlib.h>

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
using namespace glm;
double cursor_x = 0;
double cursor_y = 0;
int nth_point = 0;
static void glfw_error_callback(int error, const char* description);
static void key_call_back(GLFWwindow* windowk, int key, int scanCode, int action, int mod);
static void cursor_position_callback(GLFWwindow* windowk, double x, double y);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

//static GLfloat g_vertex_buffer_data[(GRAN+1)*2]= {
	// not init here
//};

class BesselCurve;
ControllPoint* golbal_controll_pointer;
BesselCurve* global_bessel_curve;
int main(void)
{
	printf("in main\n");
	fflush(stdout); 
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


	// init VAO
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);

	// for controll point
	ControllPoint controllPoint(programID);
	controllPoint.initAttribute();
	BesselCurve besselCurve(programID);
	besselCurve.initAttribute();
	golbal_controll_pointer = &controllPoint;
	global_bessel_curve = &besselCurve;

	do
	{

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programID);

		glBindVertexArray(VertexArrayID);

		glBindVertexArray(0);
		controllPoint.update();
		controllPoint.draw();

		if (controllPoint.getNumOfPoint() == 4) {
			besselCurve.calculatePoints(controllPoint.getArray());
			besselCurve.update();
			besselCurve.draw();
		}

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
		golbal_controll_pointer->addPoint(cursor_x, cursor_y);
		//controllPoint.addPoint(cursor_x, cursor_y);
		printf("nth is %d\n", nth_point);
		nth_point = (nth_point+1) % 4;
	}
}