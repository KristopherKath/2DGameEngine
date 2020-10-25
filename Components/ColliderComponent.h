#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H
#pragma once

#include <SDL.h>
#include "../Game.h"
#include "../AssetManager.h"
#include "../EntityManager.h"
#include "TransformComponent.h"

class ColliderComponent : public Component
{
public:
	std::string colliderTag;
	SDL_Rect collider;
	SDL_Rect sourceRectangle;
	SDL_Rect destinationRectangle;
	TransformComponent* transform;
	SDL_Texture* colliderSprite;

	//Constructor: Sets collider component based off of given colliderTag, position to be, 
		//width and height, whether to render the collision box, and collision textureID
	ColliderComponent(std::string colliderTag, int x, int y, int width, int height, std::string colliderTextureID)
	{
		colliderSprite = Game::assetManager->GetTexture(colliderTextureID);
		this->colliderTag = colliderTag;
		this->collider = { x, y, width, height };
	}

	//Initialize the component if entity has a transform component
	void Initialize() override
	{
		//if the transform component is on the entity
		if (owner->HasComponent<TransformComponent>())
		{
			transform = owner->GetComponent<TransformComponent>();
			sourceRectangle = { 0, 0, transform->width, transform->height };
			destinationRectangle = { collider.x, collider.y, collider.w, collider.h };
		}
	}
	//Update collider along with the transfrom of the entity
	void Update(float deltaTime) override
	{
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
		destinationRectangle.x = collider.x - Game::camera.x;
		destinationRectangle.y = collider.y - Game::camera.y;
	}

	//Render the collision box if render collider is true
	void Render() override
	{
		if (Game::isDebug) 
		{
			TextureManager::Draw(colliderSprite, sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
		}
	}
};

#endif // !COLLIDERCOMPONENT_H
