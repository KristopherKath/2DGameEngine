#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "AssetManager.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/KeyboardControlComponent.h"
#include "Components/TextLabelComponent.h"
#include "../../libglm/lib/glm/glm.hpp"


EntityManager entityManager;
AssetManager* Game::assetManager = new AssetManager(&entityManager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
bool Game::isDebug;
SDL_Rect Game::camera = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
Map* map;
//player entity - entity components added in LoadLevel 
Entity* mainPlayer = NULL;

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


//Loads info from level
void Game::LoadLevel(int LevelNumber)
{

	//Load lua and libraries
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

	//Open lua file
	std::string levelName = "Level" + std::to_string(LevelNumber);
	lua.script_file("./assets/scripts/" + levelName + ".lua");

	/****************************************************/
	/*       LOADS ASSETS FROM LUA CONFIG FILE          */
	/****************************************************/

	sol::table levelData = lua[levelName];
	sol::table levelAssets = levelData["assets"];

	//Load assets in file
	unsigned int assetIndex = 0;
	while (true)
	{
		//Check if asset exists
		sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
		if (existsAssetIndexNode == sol::nullopt) { break; }
		else
		{
			//Get asset at index
			sol::table asset = levelAssets[assetIndex];
			//Get asset type
			std::string assetType = asset["type"];
			//If asset is of texture type
			if (assetType.compare("texture") == 0)
			{
				//Load asset id and filepath to assetmanager
				std::string assetID = asset["id"];
				std::string assetFile = asset["file"];
				assetManager->AddTexture(assetID, assetFile.c_str());
			}
			//If asset is of font type
			else if (assetType.compare("font") == 0)
			{
				//Load asset id and filepath to assetmanager
				std::string assetID = asset["id"];
				std::string assetFile = asset["file"];
				int fontSize = asset["fontSize"];
				assetManager->AddFont(assetID, assetFile.c_str(), fontSize);
			}
		}
		assetIndex++;
	}


	/****************************************************/
	/*       LOADS MAP FROM LUA CONFIG FILE             */
	/****************************************************/

	sol::table levelMap = levelData["map"];
	std::string mapTextureID = levelMap["textureAssetID"];
	std::string mapFile = levelMap["file"];

	map = new Map(
		mapTextureID,
		static_cast<int>(levelMap["scale"]),
		static_cast<int>(levelMap["tileSize"])
	);

	map->LoadMap(
		mapFile,
		static_cast<int>(levelMap["mapSizeX"]),
		static_cast<int>(levelMap["mapSizeY"])
	);



	/**************************************************************/
	/*     LOADS ENTITIES & COMPONENTS FROM LUA CONFIG FILE       */
	/**************************************************************/


	
	sol::table levelEntities = levelData["entities"];

	//Load entities in file
	unsigned int entityIndex = 0;
	while (true)
	{
		//Check if entity exists
		sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
		if (existsEntityIndexNode == sol::nullopt) { break; }
		else
		{
			//Get entity at index
			sol::table entity = levelEntities[entityIndex];

			//Get entity information
			std::string entityName = entity["name"];
			LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));

			//Add entity to entity manager
			auto& newEntity(entityManager.AddEntity(entityName, entityLayerType));

			//Add Entity Components//

			//Add Transform Component
			sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
			if (existsTransformComponent != sol::nullopt)
			{
				newEntity.AddComponent<TransformComponent>(
					static_cast<int>(entity["components"]["transform"]["position"]["x"]),
					static_cast<int>(entity["components"]["transform"]["position"]["y"]),
					static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
					static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
					static_cast<int>(entity["components"]["transform"]["width"]),
					static_cast<int>(entity["components"]["transform"]["height"]),
					static_cast<int>(entity["components"]["transform"]["scale"])
					);
			}

			//Add Sprite Component
			sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
			if (existsSpriteComponent != sol::nullopt)
			{
				std::string textureID = entity["components"]["sprite"]["textureAssetId"];
				bool isAnimated = entity["components"]["sprite"]["animated"];

				//If sprite is animated
				if (isAnimated)
				{
					newEntity.AddComponent<SpriteComponent>(
						textureID,
						static_cast<int>(entity["components"]["sprite"]["frameCount"]),
						static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
						static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
						static_cast<bool>(entity["components"]["sprite"]["fixed"])
						);
				}
				else
				{
					newEntity.AddComponent<SpriteComponent>(textureID);
				}
			}

			//Add Input Control Component
			sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
			if (existsInputComponent != sol::nullopt)
			{
				//Check if input keys were added
				sol::optional<sol::table> existsKeyboardInputComponent = entity["components"]["input"]["keyboard"];
				if (existsKeyboardInputComponent != sol::nullopt)
				{
					std::string upKey = entity["components"]["input"]["keyboard"]["up"];
					std::string rightKey = entity["components"]["input"]["keyboard"]["right"];
					std::string downKey = entity["components"]["input"]["keyboard"]["down"];
					std::string leftKey = entity["components"]["input"]["keyboard"]["left"];
					std::string shootKey = entity["components"]["input"]["keyboard"]["shoot"];
					newEntity.AddComponent<KeyboardControlComponent>(upKey, rightKey, downKey, leftKey, shootKey);
				}
			}

			//Add Collider Component
			sol::optional<sol::table> existsColliderComponent = entity["components"]["input"];
			if (existsColliderComponent != sol::nullopt)
			{
				std::string colliderTag = entity["components"]["collider"]["tag"];
				newEntity.AddComponent<ColliderComponent>(
					colliderTag,
					static_cast<int>(entity["components"]["transform"]["position"]["x"]),
					static_cast<int>(entity["components"]["transform"]["position"]["y"]),
					static_cast<int>(entity["components"]["transform"]["width"]),
					static_cast<int>(entity["components"]["transform"]["height"]),
					"collision-image");
			}


			//Add Projectile Emitter Component
			sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
			if (existsProjectileEmitterComponent != sol::nullopt)
			{
				int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
				int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
				int parentEntityWidth = entity["components"]["transform"]["width"];
				int parentEntityHeight = entity["components"]["transform"]["height"];
				int projectileWidth = entity["components"]["projectileEmitter"]["width"];
				int projectileHeigth = entity["components"]["projectileEmitter"]["height"];
				int projectileSpeed = entity["components"]["projectileEmitter"]["speed"];
				int projectileRange = entity["components"]["projectileEmitter"]["range"];
				int projectileAngle = entity["components"]["projectileEmitter"]["angle"];
				bool projectileShouldLoop = entity["components"]["projectileEmitter"]["shouldLoop"];
				std::string textureAssetID = entity["components"]["projectileEmitter"]["textureAssetId"];

				Entity& projectile(entityManager.AddEntity("projectile", PROJECTILE_LAYER));
				projectile.AddComponent<TransformComponent>(
					parentEntityXPos + (parentEntityWidth / 2),
					parentEntityYPos + (parentEntityHeight / 2),
					0,
					0,
					projectileWidth,
					projectileHeigth,
					1
				);
				projectile.AddComponent<SpriteComponent>(textureAssetID);
				projectile.AddComponent<ProjectileEmitterComponent>(
					projectileSpeed,
					projectileAngle,
					projectileRange,
					projectileShouldLoop
				);
				projectile.AddComponent<ColliderComponent>(
					"PROJECTILE",
					parentEntityXPos,
					parentEntityYPos,
					projectileWidth,
					projectileHeigth,
					"collision-image"
				);
			}
		}
		entityIndex++;
	}
	
	/* Start including new assets to the assetmanager list */
	//Add assets to assetmanager

		/*
	assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
	assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
	assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
	assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
	assetManager->AddTexture("collision-image", std::string("./assets/images/collision-texture.png").c_str());
	assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
	assetManager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());
	assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);

	//Add tilemap
	map = new Map("jungle-tiletexture", 2, 32);
	map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);



	/* Start including entities and also components to them */
	//Add player components
		/*
	player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
	player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
	player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");
	player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32, "collision-image");

	//Add enemy and their components
	Entity& tankEntity(entityManager.AddEntity("tank", ENEMY_LAYER));
	tankEntity.AddComponent<TransformComponent>(150, 495, 5, 0, 32, 32, 1);
	tankEntity.AddComponent<SpriteComponent>("tank-image");
	tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32, "collision-image");

	//Creates a projectile and their components
	Entity& projectile(entityManager.AddEntity("projectile", PROJECTILE_LAYER));
	projectile.AddComponent<TransformComponent>(150+16, 495+16, 0, 0, 4, 4, 1);
	projectile.AddComponent<SpriteComponent>("projectile-image");
	projectile.AddComponent<ColliderComponent>("PROJECTILE", 150+16, 495+16, 4, 4, "collision-image");
	projectile.AddComponent<ProjectileEmitterComponent>(100, 270, 200, true);

	//Add level goal and their components
	Entity& heliport(entityManager.AddEntity("Heliport", OBSTACLE_LAYER));
	heliport.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
	heliport.AddComponent<SpriteComponent>("heliport-image");
	heliport.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32, "collision-image");

	//Add radar UI and their components
	Entity& radarEntity(entityManager.AddEntity("Radar", UI_LAYER));
	radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
	radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

	//Creates a UI Text display
	Entity& labelLevelName(entityManager.AddEntity("LabelLevelName", UI_LAYER));
	labelLevelName.AddComponent<TextLabelComponent>(10, 10, "First Level...", "charriot-font", WHITE_COLOR);
	*/
}

