#ifndef ENTITY_H
#define ENTITY_H
#pragma once

#include <vector>
#include <string>
#include <map>
#include <typeinfo>
#include "./EntityManager.h"
#include "./Component.h"

class Component;
class EntityManager;

class Entity
{
private:
	EntityManager& manager;
	bool isActive;
	std::vector<Component*> components;
	std::map<const std::type_info*, Component*> componentTypeMap;
public:
	std::string name;
	Entity(EntityManager& Manager);
	Entity(EntityManager& Manager, std::string name);
	void Update(float DeltaTime);
	void Render();
	void Destroy();
	bool IsActive() const;
	void ListAllComponents() const;

	//Will add a component of type T and arguments of TArgs into the Components list and ComponentTypeMap
	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args)
	{
		T* NewComponent(new T(std::forward<TArgs>(args)...));
		NewComponent->owner = this;
		components.emplace_back(NewComponent);
		componentTypeMap[&typeid(*NewComponent)] = NewComponent;
		NewComponent->Initialize();
		return *NewComponent;
	}

	//Will retrieve a component of type T from the ComponentTypeMap
	template <typename T>
	T* GetComponent()
	{
		return static_cast<T*>(componentTypeMap[&typeid(T)]);
	}

	//Will check if component is in componentTypeMap for given component
	template <typename T>
	bool HasComponent() const
	{
		return componentTypeMap.count(&typeid(T));
	}
};
#endif // !ENTITY_H

