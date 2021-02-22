#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/ColliderComponent.h"
#include "Collision.h"

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



	//Returns the type of collision of two colliding colliders
	CollisionType CheckCollisions() const
	{
		//Check each collider
		for (int i = 0; i < colliders.size() - 1; i++)
		{
			//Get the this entities collider component
			ColliderComponent* thisCollider = colliders[i];

			//Check each collider after the selected collider
			for (int j = i + 1; j < colliders.size(); j++)
			{
				ColliderComponent* thatCollider = colliders[j];

				//If their entity names are different
				if (thisCollider->owner->name.compare(thatCollider->owner->name) != 0)
				{
					//If there is a collision between the two colliders 
					if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider))
					{

						//If the collision is of player and enemy
						if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("ENEMY") == 0)
						{
							return PLAYER_ENEMY_COLLISION;
						}

						//If the collision is of player and projectile
						if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("PROJECTILE") == 0)
						{
							return PLAYER_PROJECTILE_COLLISION;
						}

						//If the collision is of enemy and friendly projectile
						if (thisCollider->colliderTag.compare("ENEMY") == 0 && thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0)
						{
							return ENEMY_PROJECTILE_COLLISION;
						}

						//If the collision is of player and level goal
						if (thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0)
						{
							return PLAYER_LEVEL_COMPLETE_COLLISION;
						}
					}
				}
			}
		}
		return NO_COLLISION;
	}
};