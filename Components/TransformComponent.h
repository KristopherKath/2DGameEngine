#ifndef TRANSFORMCOMPONENT_H
#define TRANFORMCOMPONENT_H
#pragma once

#include "EntityManager.h"
#include "/GameEngine/libglm/lib/glm/glm.hpp"
#include <SDL.h>
#include "Game.h"
#include <algorithm>

class TransformComponent : public Component
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
	int width;
	int height;
	int scale;

	//Sets the Position, Velocity, Height, Width and Scale of object
	TransformComponent(int PosX, int PosY, int VelX, int VelY, int W, int H, int S)
	{
		componentName = "TransformComponent";
		position = glm::vec2(PosX, PosY);
		velocity = glm::vec2(VelX, VelY);
		width = W;
		height = H;
		scale = S;
	}

	void Initialize() override {}

	//Updates the transform position
	void Update(float DeltaTime) override
	{
		//Change the objects position based on velocity
		position.x += velocity.x * DeltaTime;
		position.y += velocity.y * DeltaTime;

		/*
		//Clamp the values if they are out of bounds of screen
		if (!InBoundaryCheckX(position.x))
		{
			position.x = Clamp(position.x, 0, (int)WINDOW_WIDTH - 32);
		}
		if (!InBoundaryCheckY(position.y))
		{
			position.y = Clamp(position.y, 0, (int)WINDOW_HEIGHT - 32);
		}
		*/
	}

	//Clamps the value to within the min and max value
	//Useful if wanting to keep object within screen boundaries
	int Clamp(int val, int min, int max)
	{
		if (val > max)
			val = max;
		if (val < min)
			val = min;
		return val;
	}

	//Checks to see if value is within boundaries of the screen
	bool InBoundaryCheckX(int val) const
	{
		if (val > WINDOW_WIDTH || val < 0)
			return false;

		return true;
	}
	//Checks to see if value is within boundaries of the screen
	bool InBoundaryCheckY(int val) const
	{
		if (val < 0 || val > WINDOW_HEIGHT)
			return false;

		return true;
	}

	void Render() override {}
};

#endif
