#include "Raycaster.h"

Raycaster::Raycaster() {

}

Raycaster::Raycaster(Shader s, float width, float height) {
	shaderProgram = s;
	winWidth = width;
	winHeight = height;
	lastFrame = 0.0;
	
	glm::vec2 playerPos = glm::vec2(0.0, -0.1);
	glm::vec3 playerColor = glm::vec3(1.0, 1.0, 0.0);
	float playerSpeed = 0.5;

	static float vertices[] = {
		0.01,  0.01, 0.0, // Top Right
	   -0.01,  0.01, 0.0, // Top Left
		0.01, -0.01, 0.0, // Bottom Right
	   -0.01, -0.01, 0.0  // Bottom Left
	};

	static unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	player = Player(playerPos, playerColor, playerSpeed, vertices, sizeof(vertices), indices, sizeof(indices));

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Raycaster::display() {
	lastFrame = glfwGetTime();

	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.use();

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	shaderProgram.setMat4("view", view);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), winWidth / winHeight, 0.1f, 100.0f);
	shaderProgram.setMat4("projection", projection);

	drawPlayer();

	glUseProgram(0);
}

void Raycaster::drawPlayer() {
	// Move player
	// -----------
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(player.getPosition(), 0.0));
	shaderProgram.setMat4("model", model);

	shaderProgram.setVec3("pColor", player.getColor());
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Raycaster::movePlayer(int direction) {
	double currentFrame = glfwGetTime();
	double speed = player.getSpeed() * (currentFrame - lastFrame);
	lastFrame = currentFrame;

	glm::vec2 playerPos = player.getPosition();
	switch (direction) {
		case RAY_UP:
			playerPos += glm::vec2(0, speed);
			break;
		case RAY_DOWN:
			playerPos -= glm::vec2(0, speed);
			break;
		case RAY_LEFT:
			playerPos -= glm::vec2(speed, 0);
			break;
		case RAY_RIGHT:
			playerPos += glm::vec2(speed, 0);
			break;
	}
	player.setPosition(playerPos);
}

void Raycaster::setWindowXY(float width, float height)
{
	winWidth = width;
	winHeight = height;
}

