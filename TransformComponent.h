#ifndef TRANSFORMCOMPONENT_H
#define TRANFORMCOMPONENT_H
#pragma once

#include "EntityManager.h"
#include "/GameEngine/libglm/lib/glm/glm.hpp"
#include <SDL.h>
#include "Game.h"

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
		position.x += velocity.x * DeltaTime;
		position.y += velocity.y * DeltaTime;
	}

	void Render() override {}
};

#endif
