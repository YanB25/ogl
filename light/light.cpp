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
#include <common/objloader.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "include/config.hpp"
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
float delta_x = 0;
float delta_y = 0;
float delta_z = 0;

int Mod = 1;


const char* filename = "data/teapot.obj";

class Drawer {
public:
	Drawer(int programID) : programID(programID) {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &vertexbufferID);
		glGenBuffers(1, &uvsbufferID);
		glGenBuffers(1, &normalsbufferID);

		loadOBJ(filename, vertexes, uvs, normals);
		printf("ok here\n");
		fflush(stdout);
	}
	void update() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(glm::vec3) * vertexes.size(),
			vertexes.data(),
			GL_STATIC_DRAW
		);

		glBindBuffer(GL_ARRAY_BUFFER, normalsbufferID);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(glm::vec3) * normals.size(),
			normals.data(),
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
			0,
			(void*) 0
		);
		//printf("size is %lu\n", sizeof(GL_FLOAT)*3);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, normalsbufferID);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*) 0
		);
		//printf("size is %lu\n", sizeof(GL_FLOAT)*3);
		glEnableVertexAttribArray(1);
	}
	void draw() {
		// calculate mvp
		// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), float(16)/9, 0.1f, 100.0f);
		glm::mat4 View = glm::lookAt(
			glm::vec3(0 +delta_x, 6 +delta_y, 5 + delta_z),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
		);
		glm::mat4 Identity = glm::mat4(1.0f);
		glm::mat4 Model = Identity;
		mvp = Projection * View * Model;

		GLuint MVP_ID = glGetUniformLocation(programID, "MVP");
		GLuint V_ID = glGetUniformLocation(programID, "V");
		GLuint M_ID = glGetUniformLocation(programID, "M");
		GLuint LightPosition_ID = glGetUniformLocation(programID, "LightPosition_worldspace");

		glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(V_ID, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
		glUniform3f(LightPosition_ID, 3, 3, 9); //TODO: chagne me


		fflush(stdout);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);

		glDrawArrays(GL_TRIANGLES, 0, vertexes.size());
		//glDrawElements(GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, 0);

	}
	void show() {
		// for (int i = 0; i < 1000; ++i) {
		// 	printf("%lf\n", vertexes.data()[i]);
		// }
		for (int i = 0; i < normals.size(); ++i) {
			for (int j = 0; j < 3; ++j) {
				printf("%lf  ", normals[i][j]);
			}
			printf("\n");
		}
	}
private:
	int programID;
	vector<glm::vec3> vertexes;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	GLuint VAO;
	GLuint vertexbufferID;
	GLuint uvsbufferID;
	GLuint normalsbufferID;
	glm::mat4 mvp;
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
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "light", NULL, NULL);
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


	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	Drawer drawer(programID);
	drawer.update();
	drawer.initAttribute();
	// drawer.show();

	do
	{

		// glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);


		// TODO:continue here
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
	if (action == 0 || action == 2) {
		if (key == 87) {
			delta_z -=0.1;
		} else if (key == 83) {
			delta_z += 0.1;
		} else if (key == 65) {
			delta_x -= 0.1;
		} else if (key == 68) {
			delta_x += 0.1;
		} else if (key == 74) {
			delta_y -= 0.1;
		} else if (key == 75) {
			delta_y += 0.1;
		}

		if (key == 49) {
			Mod = 0;
		} else if (key == 50) {
			Mod = 1;
		} else if (key == 51) {
			Mod = 2;
		}
	}
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