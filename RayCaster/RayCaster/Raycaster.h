#pragma once

#include <iostream>
#include <vector>
#include <math.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Player.h"
#include "Map.h"

#define PI 3.1415926535
#define RADIANS 0.0174533

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
		double lastFrame;

		// Player data
		// -----------
		Player player;

		// Map data
		// ----------
		Map map;
	public:
		Raycaster();
		Raycaster(Shader s, float width, float height);

		void display();
		void drawPlayer();
		void drawMap();
		void drawRays2D();
		void drawLine(float vertices[6], glm::vec3 color, float lineWidth);

		void movePlayer(int direction);

		void setWindowXY(float width, float height);

		float calculateDistance(float ax, float ay, float bx, float by);
};