//Initializes SDL window and renderer 
//Sets game as running if no errors
void Game::Initialize(int width, int height)
{
	//Checks if SLD initializes
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Error initializing SDL" << std::endl;
		return;
	}
	//Checls if TTF initializes
	if (TTF_Init() != 0)
	{
		std::cerr << "Error initializing ADL TTF" << std::endl;
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
	LoadLevel(1);

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
		//Closes the game when key ESC is pressed & sets debug mode on TAB key pressed
		case SDL_KEYDOWN: 
		{
			//Close engine
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
			}
			//Sets debug mode
			if (event.key.keysym.sym == SDLK_TAB) 
			{
				isDebug = !(isDebug);
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
	entityManager.Update(DeltaTime);
	
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
	if (entityManager.HasNoEntities())
	{
		return;
	}

	//Render all entities
	entityManager.Render();

	//Swap front and back buffers
	SDL_RenderPresent(renderer); //Swaps the rendered screen for visualization
}

//Updates the camera movement on the player
void Game::HandleCameraMovement()
{

	if (mainPlayer)
	{
		TransformComponent* mainPlayerTransform = mainPlayer->GetComponent<TransformComponent>();
		//only updates camera when player has reached half of the screen size
		camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
		camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

		//clamp the values of the camera if leaving boundaries of map
		camera.x = camera.x < 0 ? 0 : camera.x;
		camera.y = camera.y < 0 ? 0 : camera.y;
		camera.x = camera.x > camera.w ? camera.w : camera.x;
		camera.y = camera.y > camera.h ? camera.h : camera.y;
	}
}

//Checks entities, that have collider componenet, if they are colliding
	//Changes based on type of collision
void Game::CheckCollisions()
{
	CollisionType collisionType = entityManager.CheckCollisions();
	if (collisionType == PLAYER_ENEMY_COLLISION)
	{
		ProcessGameOver();
	}
	if (collisionType == PLAYER_PROJECTILE_COLLISION)
	{
		ProcessGameOver();
	}
	if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION)
	{
		ProcessNextLevel(1);
	}

}

//Next level - Quits program as of now
void Game::ProcessNextLevel(int levelNumber)
{
	std::cout << "Next Level" << std::endl;
	isRunning = false;
}

//Game over - Quits program
void Game::ProcessGameOver()
{
	std::cout << "Game Over" << std::endl;
	isRunning = false;
}


//Destroys window and renderer then quits
void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}