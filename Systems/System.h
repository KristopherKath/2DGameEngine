#pragma once

//Stores the different entities depending on which component it has to update that component
class System
{
public:
	virtual void Update(float DeltaTime) {};	//will be used to update the state of each component the system contains
	virtual void AddComponents(EntityManager* entityManager) {};
};