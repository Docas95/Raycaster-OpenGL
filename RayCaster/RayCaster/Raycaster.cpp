#include "Raycaster.h"

Raycaster::Raycaster() {

}

Raycaster::Raycaster(Shader s, float width, float height) {
	// Shader data
	// -----------
	shaderProgram = s;
	winWidth = width;
	winHeight = height;
	lastFrame = 0.0;
	
	// Player data
	// -----------
	glm::vec2 playerPos = glm::vec2(300, 300);
	glm::vec3 playerColor = glm::vec3(1.0, 1.0, 0.0);
	float playerSpeed = 128;
	player = Player(playerPos, playerColor, playerSpeed);

	// Map data
	// --------
	std::vector<int> m = {
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 1, 0, 0, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1
	};
	map = Map(8, 8, m, 64);

	// Load vertex data and define vertexAttributes
	// --------------------------------------------
	float vertices[] = {
		// Player
		 4.00,  4.00, 0.0, // Top Right
	    -4.00,  4.00, 0.0, // Top Left
		 4.00, -4.00, 0.0, // Bottom Right
	    -4.00, -4.00, 0.0,  // Bottom Left

	    // Wall
		 32.00,  32.00, 0.0, // Top Right
		-32.00,  32.00, 0.0, // Top Left
		 32.00, -32.00, 0.0, // Bottom Right
		-32.00, -32.00, 0.0  // Bottom Left
	};

	unsigned int indices[] = {
		// Player
		0, 1, 2,
		1, 2, 3,

		// Wall
		4, 5, 6,
		5, 6, 7
	};

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

	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 512.0f, 0.0f);
	shaderProgram.setMat4("projection", projection);

	drawMap();
	drawPlayer();

	glUseProgram(0);
}

void Raycaster::drawPlayer() {
	// Move player to correct position
	// -------------------------------
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(player.getPosition(), 0.0));
	shaderProgram.setMat4("model", model);

	shaderProgram.setVec3("pColor", player.getColor());
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Raycaster::drawMap() {
	glm::mat4 model;
	glm::vec2 wallPosition;
	glm::vec3 wallColor;
	float wallSize = map.getWallSize();

	std::vector<int> m = map.getMapMap();
	int x = map.getMapX();
	int y = map.getMapY();

	glBindVertexArray(VAO);
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			wallPosition = glm::vec2(32, 32) + glm::vec2(j * (wallSize+2), i * (wallSize+2));
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(wallPosition, 0.0));
			shaderProgram.setMat4("model", model);

			m[i * x + j] == 1 ? wallColor = glm::vec3(1.0, 1.0, 1.0) : wallColor = glm::vec3(0.0, 0.0, 0.0);
			shaderProgram.setVec3("pColor", wallColor);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
		}
	}
	glBindVertexArray(0);
}

void Raycaster::movePlayer(int direction) {
	double currentFrame = glfwGetTime();
	double speed = player.getSpeed() * (currentFrame - lastFrame);
	lastFrame = currentFrame;

	glm::vec2 playerPos = player.getPosition();
	switch (direction) {
		case RAY_UP:
			playerPos -= glm::vec2(0, speed);
			break;
		case RAY_DOWN:
			playerPos += glm::vec2(0, speed);
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

