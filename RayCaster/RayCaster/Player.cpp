#include "Player.h"

Player::Player() {

}

Player::Player(glm::vec2 pos, glm::vec3 color, float speed) {
	this->position = pos;
	this->color = color;
	this->speed = speed;
	this->angle = 0;
	this->deltaPosition = glm::vec2(cos(this->angle) * 5, sin(this->angle) * 5);
}

glm::vec3 Player::getColor() {
	return color;
}

glm::vec2 Player::getPosition() {
	return position;
}

glm::vec2 Player::getDeltaPosition() {
	return deltaPosition;
}

float Player::getAngle() {
	return angle;
}

float Player::getSpeed() {
	return speed;
}

void Player::setPosition(glm::vec2 newPos) {
	position = newPos;
}

void Player::setDeltaPosition(glm::vec2 newPos) {
	deltaPosition = newPos;
}

void Player::setAngle(float newAngle) {
	angle = newAngle;
}