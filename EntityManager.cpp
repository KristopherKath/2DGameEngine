#include "EntityManager.h"
#include <iostream>
#include "Collision.h"
#include "Components\ColliderComponent.h"

//Deletes all entities in the Entity Manager
void EntityManager::ClearData()
{
	for (auto& Entity : entities)
	{
		Entity->Destroy();
	}
}

//Returns true if there are no entities in Entity Manager
bool EntityManager::HasNoEntities() const
{
	return entities.size() == 0;
}

//Updates each entity in Entity Manager
void EntityManager::Update(float DeltaTime)
{
	for (auto& Entity : entities)
	{
		Entity->Update(DeltaTime);
	}
}

//Renders each entity in Entity Manager in order of layer numbers defined in Constants.h
void EntityManager::Render()
{
	for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++)
	{
		for (auto& entity : GetEntitiesByLayer(static_cast<LayerType>(layerNumber)))
		{
			entity->Render();
		}
	}
}

//Adds an entity to list of entities and returns the entity made
Entity& EntityManager::AddEntity(std::string EntityName, LayerType layer)
{
	Entity* entity = new Entity(*this, EntityName, layer);
	entities.emplace_back(entity);
	return *entity;
}

//Get entities by its given layer type
std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const
{
	std::vector<Entity*> selectedEntities;
	for (auto& entity : entities)
	{
		//if the entity layer is the same as given layer
		if (entity->layer == layer)
		{
			//add entity to list to return
			selectedEntities.emplace_back(entity);
		}
	}
	return selectedEntities;
}

//Gets the Entity list
std::vector<Entity*> EntityManager::GetEntities() const
{
	return entities;
}

//Returns how many entities there are 
unsigned int EntityManager::GetEntityCount() const
{
	return entities.size();
}

//Returns the type of collision of two colliding entities
CollisionType EntityManager::CheckCollisions() const
{
	//Check each entity
	for (int i = 0; i < entities.size() - 1; i++) 
	{
		auto& thisEntity = entities[i];

		//If this entity has a collider component
		if (thisEntity->HasComponent<ColliderComponent>())
		{

			//Get the this entities collider component
			ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();

			//Check each entity after the selected entity
			for (int j = i + 1; j < entities.size(); j++) 
			{
				auto& thatEntity = entities[j];

				//If the entity names are different and the other entity has a collider component
				if (thisEntity->name.compare(thatEntity->name) != 0 && thatEntity->HasComponent<ColliderComponent>()) 
				{

					//Get the other entities collider
					ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();

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
	}
	return NO_COLLISION;
}

//Displays all Entities and their info
void EntityManager::ListAllEntities() const
{
	unsigned int i = 0;
	for (auto& Entity : entities)
	{
		std::cout << "Entity[" << i << "]:" << Entity->name << std::endl;
		Entity->ListAllComponents();
		i++;
	}
}
