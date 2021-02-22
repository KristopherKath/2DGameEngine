#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/ColliderComponent.h"

class ColliderSystem : public System
{
public:
	std::vector<ColliderComponent*> colliders;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		for (ColliderComponent* c : colliders)
		{
			if (c == nullptr) { continue; }

			c->collider.x = static_cast<int>(c->transform->position.x);
			c->collider.y = static_cast<int>(c->transform->position.y);
			c->collider.w = c->transform->width * c->transform->scale;
			c->collider.h = c->transform->height * c->transform->scale;
			c->destinationRectangle.x = c->collider.x - Game::camera.x;
			c->destinationRectangle.y = c->collider.y - Game::camera.y;
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
		std::cout << "Adding all collider components" << std::endl;
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