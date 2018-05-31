#ifndef __CONTROLL_POINT_H_
#define __CONTROLL_POINT_H_
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <cmath>

#include <glm/glm.hpp>
#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "config.hpp"
class ControllPoint{
public:
	ControllPoint(int programID) : num_of_points(0), programID(programID) {
		printf("init start\n");
		fflush(stdout); 
		glGenVertexArrays(1, &VOA);
		printf("ok1\n");
		fflush(stdout);
		glGenBuffers(1, &vertexbuffer);

		printf("init finish");
		fflush(stdout); 
	}
	void update() {
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBindVertexArray(VOA);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(g_vertex_controll_point_buffer_data),
			g_vertex_controll_point_buffer_data,
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

	}
	void draw() {
		glBindVertexArray(VOA);
		GLint colorLocation = glGetUniformLocation(programID, "oc");
		glUniform4f(colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
		if (num_of_points == 4) {
			glDrawArrays(GL_LINE_STRIP, 0, num_of_points);
		} else {
			glPointSize(5);
			glDrawArrays(GL_POINTS, 0, num_of_points);
		}

	}
	void addPoint(double x, double y) {
		if (num_of_points == 4) {
			num_of_points = 0;
		}
		g_vertex_controll_point_buffer_data[num_of_points*2] = x;
		g_vertex_controll_point_buffer_data[num_of_points*2 + 1] = y;
		num_of_points++;
	}
	GLfloat* getArray() {
		return g_vertex_controll_point_buffer_data;
	}
	int getNumOfPoint() const { return num_of_points; }
private:
	GLuint VOA;
	GLuint vertexbuffer;
	GLfloat g_vertex_controll_point_buffer_data[4 * 2] = {
		// not init here
		// -0.0f, -0.0f,
		// 0.0f, -0.0f,
		// 0.0f, -0.0f,
		// 0.0f, 0.0f
	};
	int num_of_points;
	int programID;
};
#endif