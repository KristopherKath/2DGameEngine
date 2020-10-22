#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "/GameEngine/libglm/lib/glm/glm.hpp"

//Constructor
Game::Game()
{
	this->bIsRunning = false;
}

//Deconstructor
Game::~Game() {}

//Checks if game is running
//Returns boolean of bIsRunning
bool Game::IsRunning() const { return this->bIsRunning; }

//Projectile position and velocity
glm::vec2 ProjectilePos = glm::vec2(0.0f, 0.0f);
glm::vec2 ProjectileVel = glm::vec2(20.0f, 20.0f);


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
	//***Not efficient since it forces 100% of CPU to focus on this while loop. Nothing else can work with the CPU while in this.***
	//Took about 15% of CPU 
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksLastFrame + FRAME_TARGET_TIME));

	//Wait until 16.6 ms has elapsed since the last frame
	//In case computations happen before 16.6 ms we need to waste time till we get to 16.6 ms
	//***Much more efficient than previous line. CPU can now do other processes in this time***
	//Takes 0-0.1% of CPU
	int TimeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - TicksLastFrame);
	if (TimeToWait > 0 && TimeToWait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(TimeToWait);
	}

	//Delta time is the difference in ticks from last frame converted to seconds
	float DeltaTime = (SDL_GetTicks() - TicksLastFrame) / 1000.0f;

	//Clamp DeltaTime to a maximum value
	DeltaTime = (DeltaTime > 0.05f) ? 0.05f : DeltaTime;

	//Sets the new ticks for the current frame to be used in the next pass
	TicksLastFrame = SDL_GetTicks(); //Update the ticks to this time

	//Update game object with delta time
	ProjectilePos = glm::vec2(
		ProjectilePos.x + ProjectileVel.x * DeltaTime,
		ProjectilePos.y + ProjectileVel.y * DeltaTime);
}

//Render Window
//SDL has a bakc buffer screen that is updated
//The buffer swaps with the actual screen to reduce artifacts
void Game::Render()
{
	//Sets background color
	SDL_SetRenderDrawColor(Renderer, 21, 21, 21, 255); //Draws the window as grey

	//Clears back buffer
	SDL_RenderClear(Renderer); 

	//Projectile Struct
	SDL_Rect Projectile
	{
		(int) ProjectilePos.x,
		(int) ProjectilePos.y,
		10, //width
		10  //height
	};

	//Draws game object of the scene
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255); //Draws white for the projectile
	SDL_RenderFillRect(Renderer, &Projectile); //Renders the Projectile

	//Swap front and back buffers
	SDL_RenderPresent(Renderer); //Swaps the rendered screen for visualization
}

void Game::Destroy()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}