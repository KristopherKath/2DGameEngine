#include "TextureManager.h"

//Loads a Texture based on the filename passed
SDL_Texture* TextureManager::LoadTexture(const char* FileName)
{
    SDL_Surface* Surface = IMG_Load(FileName);
    SDL_Texture* Texture = SDL_CreateTextureFromSurface(Game::Renderer, Surface);
    SDL_FreeSurface(Surface);
    return Texture;
}

//Draws the given Texture
static void Draw(SDL_Texture* Texture, SDL_Rect SourceRectangle, SDL_Rect DestinationRectangle, SDL_RendererFlip Flip)
{
    SDL_RenderCopyEx(Game::Renderer, Texture, &SourceRectangle, &DestinationRectangle, 0.0, NULL, Flip);
}