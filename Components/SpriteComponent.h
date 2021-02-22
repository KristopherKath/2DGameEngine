#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H
#pragma once

#include <SDL.h>
#include "TextureManager.h"
#include "AssetManager.h"
#include "Animation.h"


class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	bool isAnimated;
	int numFrames;
	int animationSpeed;
	bool isFixed;
	std::map<std::string, Animation> animations;
	std::string currentAnimationName;
	unsigned int animationIndex = 0;

public:

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	SDL_Rect sourceRectangle;
	SDL_Rect destinationRectangle;

	bool GetIsAnimated() { return isAnimated; }
	bool GetIsFixed() { return isFixed; }
	unsigned int GetAnimationIndex() { return animationIndex; }
	int GetNumFrames() { return numFrames; }
	int GetAnimationSpeed() { return animationSpeed; }
	SDL_Rect GetSourceRect() { return sourceRectangle; }
	SDL_Rect GetDestinationRect() { return destinationRectangle; }
	TransformComponent* GetTransformComp() { return transform; }


	//Constructor for single image
	SpriteComponent(std::string AssetTextureID)
	{
		isAnimated = false;
		isFixed = false;
		SetTexture(AssetTextureID);
	}

	//Constructor for animated textures
	SpriteComponent(std::string AssetTextureID, int numFrames, int animationSpeed, bool hasDirections, bool isFixed)
	{
		isAnimated = true;
		this->numFrames = numFrames;
		this->animationSpeed = animationSpeed;
		this->isFixed = isFixed;

		//for multiple animations on spritesheet (4 rows of animations currently
		if (hasDirections)
		{
			//Create animations based on index (row), number of frames to run, and animation speed
			Animation downAnimation = Animation(0, numFrames, animationSpeed);
			Animation rightAnimaiton = Animation(1, numFrames, animationSpeed);
			Animation leftAnimation = Animation(2, numFrames, animationSpeed);
			Animation upAnimaiton = Animation(3, numFrames, animationSpeed);

			//Add animaions into animations map
			animations.emplace("DownAnimation", downAnimation);
			animations.emplace("RightAnimation", rightAnimaiton);
			animations.emplace("LeftAnimation", leftAnimation);
			animations.emplace("UpAnimation", upAnimaiton);

			//Set animation index and name to first animation to run
			this->animationIndex = 0;
			this->currentAnimationName = "DownAnimation";
		}
		//For one animation
		else
		{
			//Add single animation to run and set up for play
			Animation singleAnimation = Animation(0, numFrames, animationSpeed);
			animations.emplace("SingleAnimation", singleAnimation);
			this->animationIndex = 0;
			this->currentAnimationName = "SingleAnimation";
		}

		//Play the animation and set its texture
		Play(this->currentAnimationName);
		SetTexture(AssetTextureID);
	}

	//Plays the given animation
	void Play(std::string animationName)
	{
		numFrames = animations[animationName].numFrames;
		animationIndex = animations[animationName].index;
		animationSpeed = animations[animationName].animationSpeed;
		currentAnimationName = animationName;
	}

	//Sets the sprite texture for the given textureID
	void SetTexture(std::string AssetTextureID)
	{
		texture = Game::assetManager->GetTexture(AssetTextureID);
	}

	//Initializes the sprite component
	void Initialize() override
	{
		transform = owner->GetComponent<TransformComponent>();
		sourceRectangle.x = 0;
		sourceRectangle.y = 0;
		sourceRectangle.w = transform->width;
		sourceRectangle.h = transform->height;
	}

	//Update the sprite component position and if animated
	void Update(float DeltaTime) override
	{
		
		/* ****   THIS HAS BEEN MOVED INTO SPRITESYSTEM   ***
		* 
		//animate the texture
		if (isAnimated)
		{
			//changes the frame in the animation (column of spritesheet)
			sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames); 
		}
		sourceRectangle.y = animationIndex * transform->height; //sets the row of the spritesheet

		//updates the sprite to camera screen space if not fixed
		destinationRectangle.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : Game::camera.x);
		destinationRectangle.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : Game::camera.y);
		destinationRectangle.w = transform->width * transform->scale;
		destinationRectangle.h = transform->height * transform->scale;
		*/
	}

	//Renders the texture
	void Render() override
	{
		TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
	}
};

#endif // !SPRITECOMPONENT_H
