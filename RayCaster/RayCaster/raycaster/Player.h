#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.1415926535

class Player
{
	private:
		glm::vec2 position;
		glm::vec2 deltaPosition;
		float angle;
		glm::vec3 color;
		float speed;
	public:
		Player();
		Player(glm::vec2 pos, glm::vec3 color, float speed);
		
		glm::vec3 getColor();
		glm::vec2 getPosition();
		glm::vec2 getDeltaPosition();
		float getAngle();
		float getSpeed();

		void setPosition(glm::vec2 newPos);
		void setDeltaPosition(glm::vec2 newDeltaPos);
		void setAngle(float newAngle);
};

