#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/TransformComponent.h"

class TransformSystem : public System
{
public:
	std::vector<TransformComponent*> transforms;
	std::vector<int> positionsToRemove;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		int i = 0;
		for (TransformComponent* transform : transforms)
		{
			if (transform == nullptr) 
			{ 
				positionsToRemove.push_back(i);
				continue; 
			}

			//Change the objects position based on velocity
			transform->position.x += transform->velocity.x * deltaTime;
			transform->position.y += transform->velocity.y * deltaTime;
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
			transforms.erase(transforms.begin() + i);
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
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