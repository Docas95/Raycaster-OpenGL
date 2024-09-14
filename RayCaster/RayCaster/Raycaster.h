#pragma once

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Player.h"

enum KEYS {
	RAY_UP = 1,
	RAY_DOWN,
	RAY_LEFT,
	RAY_RIGHT,
};

class Raycaster
{
	private:
		// Shader data
		// -----------
		Shader shaderProgram;
		unsigned int VAO, VBO, EBO;
		float winWidth, winHeight;

		// Player Data
		// -----------
		Player player;

		// Frame Data
		// ----------
		double lastFrame;
	public:
		Raycaster();
		Raycaster(Shader s, float width, float height);

		void display();
		void drawPlayer();

		void movePlayer(int direction);

		void setWindowXY(float width, float height);
};

