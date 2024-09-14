#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

// Global vars
// -----------
Shader shaderProgram;
const char* vertexShaderSource = "vertexShader.txt";
const char* fragmentShaderSource = "fragmentShader.txt";

GLFWwindow* window;
const int winWidth = 800;
const int winHeight = 600;


// Prototypes
// ----------
void setup();
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void handleInput(GLFWwindow* window);

int main(void) {

	setup();

	// Set up vertex data and configure vertex attributes
	float vertexData[] = {
		// Coords			// Color
		0.5, -0.5, 0.0,		1.0, 0.0, 0.0,
	   -0.5, -0.5, 0.0,		0.0, 1.0, 0.0,
	    0.0,  0.5, 0.0,		0.0, 0.0, 1.0
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Main Loop
	// ---------
	while (!glfwWindowShouldClose(window)) {
		handleInput(window);

		// Draw triangle
		glClearColor(0.7, 0.7, 0.7, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void setup() {
	// Initial Set up
	// --------------

	// GLFW: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW: create window
	window = glfwCreateWindow(winWidth, winHeight, "Hello Textures", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// GLAD: initialize openGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR: COULD NOT LOAD OPENGL FUNCTION POINTERS\n" << std::endl;
	}

	// Set up Shader Program
	// ---------------------
	shaderProgram = Shader(vertexShaderSource, fragmentShaderSource);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}