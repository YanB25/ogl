#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <cmath>
GLFWwindow *window;

#include <glm/glm.hpp>
#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "include/mymath.hpp"
#include "include/config.hpp"
using namespace glm;
static void glfw_error_callback(int error, const char* description);

static void key_call_back(GLFWwindow* windowk, int key, int scanCode, int action, int mod);

static GLfloat g_vertex_buffer_data[NUM_OF_SLICE][(CYCLE_SIDE+1) * 3]= {
	// not init here
};
const int vertex_buffer_size = NUM_OF_SLICE * (CYCLE_SIDE + 1) * 3 * sizeof(GLfloat);
// void init_points() {
// 	int lim = LINE_PER_CYCLE-1;
// 	int step = 360 / lim;
// 	for (int i = 0; i < lim; ++i) {
// 		int degree = step * i;
// 		double x = cos(degree * PI / 180);
// 		double y = sin(degree * PI / 180);
// 		g_vertex_buffer_data[2*i] = x;
// 		g_vertex_buffer_data[2*i + 1] = y;
// 	}
// 	g_vertex_buffer_data[2*lim] = cos(0);
// 	g_vertex_buffer_data[2*lim + 1] = sin(0);
// }

static const GLfloat g_color_buffer_data[] = {
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};

int main(void)
{
	for (int i = 0; i < NUM_OF_SLICE; ++i) {
		// magic number:
		// devide z into [-0.98, 0.98], width is 1.96, starting point is -0.98
		double step = 1.96 / (NUM_OF_SLICE-1);
		double z = -0.98 + step * i;
		printf("z is %lf\n", z);
		fill_cycle(z, g_vertex_buffer_data[i]);
	}
	// Initialise GLFW
	if (!glfwInit())
	{
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
	window = glfwCreateWindow(1600, 900, "star", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	// set key callback that do nothing
	glfwSetKeyCallback(window, key_call_back);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; //TODO: no this, segment fault
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

	// copy vertex, index and color into VBO 

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 
		vertex_buffer_size, //NOTICE: mannually calculate size
	 	g_vertex_buffer_data, 
	 	GL_STATIC_DRAW
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbound

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbound

	// set data format. bind VAO
	glBindVertexArray(VertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0, 
		3, 
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GL_FLOAT),
		(void*)(0)
	);
	glEnableVertexAttribArray(0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbound

	// before unbind VAO, bind element buffer.
	glBindVertexArray(0);
	
	do
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programID);

		// set time to change by time
		float timeValue = glfwGetTime();
		float color_value = (sin(timeValue) / 2.0) + 0.5;
		GLint colorLocation = glGetUniformLocation(programID, "oc");
		glUniform4f(colorLocation, color_value, color_value, color_value, 1.0f);

		// calculate mvp
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), float(16)/9, 0.1f, 100.0f);
		glm::mat4 View = glm::lookAt(
			glm::vec3(0, 0, 8),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
		);
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 mvp = Projection * View * Model;

		GLuint MVP_ID = glGetUniformLocation(programID, "MVP");
		glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &mvp[0][0]);


		glBindVertexArray(VertexArrayID);
		for (int i = 0; i < NUM_OF_SLICE; ++i) {
			glDrawArrays(GL_LINE_STRIP, i * (CYCLE_SIDE + 1), CYCLE_SIDE + 1);
		}
		//glDrawArrays(GL_LINE_STRIP, 0 , CYCLE_SIDE + 1);
		//glDrawArrays(GL_LINE_STRIP, CYCLE_SIDE + 1 , CYCLE_SIDE + 1);
		//glDrawArrays(GL_LINE_STRIP, 2*(CYCLE_SIDE + 1), CYCLE_SIDE + 1);
		glBindVertexArray(0);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS && 
			glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	//glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
	glfwTerminate();

	return 0;
}

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Error in glfw, msg is: %s\n", description);
	fprintf(stderr, "Error in glfw, code is: %d\n", error);
}

static void key_call_back(GLFWwindow* windowk, int key, int scanCode, int action, int mod) {
	// printf("key callback, key %d, scancode %d, action %d, mod %d \n", key, scanCode, action, mod);
	printf("key pressed\n");
}