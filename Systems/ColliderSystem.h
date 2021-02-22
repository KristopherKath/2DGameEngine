#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/ColliderComponent.h"

class ColliderSystem : public System
{
public:
	std::vector<ColliderComponent*> colliders;
	std::vector<int> positionsToRemove;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		int i = 0;
		for (ColliderComponent* c : colliders)
		{
			if (c == nullptr) { continue; }

			c->collider.x = static_cast<int>(c->transform->position.x);
			c->collider.y = static_cast<int>(c->transform->position.y);
			c->collider.w = c->transform->width * c->transform->scale;
			c->collider.h = c->transform->height * c->transform->scale;
			c->destinationRectangle.x = c->collider.x - Game::camera.x;
			c->destinationRectangle.y = c->collider.y - Game::camera.y;

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
			colliders.erase(colliders.begin() + i);
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
		std::vector<Entity*> entities = entityManager->GetEntities();

		for (Entity* e : entities)
		{
			if (e->HasComponent<ColliderComponent>())
			{
				//std::cout << "Adding Collider Component Reference" << std::endl;
				colliders.push_back(e->GetComponent<ColliderComponent>());
			}
		}
	};
};