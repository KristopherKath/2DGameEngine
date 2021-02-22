#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/ProjectileEmitterComponent.h"


class ProjectileEmitterSystem : public System
{
public:
	std::vector<ProjectileEmitterComponent*> projectiles;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		for (ProjectileEmitterComponent* projectile : projectiles)
		{
			if (projectile == nullptr) { continue; }

			if (glm::distance(projectile->GetTransformComp()->position, projectile->GetOrigin()) > projectile->GetRange())
			{
				//If the projectile should loop then loop it, else destroy it
				if (projectile->GetShouldLoop())
				{
					projectile->GetTransformComp()->position.x = projectile->GetOrigin().x;
					projectile->GetTransformComp()->position.y = projectile->GetOrigin().y;
				}
				else
				{
					projectile->owner->Destroy();
				}
			}
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
		std::cout << "Adding all ProjectileEmitter components" << std::endl;
		std::vector<Entity*> entities = entityManager->GetEntities();

		for (Entity* e : entities)
		{
			if (e->HasComponent<ProjectileEmitterComponent>())
			{
				//std::cout << "Adding ProjectileEmitter Component Reference" << std::endl;
				projectiles.push_back(e->GetComponent<ProjectileEmitterComponent>());
			}
		}
	};
};