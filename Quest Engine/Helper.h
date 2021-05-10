#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtx/random.hpp>

#include <vector>

#include "Entity.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Coin.h"
#include "Player.h"
#include "Light.h"

//generate random positions
//used for coins but can be adapted to other items
glm::vec3 randomPosition(glm::vec2 upperBound, glm::vec2 lowerBound) {
	glm::vec3 position(0.0f);

	position.x = glm::compRand1(lowerBound.x, upperBound.x) * 0.9f;
	position.z = glm::compRand1(lowerBound.y, upperBound.y) * 0.9f;

	return position;
}

//assume radius of coins are 0.5 and radius of player is 1.0
//returns the index that needs to be popped
int checkCollision(std::vector<Coin*> coins, glm::vec3 playerPos)
{
	for (int i = 0; i < coins.size(); i++)
	{
		if (glm::distance(playerPos, coins[i]->position) <= 2.5f)
		{
			return i;
		}
	}

	return -1;
}

//check collision between player and projectile
int checkCollision(std::vector<Projectile*> projectiles, glm::vec3 playerPos)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		glm::vec3 proPos = projectiles[i]->position;
		proPos.y = 0.0f;
		if (glm::distance(playerPos, proPos) <= 2.0f)
		{
			return i;
		}
	}

	return -1;
}

//check projectiles
int checkProjectiles(std::vector<Projectile*> projectiles)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		if (projectiles[i]->terminate())
		{
			return i;
		}
	}
	return -1;
}