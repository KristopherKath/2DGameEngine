#include <iostream>
#include "Constants.h"
#include "Game.h"

//Constructor
Game::Game()
{
	this->bIsRunning = false;
}

//Checks if game is running
//Returns boolean of bIsRunning
bool Game::IsRunning() const
{
	return this->bIsRunning;
}

float ProjectilePosX = 0.0f;
float ProjectilePosY = 0.0f;
float ProjectileVelX = 20.0f;
float ProjectileVelY = 30.0f;

//Initializes SDL window and renderer 
//Sets game as running if no errors
void Game::Initialize(int width, int height)
{
	//Checks if SLD works
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Error initializing SDL." << std::endl;
		return;
	}

	//Create the window
	Window = SDL_CreateWindow(
		NULL,					//name
		SDL_WINDOWPOS_CENTERED, //x-pos
		SDL_WINDOWPOS_CENTERED, //y-pos
		width,
		height,
		SDL_WINDOW_BORDERLESS); //type of window
	//Check if window was created
	if (!Window)
	{
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}

	//Create the Renderer
	Renderer = SDL_CreateRenderer(
		Window,					//The window to render
		-1,						//Which driver to render with (-1 is default)
		0);
	//Check if renderer was created
	if (!Renderer)
	{
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}

	//Game is now running
	bIsRunning = true;
	return;
}

//Processes User Input
void Game::ProcessInput()
{
	SDL_Event Event;
	SDL_PollEvent(&Event);
	switch (Event.type)
	{
		//Closes the game when the window is closed
		case SDL_QUIT:
		{
			bIsRunning = false;
			break;
		}
		//Closes the game when key ESC is pressed
		case SDL_KEYDOWN: 
		{
			if (Event.key.keysym.sym == SDLK_ESCAPE)
			{
				bIsRunning = false;
			}
		}
		default:
		{
			break;
		}
	}
}

//Update the game objects
void Game::Update()
{
	//Wait until 16.6 ms has elapsed since the last frame
	//In case computations happen before 16.6 ms we need to waste time till we get to 16.6 ms
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksLastFrame + FRAME_TARGET_TIME));

	//Delta time is the difference in ticks from last frame converted to seconds
	float DeltaTime = (SDL_GetTicks() - TicksLastFrame) / 1000.0f;

	//Clamp DeltaTime to a maximum value
	DeltaTime = (DeltaTime > 0.05f) ? 0.05f : DeltaTime;

	//Sets the new ticks for the current frame to be used in the next pass
	TicksLastFrame = SDL_GetTicks(); //Update the ticks to this time

	ProjectilePosX += ProjectileVelX * DeltaTime;
	ProjectilePosY += ProjectileVelY * DeltaTime;
}

//Render Window
//SDL has a bakc buffer screen that is updated
//The buffer swaps with the actual screen to reduce artifacts
void Game::Render()
{
	SDL_SetRenderDrawColor(Renderer, 21, 21, 21, 255); //Draws the window as grey
	SDL_RenderClear(Renderer); //Clears back buffer

	//Projectile Struct
	SDL_Rect Projectile
	{
		(int) ProjectilePosX,
		(int) ProjectilePosY,
		10, //width
		20  //height
	};

	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255); //Draws white for the projectile
	SDL_RenderFillRect(Renderer, &Projectile); //Renders the Projectile

	SDL_RenderPresent(Renderer); //Swaps the rendered screen for visualization
}

void Game::Destroy()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}