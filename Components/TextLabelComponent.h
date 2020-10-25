#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H
#pragma once

#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Game.h"
#include "../FontManager.h"
#include <SDL.h>
#include <SDL_ttf.h>

class TextLabelComponent : public Component
{
private:
	SDL_Rect position;
	std::string text;
	std::string fontFamily;
	SDL_Color color;
	SDL_Texture* texture;

public:
	//Constructor: needs position, text and color to render
	TextLabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color& color)
	{
		this->position.x = x;
		this->position.y = y;
		this->text = text;
		this->fontFamily = fontFamily;
		this->color = color;
		SetLabelText(text, fontFamily);
	}

	//Creates the fonts onto a texture for rendering
	void SetLabelText(std::string text, std::string fontfamily)
	{
		SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->GetFont(fontFamily), text.c_str(), color);
		texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
		SDL_FreeSurface(surface);
		SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
	}

	//Renders the fonts based on created texture and position
	void Render() override
	{
		FontManager::Draw(texture, position);
	}
};
#endif // !TEXTLABELCOMPONENT_H
