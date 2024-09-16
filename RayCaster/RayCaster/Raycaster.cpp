#include "Raycaster.h"

Raycaster::Raycaster() {

}

Raycaster::Raycaster(Shader s, float width, float height, std::vector<int> m, int mapX, int mapY, Player p) {
	// Shader data
	// -----------
	shaderProgram = s;
	winWidth = width;
	winHeight = height;
	lastFrame = 0.0;
	
	// Player data
	// -----------
	player = p;

	// Map data
	// --------
	map = Map(mapX, mapY, m, mapX * mapY);

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

	// Clear screen
	// ------------
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.use();

	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 512.0f, 0.0f);
	shaderProgram.setMat4("projection", projection);

	drawMap();
	drawRays2D();
	drawPlayer();

	glUseProgram(0);
}

void Raycaster::drawPlayer() {
	// Draw player
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
			// Set wall position
			wallPosition = glm::vec2(32, 32) + glm::vec2(j * (wallSize+1), i * (wallSize+1));
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(wallPosition, 0.0));
			shaderProgram.setMat4("model", model);

			// Draw wall
			m[i * x + j] >= 1 ? wallColor = glm::vec3(1.0, 1.0, 1.0) : wallColor = glm::vec3(0.0, 0.0, 0.0);
			shaderProgram.setVec3("pColor", wallColor);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
		}
	}
	glBindVertexArray(0);
}

void Raycaster::drawRays2D() {
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo, distT = 0;
	glm::vec3 wallColor = glm::vec3(0.9f, 0.0, 0.0);

	glm::vec2 playerPos = player.getPosition();
	int mapX = map.getMapX();
	int mapY = map.getMapY();
	std::vector<int> mapMap = map.getMapMap();

	ra = player.getAngle() - 30 * RADIANS;
	if (ra < 0) ra += 2 * PI;
	if (ra > 2 * PI) ra -= 2 * PI;

	for (r = 0; r < 60; r++) {
		// Check horizontal lines
		// ----------------------
		dof = 0;
		float distH = 100000, hx = playerPos.x, hy = playerPos.y;
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
			dof = mapX;
		}
		while (dof < mapX) {
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mx >= 0 && mx < mapX && my >= 0 && my < mapY) {
				if (mp < mapX * mapY && mapMap[mp] >= 1) { // hit wall
					hx = rx;
					hy = ry;
					distH = calculateDistance(hx, hy, playerPos.x, playerPos.y);
					dof = mapX;
				}
				else {
					rx += xo;
					ry += yo;
					dof += 1;
				}
			}
			else {
				dof = mapX;
			}
		}

		// Check vertical lines
		// ----------------------
		dof = 0;
		float distV = 100000, vx = playerPos.x, vy = playerPos.y;
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
			dof = mapY;
		}
		while (dof < mapY) {
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;
			if (mx >= 0 && mx < mapX && my >= 0 && my < mapY) {
				if (mp < mapX * mapY && mapMap[mp] >= 1) { // hit wall
					vx = rx;
					vy = ry;
					distV = calculateDistance(vx, vy, playerPos.x, playerPos.y);
					dof = mapY;
				}
				else {
					rx += xo;
					ry += yo;
					dof += 1;
				}
			}
			else {
				dof = mapY;
			}
		}

		// Find nearest wall
		// -----------------
		if (distV < distH) {
			rx = vx; ry = vy; distT = distV;
		}
		if (distH < distV) {
			rx = hx; ry = hy; distT = distH;
		}

		// Assign color to 3D wall
		// -----------------------
		mx = (int)(rx) >> 6;
		my = (int)(ry) >> 6;
		mp = my * mapX + mx;
		if (mx >= 0 && mx < mapX && my >= 0 && my < mapY) {
			if (mapMap[mp] == 1) {
				wallColor = glm::vec3(0.9, 0.0, 0.0);
			}
			else {
				wallColor = glm::vec3(0.0, 0.0, 0.9);
			}
		}
		if (distV < distH) wallColor = 0.7f * wallColor;

		float lineVertices[6] = {
			playerPos.x, playerPos.y, 0.0,
			rx, ry, 0.0
		};
		drawLine(lineVertices,wallColor, 3.0);

		// Draw 3D wall
		// -------------
		float ca = player.getAngle() - ra;
		if (ca < 0) ca += 2 * PI;
		if (ca > 2 * PI) ca -= 2 * PI;
		distT = distT * cos(ca);

		float lineH = (map.getWallSize() * 320) / distT; 
		float lineOffset = 160 - lineH / 2;
		if (lineH > winHeight * 2.0) lineH = winHeight * 2.0;

		float lineWallVertices[6] = {
			r * 8 + 530 + 8, lineOffset, 0.0,
			r * 8 + 530 + 8, lineH + lineOffset, 0.0
		};
		drawLine(lineWallVertices, wallColor, winWidth / 128.0);

		// Increment ray angle
		// -------------------
		ra += RADIANS;
		if (ra < 0) ra += 2 * PI;
		if (ra > 2 * PI) ra -= 2 * PI;
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
			playerPos += glm::vec2(playerDeltaPos.x * player.getSpeed() * deltaTime, playerDeltaPos.y * player.getSpeed() * deltaTime);
			break;
		case RAY_DOWN:
			playerPos -= glm::vec2(playerDeltaPos.x * player.getSpeed() * deltaTime, playerDeltaPos.y * player.getSpeed() * deltaTime);
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

float Raycaster::calculateDistance(float ax, float ay, float bx, float by) {
	return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}