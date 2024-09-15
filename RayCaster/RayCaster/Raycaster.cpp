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
	float playerSpeed = 16.0;
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
	drawRays3D();
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

	// Draw player FOV lines
	// ---------------------
	glm::vec2 pos = player.getPosition();
	glm::vec2 deltaPos = player.getDeltaPosition();

	// Vertices for FOV lines
	float lineVertices[] = {
		pos.x, pos.y, 0.0,
		pos.x + deltaPos.x * 5, pos.y + deltaPos.y * 5, 0.0
	};
	drawLine(lineVertices, glm::vec3(1.0,1.0,0.0), 2.0);
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

void Raycaster::drawRays3D() {
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo;

	glm::vec2 playerPos = player.getPosition();
	int mapX = map.getMapX();
	int mapY = map.getMapY();
	std::vector<int> mapMap = map.getMapMap();

	ra = player.getAngle();

	for (r = 0; r < 1; r++) {
		// Check horizontal lines
		// ----------------------
		dof = 0;
		float aTan = -1 / tan(ra);
		if (ra > PI) { // looking down
			ry = (((int)playerPos.y >> 6) << 6) - 0.0001;
			rx = (playerPos.y - ry) * aTan + playerPos.x;
			yo = -64;
			xo = -yo * aTan;
		}
		if (ra < PI) { // looking up
			ry = (((int)playerPos.y >> 6) << 6) + 64;
			rx = (playerPos.y - ry) * aTan + playerPos.x;
			yo = 64;
			xo = -yo * aTan;
		}
		if (ra == 0 || ra == PI) { // looking left or right
			rx = playerPos.x;
			ry = playerPos.y;
			dof = 8;
		}
		while (dof < 8) {
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mx >= 0 && mx < mapX && my >= 0 && my < mapY) {
				if (mp < mapX * mapY && mapMap[mp] == 1) { // hit wall
					dof = 8;
				}
				else {
					rx += xo;
					ry += yo;
					dof += 1;
				}
			}
			else {
				dof = 8;
			}
		}
	
		float hLineVertices[6] = {
			playerPos.x, playerPos.y, 0.0,
			rx, ry, 0.0
		};
		drawLine(hLineVertices, glm::vec3(0.0, 1.0, 0.0), 6.0);

		// Check vertical lines
		// ----------------------
		dof = 0;
		float nTan = -tan(ra);
		if (ra > PI/2 && ra < 3 * PI/2) { // looking left
			rx = (((int)playerPos.x >> 6) << 6) - 0.0001;
			ry = (playerPos.x - rx) * nTan + playerPos.y;
			xo = -64;
			yo = -xo * nTan;
		}
		if (ra < PI/2 || ra > 3 * PI/2) { // looking right
			rx = (((int)playerPos.x >> 6) << 6) + 64;
			ry = (playerPos.x - rx) * nTan + playerPos.y;
			xo = 64;
			yo = -xo * nTan;
		}
		if (ra == PI/2 || ra == 3 * PI/2) { // looking up or down
			ry = playerPos.y;
			rx = playerPos.x;
			dof = 8;
		}
		while (dof < 8) {
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mx >= 0 && mx < mapX && my >= 0 && my < mapY) {
				if (mp < mapX * mapY && mapMap[mp] == 1) { // hit wall
					dof = 8;
				}
				else {
					rx += xo;
					ry += yo;
					dof += 1;
				}
			}
			else {
				dof = 8;
			}
		}

		// Vertices for RAY lines
		float vLineVertices[6] = {
			playerPos.x, playerPos.y, 0.0,
			rx, ry, 0.0
		};
		drawLine(vLineVertices, glm::vec3(1.0, 0.0, 0.0), 3.0);
	}
}

void Raycaster::drawLine(float vertices[6], glm::vec3 color, float lineWidth) {
	unsigned int VAO_RAY, VBO_RAY;
	glGenVertexArrays(1, &VAO_RAY);
	glGenBuffers(1, &VBO_RAY);

	glBindVertexArray(VAO_RAY);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_RAY);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	shaderProgram.setVec3("pColor", color);

	glm::mat4 modelFOV = glm::mat4(1.0f);  // No translation needed
	shaderProgram.setMat4("model", modelFOV);

	glLineWidth(lineWidth);
	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO_RAY);
	glDeleteBuffers(1, &VBO_RAY);
}

void Raycaster::movePlayer(int direction) {
	double currentFrame = glfwGetTime();
	double deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glm::vec2 playerPos = player.getPosition();
	glm::vec2 playerDeltaPos = player.getDeltaPosition();
	float playerAngle = player.getAngle();
	switch (direction) {
		case RAY_UP:
			playerPos += glm::vec2(playerDeltaPos.x * 16.0 * deltaTime, playerDeltaPos.y * 16.0 * deltaTime);
			break;
		case RAY_DOWN:
			playerPos -= glm::vec2(playerDeltaPos.x * 16.0 * deltaTime, playerDeltaPos.y * 16.0 * deltaTime);
			break;
		case RAY_LEFT:
			// Look to the left
			// ----------------
			playerAngle -= 10.0 * deltaTime;
			if (playerAngle < 0) playerAngle += 2 * PI;
			playerDeltaPos = glm::vec2(cos(playerAngle) * 5 , sin(playerAngle) * 5);
			break;
		case RAY_RIGHT:
			// Look to the right
			// -----------------
			playerAngle += 10.0 * deltaTime;
			if (playerAngle > 2*PI) playerAngle -= 2 * PI;
			playerDeltaPos = glm::vec2(cos(playerAngle) * 5, sin(playerAngle) * 5);
			break;
	}
	player.setPosition(playerPos);
	player.setDeltaPosition(playerDeltaPos);
	player.setAngle(playerAngle);
}

void Raycaster::setWindowXY(float width, float height)
{
	winWidth = width;
	winHeight = height;
}

