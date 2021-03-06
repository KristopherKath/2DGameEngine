#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#pragma once

#include "Component.h"
#include "Constants.h"
#include "Entity.h"
#include <vector>
#include <iostream>

class EntityManager
{
private:
	std::vector<Entity*> entities;

public:
	void ClearData();
	void Update(float DeltaTime);
	void Render();
	void DestroyInactiveEntities();
	void ListAllEntities() const;
	bool HasNoEntities() const;
	Entity& AddEntity(std::string entityName, LayerType layer);
	std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
	std::vector<Entity*> GetEntities() const;
	Entity* GetEntityByName(std::string name) const;
	unsigned int GetEntityCount() const;

	//CollisionType CheckCollisions() const;
};

#endif