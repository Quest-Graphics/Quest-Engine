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

//assume radius of coins and projectiles is 0.5 and radius of player is 1.0
//returns the index that needs to be popped
int checkCollision(std::vector<Coin*> coins, Player* player)
{
	for (int i = 0; i < coins.size(); i++)
	{
		if (glm::distance(player->position, coins[i]->position) <= 2.5f)
		{
			return i;
		}
	}

	return -1;
}

int checkCollision(std::vector<Projectile*> projectiles, Player* player)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		if (glm::distance(player->position, projectiles[i]->position) <= 2.5f)
		{
			return i;
		}
	}

	return -1;
}

//emitts projectiles from enemy position
void shootProjectiles(std::vector<Enemy*> enemies, std::vector<Projectile*> projectiles, std::vector<Light*> lights, Player* player, float currentTime, Shader* shader)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->fire(currentTime))
		{
			Projectile* projectile = new Projectile(enemies[i]->position, player->position - enemies[i]->position, shader);
			projectiles.push_back(projectile);

			//create new light and tie it to the projectile
			Light* light = new Light();
			light->position = projectile->position;
			light->color = glm::vec3(1.0f);

			light->ambient = glm::vec3(0.5f);
			light->diffuse = glm::vec3(0.75f);
			light->specular = glm::vec3(1.0f);

			light->constant = 0.5f;
			light->linear = 0.032f;
			light->quadratic = 0.01f;
			lights.push_back(light);
		}
	}
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