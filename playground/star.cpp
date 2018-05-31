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

static const GLfloat g_color_buffer_data[] = {
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};
// NOTICE: important init

void INIT_BESEL() {
	// float x = controllPoint.getArray()[0];
	// float y = controllPoint.getArray()[1];
	// glm::vec2 P0 = glm::vec2(x, y);
	// x = controllPoint.getArray()[2];
	// y = controllPoint.getArray()[3];
	// glm::vec2 P1 = glm::vec2(x, y);
	// x = controllPoint.getArray()[4];
	// y = controllPoint.getArray()[5];
	// glm::vec2 P2 = glm::vec2(x, y);
	// x = controllPoint.getArray()[6];
	// y = controllPoint.getArray()[7];
	// glm::vec2 P3 = glm::vec2(x, y);
	// float step = 1.0f / GRAN;
	// for (int i = 0; i <= GRAN; ++i) {
	// 	float t = step * i;
	// 	glm::vec2 t0 = P0 * float(pow((1-t), 3));
	// 	glm::vec2 t1 = P1 * float(pow((1-t), 2) * t);
	// 	glm::vec2 t2 = P2 * float((1-t) * pow((t), 2));
	// 	glm::vec2 t3 = P3 * float(pow(t, 3));
	// 	glm::vec2 output = t0 + t1 + t2 + t3;
	// 	g_vertex_buffer_data[	// float x = controllPoint.getArray()[0];
	// float y = controllPoint.getArray()[1];
	// glm::vec2 P0 = glm::vec2(x, y);
	// x = controllPoint.getArray()[2];
	// y = controllPoint.getArray()[3];
	// glm::vec2 P1 = glm::vec2(x, y);
	// x = controllPoint.getArray()[4];
	// y = controllPoint.getArray()[5];
	// glm::vec2 P2 = glm::vec2(x, y);
	// x = controllPoint.getArray()[6];
	// y = controllPoint.getArray()[7];
	// glm::vec2 P3 = glm::vec2(x, y);
	// float step = 1.0f / GRAN;
	// for (int i = 0; i <= GRAN; ++i) {
	// 	float t = step * i;
	// 	glm::vec2 t0 = P0 * float(pow((1-t), 3));
	// 	glm::vec2 t1 = P1 * float(pow((1-t), 2) * t);
	// 	glm::vec2 t2 = P2 * float((1-t) * pow((t), 2));
	// 	glm::vec2 t3 = P3 * float(pow(t, 3));
	// 	glm::vec2 output = t0 + t1 + t2 + t3;
	// 	g_vertex_buffer_data[i*2] = output[0];
	// 	g_vertex_buffer_data[i*2 + 1] = output[1];
	// 	//printf("(%f, %f at %f\n", output[0], output[1], t);
	// }i*2] = output[0];
	// 	g_vertex_buffer_data[i*2 + 1] = output[1];
	// 	//printf("(%f, %f at %f\n", output[0], output[1], t);
	// }
}
class BesselCurve;
ControllPoint* golbal_controll_pointer;
BesselCurve* global_bessel_curve;
class BesselCurve {
public:
	BesselCurve(int programID) : programID(programID){
		glGenVertexArrays(1, &VOA);
		glGenBuffers(1, &vertexbuffer);
		glGenBuffers(1, &colorbuffer);

		glBindVertexArray(VOA);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(g_color_data),
			g_color_data,
			GL_STATIC_DRAW
		);
	}
	void update() {
		glBindVertexArray(VOA);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(g_vertex_data),
			g_vertex_data,
			GL_STATIC_DRAW
		);

	}
	void initAttribute() {
		glBindVertexArray(VOA);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			2 * sizeof(GL_FLOAT),
			(void*) 0
		);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);
		glEnableVertexAttribArray(1);
	}

	void draw() {
		glBindVertexArray(VOA);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		GLint colorLocation = glGetUniformLocation(programID, "oc");
		glUniform4f(colorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_LINE_STRIP, 0, GRAN + 1);
	}
	void calculatePoints(GLfloat* controllPoint) {
		float x = controllPoint[0];
		float y = controllPoint[1];
		glm::vec2 P0 = glm::vec2(x, y);
		x = controllPoint[2];
		y = controllPoint[3];
		glm::vec2 P1 = glm::vec2(x, y);
		x = controllPoint[4];
		y = controllPoint[5];
		glm::vec2 P2 = glm::vec2(x, y);
		x = controllPoint[6];
		y = controllPoint[7];
		glm::vec2 P3 = glm::vec2(x, y);
		// printf("p0(%f, %f)\n", P0[0], P0[1]);
		// printf("p1(%f, %f)\n", P1[0], P1[1]);
		// printf("p2(%f, %f)\n", P2[0], P2[1]);
		// printf("p3(%f, %f)\n", P3[0], P3[1]);
		float step = 1.0f / GRAN;
		for (int i = 0; i <= GRAN; ++i) {
			float t = step * i;
			glm::vec2 t0 = P0 * float(pow((1-t), 3));
			glm::vec2 t1 = P1 * float(pow((1-t), 2) * t * 3);
			glm::vec2 t2 = P2 * float((1-t) * pow((t), 2) * 3);
			glm::vec2 t3 = P3 * float(pow(t, 3));
			glm::vec2 output = t0 + t1 + t2 + t3;
			g_vertex_data[2*i] = output[0];
			g_vertex_data[2*i + 1] = output[1];
		}
	}

