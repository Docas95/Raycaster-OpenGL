#include "Player.h"

Player::Player() {

}

Player::Player(glm::vec2 pos, glm::vec3 color, float speed, float* vertices, int verticesSize, unsigned int* indices, int indicesSize) {
	this->position = pos;
	this->color = color;
	this->speed = speed;

	this->vertices = vertices;
	this->verticesSize = verticesSize;

	this->indices = indices;
	this->indicesSize = indicesSize;
}

glm::vec3 Player::getColor() {
	return color;
}

glm::vec2 Player::getPosition() {
	return position;
}

float Player::getSpeed() {
	return speed;
}

void Player::setPosition(glm::vec2 newPos) {
	position = newPos;
}