#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H
#pragma once

#include "Game.h"
#include "EntityManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <string>

class KeyboardControlComponent : public Component
{
public:
	std::string upKey;
	std::string downKey;
	std::string rightKey; 
	std::string leftKey;
	std::string shootKey;
	TransformComponent* transform;
	SpriteComponent* sprite;

	KeyboardControlComponent() {}

	//Constructor: set key codes 
	KeyboardControlComponent(std::string upKey, std::string rightKey, std::string downKey, std::string leftKey, std::string shootKey)
	{
		this->upKey = GetSDLKeyStringCode(upKey);
		this->rightKey = GetSDLKeyStringCode(rightKey);
		this->downKey = GetSDLKeyStringCode(downKey);
		this->leftKey = GetSDLKeyStringCode(leftKey);
		this->shootKey = GetSDLKeyStringCode(shootKey);
	}

	//Gets the SDL key code as string
	std::string GetSDLKeyStringCode(std::string key)
	{
		if (key.compare("up") == 0) return "1073741906";
		if (key.compare("down") == 0) return "1073741905";
		if (key.compare("left") == 0) return "1073741904";
		if (key.compare("right") == 0) return "1073741903";
		if (key.compare("space") == 0) return "32";
		return std::to_string(static_cast<int>(key[0]));
	}

	//Initialize the keyboard component by setting sprite and transform
	void Initialize() override
	{
		transform = owner->GetComponent<TransformComponent>();
		sprite = owner->GetComponent<SpriteComponent>();
	}

	//Updates based on keyboard input
	void Update(float deltaTime) override
	{
		/*		THIS WAS MOVED INTO KEYBOARD CONTROL SYSTEM
		//When key pressed update the speed/direction of entity and animation
		if (Game::event.type == SDL_KEYDOWN)
		{
			std::string keyCode = std::to_string(Game::event.key.keysym.sym);
			if (keyCode.compare(upKey) == 0)
			{
				transform->velocity.y = -100;
				transform->velocity.x = 0;
				sprite->Play("UpAnimation");
			}
			if (keyCode.compare(rightKey) == 0)
			{
				transform->velocity.y = 0;
				transform->velocity.x = 100;
				sprite->Play("RightAnimation");
			}
			if (keyCode.compare(downKey) == 0)
			{
				transform->velocity.y = 100;
				transform->velocity.x = 0;
				sprite->Play("DownAnimation");
			}
			if (keyCode.compare(leftKey) == 0)
			{
				transform->velocity.y = 0;
				transform->velocity.x = -100;
				sprite->Play("LeftAnimation");
			}
		}

		//When key released update the speed/direction of entity
		if (Game::event.type == SDL_KEYUP)
		{
			std::string keyCode = std::to_string(Game::event.key.keysym.sym);
			if (keyCode.compare(upKey) == 0)
			{
				transform->velocity.y = 0;
			}
			if (keyCode.compare(rightKey) == 0)
			{
				transform->velocity.x = 0;
			}
			if (keyCode.compare(downKey) == 0)
			{
				transform->velocity.y = 0;
			}
			if (keyCode.compare(leftKey) == 0)
			{
				transform->velocity.x = 0;
			}
			if (keyCode.compare(shootKey) == 0)
			{
				// TODO
			}
		}
		*/
	}
};


#endif // !KEYBOARDCONTROLCOMPONENT_H
