#include "EntityManager.h"
#include <iostream>

//Deletes all entities in the Entity Manager
void EntityManager::ClearData()
{
	for (auto& Entity : Entities)
	{
		Entity->Destroy();
	}
}

//Returns true if there are no entities in Entity Manager
bool EntityManager::HasNoEntities()
{
	return Entities.size() == 0;
}

//Updates each entity in Entity Manager
void EntityManager::Update(float DeltaTime)
{
	for (auto& Entity : Entities)
	{
		Entity->Update(DeltaTime);
	}
}

//Renders each entity in Entity Manager
void EntityManager::Render()
{
	for (auto& Entity : Entities)
	{
		Entity->Render();
	}
}

//Adds an entity to list of entities and returns the entity made
Entity& EntityManager::AddEntity(std::string EntityName)
{
	Entity* entity = new Entity(*this, EntityName);
	Entities.emplace_back(entity);
	return *entity;
}

//Gets the Entity list
std::vector<Entity*> EntityManager::GetEntities() const
{
	return Entities;
}

//Returns how many entities there are 
unsigned int EntityManager::GetEntityCount()
{
	return Entities.size();
}

//Displays all Entities and their info
void EntityManager::ListAllEntities() const
{
	for (auto& Entity : Entities)
	{
		std::cout << "Entity Name: " << Entity->Name << "\n    ";
		Entity->ListAllComponents();
	}
}
