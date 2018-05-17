#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow *window;

#include <glm/glm.hpp>
#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
static void glfw_error_callback(int error, const char* description);

static void key_call_back(GLFWwindow* windowk, int key, int scanCode, int action, int mod);

static const GLfloat g_vertex_buffer_data[] = {
	-0.5f, 0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, -0.5f,
};

static const GLuint g_index_buffer_data[] = {
	0, 1, 2,
	3, 1, 2
};

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
	window = glfwCreateWindow(700, 700, "helloworld", NULL, NULL);
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

	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);

	GLuint indexbuffer;
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(g_index_buffer_data),
		g_index_buffer_data,
		GL_STATIC_DRAW
	);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbound

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(g_vertex_buffer_data), 
	 	g_vertex_buffer_data, 
	 	GL_STATIC_DRAW
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbound

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbound

	glBindVertexArray(VertexArrayID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0, 
		2, 
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(GL_FLOAT),
		(void*)(0)
	);
	glEnableVertexAttribArray(0);

	GLint colorAttri = glGetAttribLocation(programID, "color");
	glEnableVertexAttribArray(colorAttri);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		colorAttri,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbound

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

	
		glBindVertexArray(VertexArrayID);
		//glDrawArrays(GL_TRIANGLES, 0, 6); 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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