#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component 
{
public:
	Entity* Owner;
	virtual ~Component() {}
	virtual void Initialize() {}
	virtual void Update(float DeltaTime) {}
	virtual void Render() {}
};
#endif // !COMPONENT_H

