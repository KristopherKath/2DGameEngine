#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

class Component;
class EntityManager;

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

	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args)
	{
		T* NewComponent(new T(std::forward<TArgs>(args)...));
		NewComponent->Owner = this;
		Components.emplace_back(NewComponent);
		NewComponent->Initialize();
		return *NewComponent;
	}
};
#endif // !ENTITY_H

