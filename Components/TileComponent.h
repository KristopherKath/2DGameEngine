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

	//Constructor: given the sprite location of spritesheet, location to place on screen, size of sprite, scale to display it as, and the textureID
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

	//Deconstructor
	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	//Update the tile based on the position if camera component
	void Update(float deltaTime) override
	{
		/*   THIS HAS BEEN MOVED INTO TILE SYSTEM
		destinationRectangle.x = position.x - Game::camera.x;
		destinationRectangle.y = position.y - Game::camera.y;
		*/
	}

	//render the tile
	void Render() override
	{
		TextureManager::Draw(texture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
	}
};
#endif // !TILECOMPONENT_H
