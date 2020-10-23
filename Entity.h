#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

class EntityManager;
class Component;

class Entity
{
private:
	EntityManager& Manager;
	bool bIsActive;
	std::vector<Component*> Components;

public:
	std::string Name;
	Entity(EntityManager& Manager);
	Entity(EntityManager& Manager, std::string name);
	void Update(float DeltaTime);
	void Render();
	void Destroy();
	bool IsActive() const;
};
#endif // !ENTITY_H

