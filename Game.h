#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Game
{
private:
	bool bIsRunning;
	SDL_Window* Window;
	SDL_Renderer* Renderer;

public:
	Game();
	~Game();
	bool IsRunning() const; //getter for bIsRunning
	void Initialize(int width, int height);
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

public:
	int TicksLastFrame;

};


#endif // !GAME_H

