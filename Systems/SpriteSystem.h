#pragma once
#include "System.h"
#include <iostream>
#include "../Entity.h"
#include "../Components/SpriteComponent.h"


class SpriteSystem : public System
{
public:
	std::vector<SpriteComponent*> sprites;

	//Override update in parent to handle transform components
	void Update(float deltaTime) override
	{
		for (SpriteComponent* sc : sprites)
		{
			if (sc == nullptr) { continue; }

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
		}
	}

	//Will add each of the desired components into the system
	void AddComponents(EntityManager* entityManager) override
	{
		std::cout << "Adding all Sprite components" << std::endl;
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