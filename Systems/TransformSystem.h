#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/TransformComponent.h"

class TransformSystem : public System
{
public:
	std::vector<TransformComponent*> transforms;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		for (TransformComponent* transform : transforms)
		{
			if (transform == nullptr) { continue; }

			//Change the objects position based on velocity
			transform->position.x += transform->velocity.x * deltaTime;
			transform->position.y += transform->velocity.y * deltaTime;
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
		std::cout << "Adding all transform components" << std::endl;
		std::vector<Entity*> entities = entityManager->GetEntities();

		for (Entity* e : entities)
		{
			if (e->HasComponent<TransformComponent>())
			{
				//std::cout << "Adding Transform Component Reference" << std::endl;
				transforms.push_back(e->GetComponent<TransformComponent>());
			}
		}
	};
};