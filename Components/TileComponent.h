#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H
#pragma once

#include <SDL.h>
#include "../EntityManager.h"
#include "../../../libglm/lib/glm/glm.hpp"
#include "../AssetManager.h"

class TileComponent : public Component
{
public:
	SDL_Texture* texture;
	SDL_Rect sourceRectangle;
	SDL_Rect destinationRectangle;
	glm::vec2 position;

	TileComponent(int sourceRectX, int sourceRectY, int x, int y, int tileSize, int tileScale, std::string assetTextureID)
	{
		texture = Game::assetManager->GetTexture(assetTextureID);

		sourceRectangle.x = sourceRectX;
		sourceRectangle.y = sourceRectY;
		sourceRectangle.h = tileSize;
		sourceRectangle.w = tileSize;

		destinationRectangle.x = x;
		destinationRectangle.y = y;
		destinationRectangle.w = tileScale * tileSize;
		destinationRectangle.h = tileScale * tileSize;

		position.x = x;
		position.y = y;
	}

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void Update(float deltaTime) override
	{
		//TODO: here we will take care of tile positions based on camera control that will keep moving
	}

	void Render() override
	{
		TextureManager::Draw(texture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
	}
};
#endif // !TILECOMPONENT_H
