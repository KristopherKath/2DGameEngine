#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/TileComponent.h"


class TileSystem : public System
{
public:
	std::vector<TileComponent*> tiles;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		for (TileComponent* tile : tiles)
		{
			if (tile == nullptr) { continue; }
			tile->destinationRectangle.x = tile->position.x - Game::camera.x;
			tile->destinationRectangle.y = tile->position.y - Game::camera.y;
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
		std::cout << "Adding all Tile components" << std::endl;
		std::vector<Entity*> entities = entityManager->GetEntities();

		for (Entity* e : entities)
		{
			if (e->HasComponent<TileComponent>())
			{
				//std::cout << "Adding Tile Component Reference" << std::endl;
				tiles.push_back(e->GetComponent<TileComponent>());
			}
		}
	};
};