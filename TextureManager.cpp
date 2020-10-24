#include "TextureManager.h"

//Loads a Texture based on the filename passed
SDL_Texture* TextureManager::LoadTexture(const char* FileName)
{
    SDL_Surface* surface = IMG_Load(FileName);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

//Draws the given Texture
void TextureManager::Draw(SDL_Texture* Texture, SDL_Rect SourceRectangle, SDL_Rect DestinationRectangle, SDL_RendererFlip Flip)
{
    SDL_RenderCopyEx(Game::renderer, Texture, &SourceRectangle, &DestinationRectangle, 0.0, NULL, Flip);
}
