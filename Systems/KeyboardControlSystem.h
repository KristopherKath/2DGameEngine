#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/KeyboardControlComponent.h"

class KeyboardControlSystem : public System
{
public:
	std::vector<KeyboardControlComponent*> keyboards;
	std::vector<int> positionsToRemove;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		int i = 0;
		for (KeyboardControlComponent* kb : keyboards)
		{
			if (kb == nullptr) 
			{ 
				positionsToRemove.push_back(i);
				continue; 
			}

			//When key pressed update the speed/direction of entity and animation
			if (Game::event.type == SDL_KEYDOWN)
			{
				std::string keyCode = std::to_string(Game::event.key.keysym.sym);
				if (keyCode.compare(kb->upKey) == 0)
				{
					kb->transform->velocity.y = -100;
					kb->transform->velocity.x = 0;
					kb->sprite->Play("UpAnimation");
				}
				if (keyCode.compare(kb->rightKey) == 0)
				{
					kb->transform->velocity.y = 0;
					kb->transform->velocity.x = 100;
					kb->sprite->Play("RightAnimation");
				}
				if (keyCode.compare(kb->downKey) == 0)
				{
					kb->transform->velocity.y = 100;
					kb->transform->velocity.x = 0;
					kb->sprite->Play("DownAnimation");
				}
				if (keyCode.compare(kb->leftKey) == 0)
				{
					kb->transform->velocity.y = 0;
					kb->transform->velocity.x = -100;
					kb->sprite->Play("LeftAnimation");
				}
			}

			//When key released update the speed/direction of entity
			if (Game::event.type == SDL_KEYUP)
			{
				std::string keyCode = std::to_string(Game::event.key.keysym.sym);
				if (keyCode.compare(kb->upKey) == 0)
				{
					kb->transform->velocity.y = 0;
				}
				if (keyCode.compare(kb->rightKey) == 0)
				{
					kb->transform->velocity.x = 0;
				}
				if (keyCode.compare(kb->downKey) == 0)
				{
					kb->transform->velocity.y = 0;
				}
				if (keyCode.compare(kb->leftKey) == 0)
				{
					kb->transform->velocity.x = 0;
				}
				if (keyCode.compare(kb->shootKey) == 0)
				{
					// TODO
				}
			}
			++i;
		}
		RemoveNullComponents();
	}

	//Removes null pointers from list
	void RemoveNullComponents()
	{
		for (int i : positionsToRemove)
		{
			std::cout << "REMOVING NULL PTR" << std::endl;
			keyboards.erase(keyboards.begin() + i);
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
		std::vector<Entity*> entities = entityManager->GetEntities();

		for (Entity* e : entities)
		{
			if (e->HasComponent<KeyboardControlComponent>())
			{
				//std::cout << "Adding KeyboardControl Component Reference" << std::endl;
				keyboards.push_back(e->GetComponent<KeyboardControlComponent>());
			}
		}
	};
};