#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Player
{
	private:
		glm::vec2 position;
		glm::vec3 deltaPos; 
		glm::vec3 color;
		float speed;
	public:
		Player();
		Player(glm::vec2 pos, glm::vec3 color, float speed);
		
		glm::vec3 getColor();
		glm::vec2 getPosition();
		float getSpeed();

		void setPosition(glm::vec2 newPos);
};

