#include "FontManager.h"

//Loads font from the given filename
TTF_Font* FontManager::LoadFont(const char* fileName, int fontSize)
{
    return TTF_OpenFont(fileName, fontSize);
}

//Draws the font on the given position
void FontManager::Draw(SDL_Texture* texture, SDL_Rect position)
{
    SDL_RenderCopy(Game::renderer, texture, NULL, &position);
}
