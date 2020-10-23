#include "Entity.h"
#include "EntityManager.h"
#include "Component.h"


//Constructors
Entity::Entity(EntityManager& Manager) : Manager(Manager)
{
	this->bIsActive = true;
}
//Constructor given a name
Entity::Entity(EntityManager& Manager, std::string Name) : Manager(Manager), Name(Name)
{
	this->bIsActive = true;
}

//Updates each component in this entity
void Entity::Update(float DeltaTime)
{
	for (auto& Component : Components)
	{
		Component->Update(DeltaTime);
	}
}

//Renders each component in this entity
void Entity::Render()
{
	for (auto& Component : Components)
	{
		Component->Render();
	}
}

//Destroy entity by setting it to not active
void Entity::Destroy()
{
	this->bIsActive = false;
}

//Returns whether this entity is active or not
bool Entity::IsActive() const
{
	return this->bIsActive;
}