#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Entity.h"
#include "Component.h"
#include "EntityManager.h"

class Game
{
private:
	bool bIsRunning;
	SDL_Window* Window;

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

public:
	int TicksLastFrame;
	static SDL_Renderer* Renderer;
};


#endif // !GAME_H

