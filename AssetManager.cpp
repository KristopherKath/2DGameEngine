#include "AssetManager.h"

//Constructor: Needs the EntityManager
AssetManager::AssetManager(EntityManager* Manager) : Manager(Manager) {}

//Clears all the data in the map
void AssetManager::ClearData()
{
	Textures.clear();
}

//Adds a Texture into the map given the TextureID and its fila path
void AssetManager::AddTexture(std::string TextureID, const char* FilePath)
{
	Textures.emplace(TextureID, TextureManager::LoadTexture(FilePath));
}

//Returns the Texture for the given TextureID
SDL_Texture* AssetManager::GetTexture(std::string TextureID)
{
	return Textures[TextureID];
}
