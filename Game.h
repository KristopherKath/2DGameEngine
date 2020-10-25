#ifndef GAME_H
#define GAME_H
#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Entity.h"
#include "Component.h"
#include "EntityManager.h"

class AssetManager;

class Game
{
private:
	bool isRunning;
	SDL_Window* window;

public:
	Game();
	~Game();
	bool IsRunning() const; //getter for bIsRunning
	void Initialize(int width, int height);
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();
	void LoadLevel(int LevelNumber);
	void HandleCameraMovement();
	void CheckCollisions();
	void ProcessNextLevel(int levelNumber);
	void ProcessGameOver();

public:
	int ticksLastFrame = 0;
	static SDL_Rect camera;
	static SDL_Renderer* renderer;
	static AssetManager* assetManager;
	static SDL_Event event;
	static bool isDebug;
};


#endif // !GAME_H
