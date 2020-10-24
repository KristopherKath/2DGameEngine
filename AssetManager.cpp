#include "AssetManager.h"

//Constructor: Needs the EntityManager
AssetManager::AssetManager(EntityManager* Manager) : manager(Manager) {}

AssetManager::~AssetManager() {}

//Clears all the data in the map
void AssetManager::ClearData()
{
	textures.clear();
}

//Adds a Texture into the map given the TextureID and its file path
void AssetManager::AddTexture(std::string TextureID, const char* FilePath)
{
	textures.emplace(TextureID, TextureManager::LoadTexture(FilePath));
}

//Returns the Texture for the given TextureID
SDL_Texture* AssetManager::GetTexture(std::string TextureID)
{
	return textures[TextureID];
}
