#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Component.h"
#include "Entity.h"
#include <vector>

class EntityManager
{
private:
	std::vector<Entity*> Entities;

public:
	void ClearData();
	void Update(float DeltaTime);
	void Render();
	bool HasNoEntities();
	Entity& AddEntity(std::string entityName);
	std::vector<Entity*> GetEntities() const;
	unsigned int GetEntityCount();

};

#endif