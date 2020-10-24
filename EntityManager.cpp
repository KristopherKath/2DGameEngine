#include "EntityManager.h"
#include <iostream>

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

//Renders each entity in Entity Manager
void EntityManager::Render()
{
	for (auto& Entity : entities)
	{
		Entity->Render();
	}
}

//Adds an entity to list of entities and returns the entity made
Entity& EntityManager::AddEntity(std::string EntityName)
{
	Entity* entity = new Entity(*this, EntityName);
	entities.emplace_back(entity);
	return *entity;
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
