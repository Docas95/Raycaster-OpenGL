#include <iostream>
#include <filesystem>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../shaders/Shader.h"
#include "../raycaster/Raycaster.h"
#include "../raycaster/Player.h"

// Global vars
// -----------
Shader shaderProgram;
const char* vertexShaderSource = "shaders/vertexShader.txt";
const char* fragmentShaderSource = "shaders/fragmentShader.txt";

GLFWwindow* window;
float winWidth = 1024.0;
float winHeight = 512.0;

Raycaster raycaster;

// Prototypes
// ----------
void init();
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void handleInput(GLFWwindow* window);

int main(void) {
	init();

	// Main Loop
	// ---------
	while (!glfwWindowShouldClose(window)) {
		handleInput(window);

		raycaster.display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void init() {
	// Initial Set up
	// --------------

	// GLFW: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW: create window
	window = glfwCreateWindow(winWidth, winHeight, "Hello Raytracing", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// GLAD: initialize openGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR: COULD NOT LOAD OPENGL FUNCTION POINTERS\n" << std::endl;
	}

	// Set up Shader Program
	// ---------------------
	shaderProgram = Shader(vertexShaderSource, fragmentShaderSource);

	// Set up Raycaster
	// ----------------
	std::vector<int> map = {
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 2, 2, 2, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1
	};

	Player p = Player(glm::vec2(100, 200), glm::vec3(1.0, 1.0, 0.0), 32.0);

	raycaster = Raycaster(shaderProgram, winWidth, winHeight, map, 8, 8, p);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	winWidth = width;
	winHeight = height;

	raycaster.setWindowXY(winWidth, winHeight);
	
	glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		raycaster.movePlayer(RAY_UP);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		raycaster.movePlayer(RAY_LEFT);
	}
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		raycaster.movePlayer(RAY_DOWN);
	}
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		raycaster.movePlayer(RAY_RIGHT);
	}
}