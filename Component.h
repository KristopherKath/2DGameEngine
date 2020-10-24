#ifndef COMPONENT_H
#define COMPONENT_H
#pragma once

#include <string>

class Entity;

class Component 
{
public:
	Entity* owner;
	virtual ~Component() {}
	virtual void Initialize() {}
	virtual void Update(float DeltaTime) {}
	virtual void Render() {}
	std::string componentName;
};
#endif // !COMPONENT_H

