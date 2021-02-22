#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/TileComponent.h"


class TileSystem : public System
{
public:
	std::vector<TileComponent*> tiles;
	std::vector<int> positionsToRemove;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		int i = 0;
		for (TileComponent* tile : tiles)
		{
			if (tile == nullptr) 
			{
				positionsToRemove.push_back(i);
				continue;
			}

			tile->destinationRectangle.x = tile->position.x - Game::camera.x;
			tile->destinationRectangle.y = tile->position.y - Game::camera.y;
			++i;
		}
		RemoveNullComponents();
	}

	//Removes null pointers from list
	void RemoveNullComponents()
	{
		for (int i : positionsToRemove)
		{
			std::cout << "REMOVING NULL PTR" << std::endl;
			tiles.erase(tiles.begin() + i);
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
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