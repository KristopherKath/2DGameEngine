#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>
#include <typeinfo>

class Component;
class EntityManager;

class Entity
{
private:
	EntityManager& Manager;
	bool bIsActive;
	std::vector<Component*> Components;
	std::map<const std::type_info*, Component*> ComponentTypeMap;
public:
	std::string Name;
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
		NewComponent->Owner = this;
		Components.emplace_back(NewComponent);
		ComponentTypeMap[&typeid(*NewComponent)] = NewComponent;
		NewComponent->Initialize();
		return *NewComponent;
	}

	//Will retrieve a component of type T from the ComponentTypeMap
	template <typename T>
	T* GetComponent()
	{
		return static_cast<T*>(ComponentTypeMap[&typeid(T)]);
	}
};
#endif // !ENTITY_H

