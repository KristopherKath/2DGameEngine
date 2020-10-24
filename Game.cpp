#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "AssetManager.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "/GameEngine/libglm/lib/glm/glm.hpp"
#include "Map.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
Map* map;


//Constructor
Game::Game()
{
	this->isRunning = false;
}

//Deconstructor
Game::~Game() {}

//Checks if game is running
//Returns boolean of bIsRunning
bool Game::IsRunning() const { return this->isRunning; }


//player entity - entity components added in LoadLevel 
Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

//Loads info from level
void Game::LoadLevel(int LevelNumber)
{
	/* Start including new assets to the assetmanager list */
	//Add assets to assetmanager
	assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
	assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
	assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
	assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());

	//Add tilemap
	map = new Map("jungle-tiletexture", 2, 32);
	map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

	/* Start including entities and also components to them */
	//Add player components
	player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
	player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
	player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");
	player.AddComponent<ColliderComponent>("player", 240, 106, 32, 32);

	//Add enemy and their components
	Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
	tankEntity.AddComponent<TransformComponent>(150, 495, 5, 0, 32, 32, 1);
	tankEntity.AddComponent<SpriteComponent>("tank-image");
	tankEntity.AddComponent<ColliderComponent>("enemy", 150, 495, 32, 32);

	//Add radar UI and their components
	Entity& radarEntity(manager.AddEntity("Radar", UI_LAYER));
	radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
	radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

}

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
	window = SDL_CreateWindow(
		NULL,					//name
		SDL_WINDOWPOS_CENTERED, //x-pos
		SDL_WINDOWPOS_CENTERED, //y-pos
		width,
		height,
		SDL_WINDOW_BORDERLESS); //type of window
	//Check if window was created
	if (!window)
	{
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}

	//Create the Renderer
	renderer = SDL_CreateRenderer(
		window,					//The window to render
		-1,						//Which driver to render with (-1 is default)
		0);
	//Check if renderer was created
	if (!renderer)
	{
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}

	//Load the level to run
	LoadLevel(0);

	//Game is now running
	isRunning = true;
	return;
}

//Processes User Input
void Game::ProcessInput()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
		//Closes the game when the window is closed
		case SDL_QUIT:
		{
			isRunning = false;
			break;
		}
		//Closes the game when key ESC is pressed
		case SDL_KEYDOWN: 
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
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
	int TimeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
	if (TimeToWait > 0 && TimeToWait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(TimeToWait);
	}

	//Delta time is the difference in ticks from last frame converted to seconds
	float DeltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	//Clamp DeltaTime to a maximum value
	DeltaTime = (DeltaTime > 0.05f) ? 0.05f : DeltaTime;

	//Sets the new ticks for the current frame to be used in the next pass
	ticksLastFrame = SDL_GetTicks(); //Update the ticks to this time

	//Updates all entities
	manager.Update(DeltaTime);
	
	//Update the camera movement
	HandleCameraMovement();
	
	//Check if there are any collisions with player in game
	CheckCollisions();
}

//Render Window
//SDL has a bakc buffer screen that is updated
//The buffer swaps with the actual screen to reduce artifacts
void Game::Render()
{
	//Sets background color
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255); //Draws the window as grey

	//Clears back buffer
	SDL_RenderClear(renderer); 

	//Dont render if there is nothing to render
	if (manager.HasNoEntities())
	{
		return;
	}

	//Render all entities
	manager.Render();

	//Swap front and back buffers
	SDL_RenderPresent(renderer); //Swaps the rendered screen for visualization
}

//Updates the camera movement on the player
void Game::HandleCameraMovement()
{
	TransformComponent* mainPlayerTransform = player.GetComponent<TransformComponent>();

	//only updates camera when player has reached half of the screen size
	camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
	camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

	//clamp the values of the camera if leaving boundaries of map
	camera.x = camera.x < 0 ? 0 : camera.x;
	camera.y = camera.y < 0 ? 0 : camera.y;
	camera.x = camera.x > camera.w ? camera.w : camera.x;
	camera.y = camera.y > camera.h ? camera.h : camera.y;
}

//Checks entities, that have collider componenet, if they are colliding
void Game::CheckCollisions()
{
	std::string collisionTagType = manager.CheckEntityCollisions(player);
	if (collisionTagType.compare("enemy") == 0)
	{
		//TODO: do something when collision is identified with an enemy
		isRunning = false;
	}
}

//Destroys window and renderer then quits
void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}