#include "Animation.h"

//Constructor
Animation::Animation() {}

//Constructor given the index of the animation (row), how many frames to play, and speed of animation
Animation::Animation(unsigned int index, unsigned int numFrames, unsigned int animationSpeed) 
{
	this->index = index;
	this->numFrames = numFrames;
	this->animationSpeed = animationSpeed;
}

