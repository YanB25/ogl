#ifndef __BESSEL_CURVE_H_
#define __BESSEL_CURVE_H_
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <cmath>

#include <glm/glm.hpp>
#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "config.hpp"

using namespace glm;
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
    ~BesselCurve() {
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &colorbuffer);
        glDeleteVertexArrays(1, &VOA);
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


#endif