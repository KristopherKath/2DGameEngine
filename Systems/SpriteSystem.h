#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/SpriteComponent.h"


class SpriteSystem : public System
{
public:
	std::vector<SpriteComponent*> sprites;
	std::vector<int> positionsToRemove;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		int i = 0;
		for (SpriteComponent* sc : sprites)
		{
			if (sc == nullptr) 
			{
				positionsToRemove.push_back(i);
				continue;
			}

			//animate the texture
			if (sc->GetIsAnimated())
			{
				//changes the frame in the animation (column of spritesheet)
				sc->sourceRectangle.x = sc->sourceRectangle.w * static_cast<int>((SDL_GetTicks() / sc->GetAnimationSpeed()) % sc->GetNumFrames());
			}
			sc->sourceRectangle.y = sc->GetAnimationIndex() * sc->GetTransformComp()->height; //sets the row of the spritesheet

			//updates the sprite to camera screen space if not fixed
			sc->destinationRectangle.x = static_cast<int>(sc->GetTransformComp()->position.x) - (sc->GetIsFixed() ? 0 : Game::camera.x);
			sc->destinationRectangle.y = static_cast<int>(sc->GetTransformComp()->position.y) - (sc->GetIsFixed() ? 0 : Game::camera.y);
			sc->destinationRectangle.w = sc->GetTransformComp()->width * sc->GetTransformComp()->scale;
			sc->destinationRectangle.h = sc->GetTransformComp()->height * sc->GetTransformComp()->scale;

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
			sprites.erase(sprites.begin() + i);
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
		std::vector<Entity*> entities = entityManager->GetEntities();

		for (Entity* e : entities)
		{
			if (e->HasComponent<SpriteComponent>())
			{
				//std::cout << "Adding Sprite Component Reference" << std::endl;
				sprites.push_back(e->GetComponent<SpriteComponent>());
			}
		}
	};
};