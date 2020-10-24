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

//Looks through all entities to check collision with given entity
std::string EntityManager::CheckEntityCollisions(Entity& myEntity) const
{
	//Get the collider of given entity
	ColliderComponent* myCollider = myEntity.GetComponent<ColliderComponent>();

	//loop through all entities
	for (auto& entity : entities)
	{
		//if the entity is not the same as given entity and not a tile
		if (entity->name.compare(myEntity.name) != 0 && entity->name.compare("Tile") != 0)
		{
			//If the entity has a collider componenet
			if (entity->HasComponent<ColliderComponent>())
			{
				//Check if the colliders collide with each other
				ColliderComponent* otherCollider = entity->GetComponent<ColliderComponent>();
				if (Collision::CheckRectangleCollision(myCollider->collider, otherCollider->collider))
				{
					//return the tag of colliding entity
					return otherCollider->colliderTag;
				}
			}
		}
	}
	return std::string();;
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
