#include "Entity.h"
#include <iostream>

//Constructors
Entity::Entity(EntityManager& Manager) : manager(Manager)
{
	this->isActive = true;
}
//Constructor given a name
Entity::Entity(EntityManager& Manager, std::string Name) : manager(Manager), name(Name)
{
	this->isActive = true;
}

//Updates each component in this entity
void Entity::Update(float DeltaTime)
{
	for (auto& Component : components)
	{
		Component->Update(DeltaTime);
	}
}

//Renders each component in this entity
void Entity::Render()
{
	for (auto& Component : components)
	{
		Component->Render();
	}
}

//Destroy entity by setting it to not active
void Entity::Destroy()
{
	this->isActive = false;
}

//Returns whether this entity is active or not
bool Entity::IsActive() const
{
	return this->isActive;
}

//Lists all componentes
void Entity::ListAllComponents() const
{
	for (auto& mapElement : componentTypeMap)
	{
		std::cout << "   Component<" << mapElement.first->name() << ">" << std::endl;
	}
}