private:
	GLuint VOA;
	GLuint vertexbuffer;
	GLuint colorbuffer;
	GLfloat g_vertex_data[2 * (GRAN + 1)];
	GLfloat g_color_data[3] = {
		1.0f, 0.0f, 0.0f
	};
	int programID;
};
int main(void)
{
	printf("in main\n");
	fflush(stdout); 
	// Initialise GLFW
	INIT_BESEL();
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
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "xx", NULL, NULL);
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

	// copy vertex, index and color into VBO 

//NOTICE: comment start here
/*
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(g_vertex_buffer_data), //NOTICE: mannually calculate size
	 	g_vertex_buffer_data, 
	 	GL_STATIC_DRAW
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbound

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
		2, 
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(GL_FLOAT),
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
	*/


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

		// set time to change by time
		// float timeValue = glfwGetTime();
		// float color_value = (sin(timeValue) / 3.0) + 0.5;
		// GLint colorLocation = glGetUniformLocation(programID, "oc");
		// glUniform4f(colorLocation, color_value, color_value, color_value, 1.0f);
		// glUniform4f(colorLocation, color_value, color_value, color_value, 1.0f);

		// calculate mvp
		// glm::mat4 Projection = glm::perspective(glm::radians(45.0f), float(16)/9, 0.1f, 100.0f);
		// glm::mat4 View = glm::lookAt(
		// 	glm::vec3(0, 0, 8),
		// 	glm::vec3(0, 0, 0),
		// 	glm::vec3(0, 1, 0)
		// );
		// glm::mat4 Identity = glm::mat4(1.0f);
		// glm::mat4 Rotate = glm::rotate(Identity, float(spin_rotate), glm::vec3(0, 1, 0));
		// glm::mat4 Scale = glm::scale(Identity, glm::vec3(0.5, 0.5, 0.5));
		// glm::mat4 Translation = glm::translate(Identity, glm::vec3(3, 0, 0));
		// glm::mat4 Revolution = glm::rotate(Identity, float(revolution_rotate), glm::vec3(0, 1, 0));
		// glm::mat4 Model = Revolution * Translation * Scale * Rotate;
		// glm::mat4 mvp = Projection * View * Model;

		// GLuint MVP_ID = glGetUniformLocation(programID, "MVP");
		// glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &mvp[0][0]);

		glBindVertexArray(VertexArrayID);

//NOTICE: comment start here
/*
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, 
			sizeof(g_vertex_buffer_data), //NOTICE: mannually calculate size
			g_vertex_buffer_data, 
			GL_STATIC_DRAW
		);
*/
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
	//glDeleteBuffers(1, &vertexbuffer);
//	glDeleteBuffers(1, &colorbuffer);
	//glDeleteVertexArrays(1, &VertexArrayID);
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
		INIT_BESEL();
	}
}