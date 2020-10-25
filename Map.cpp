#include "Map.h"
#include "Game.h"
#include "EntityManager.h"
#include "Components\TileComponent.h"
#include <fstream>

extern EntityManager entityManager;

//Constructor given tilemap textureID, scale to display it as, and tile size
Map::Map(std::string textureID, int scale, int tileSize) : textureID(textureID), scale(scale), tileSize(tileSize) {}

//Loads the map information to display in game
void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY)
{
	//open the map file
	std::fstream mapFile;
	mapFile.open(filePath);

	//for the rows and columns in map file add the tile based on code
	for (int y = 0; y < mapSizeY; y++)
	{
		for (int x = 0; x < mapSizeX; x++)
		{
			char ch;
			mapFile.get(ch);
			int sourceRectY = atoi(&ch) * tileSize;
			mapFile.get(ch);
			int sourceRectX = atoi(&ch) * tileSize;

			//add tile to tilemap
			AddTile(sourceRectX, sourceRectY, x * (scale * tileSize), y * (scale * tileSize));
			mapFile.ignore();

		}
	}
	mapFile.close();
}

//Adds the given texture to tile map
void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y)
{
	Entity& newTile(entityManager.AddEntity("Tile", TILEMAP_LAYER));
	newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize, scale, textureID);
}
