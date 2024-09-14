#pragma once

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Raycaster
{
	private:
		Shader shaderProgram;
		unsigned int VAO, VBO, EBO;
		float winWidth, winHeight;

		// Player Data
		// -----------
		glm::vec2 playerPos;
		float* playerVertex;
		unsigned int* playerIndices;
		glm::vec3 playerColor;
	public:
		Raycaster();
		Raycaster(Shader s, float width, float height);

		void display();
		void drawPlayer();

		void setWindowXY(float width, float height);
};

