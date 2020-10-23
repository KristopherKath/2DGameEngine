#ifndef TRANSFORMCOMPONENT_H
#define TRANFORMCOMPONENT_H

#include "EntityManager.h"
#include "/GameEngine/libglm/lib/glm/glm.hpp"
#include <SDL.h>
#include "Game.h"

class TransformComponent : public Component
{
public:
	glm::vec2 Position;
	glm::vec2 Velocity;
	int Width;
	int Height;
	int Scale;

	//Sets the Position, Velocity, Height, Width and Scale of object
	TransformComponent(int PosX, int PosY, int VelX, int VelY, int W, int H, int S)
	{
		ComponentName = "TransformComponent";
		Position = glm::vec2(PosX, PosY);
		Velocity = glm::vec2(VelX, VelY);
		Width = W;
		Height = H;
		Scale = S;
	}

	void Initialize() override
	{

	}

	//Updates the transform position
	void Update(float DeltaTime) override
	{
		Position.x += Velocity.x * DeltaTime;
		Position.y += Velocity.y * DeltaTime;
	}

	void Render() override
	{
		SDL_Rect TransformRectangle = {
			(int)Position.x,
			(int)Position.y,
			Width,
			Height
		};
		SDL_SetRenderDrawColor(Game::Renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(Game::Renderer, &TransformRectangle);
	}
};

#endif
