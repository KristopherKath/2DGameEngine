#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Game.h"

class
	TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* FileName);
	static void Draw(SDL_Texture* Texture, SDL_Rect SourceRectangle, SDL_Rect DestinationRectangle, SDL_RendererFlip Flip);
};

#endif