#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL.h>
#include "TextureManager.h"
#include "AssetManager.h"
#include "Animation.h"


class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect sourceRectangle;
	SDL_Rect destinationRectangle;
	bool isAnimated;
	int numFrames;
	int animationSpeed;
	bool isFixed;
	std::map<std::string, Animation> animations;
	std::string currentAnimationName;
	unsigned int animationIndex = 0;

public:
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

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
			Animation downAnimation = Animation(0, numFrames, animationSpeed);
			Animation rightAnimaiton = Animation(1, numFrames, animationSpeed);
			Animation leftAnimation = Animation(2, numFrames, animationSpeed);
			Animation upAnimaiton = Animation(3, numFrames, animationSpeed);

			animations.emplace("DownAnimation", downAnimation);
			animations.emplace("RightAnimaiton", rightAnimaiton);
			animations.emplace("LeftAnimation", leftAnimation);
			animations.emplace("UpAnimaiton", upAnimaiton);

			this->animationIndex = 0;
			this->currentAnimationName = "DownAnimation";
		}
		//For one animation
		else
		{
			Animation singleAnimation = Animation(0, numFrames, animationSpeed);
			animations.emplace("SingleAnimation", singleAnimation);
			this->animationIndex = 0;
			this->currentAnimationName = "SingleAnimation";
		}

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

	//sets the texture for the given textureID
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

	//Update the sprite component
	void Update(float DeltaTime) override
	{
		//animate the texture
		if (isAnimated)
		{
			sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames); //changes the frame in the animation (column of spritesheet)
		}
		sourceRectangle.y = animationIndex * transform->height; //sets the row of the spritesheet

		destinationRectangle.x = static_cast<int>(transform->position.x);
		destinationRectangle.y = static_cast<int>(transform->position.y);
		destinationRectangle.w = transform->width * transform->scale;
		destinationRectangle.h = transform->height * transform->scale;
	}

	//Renders the texture
	void Render() override
	{
		TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
	}
};

#endif // !SPRITECOMPONENT_H
