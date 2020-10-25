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

//Adds a font into font map needing a fontID, the font file path, and wanted font size
void AssetManager::AddFont(std::string fontID, const char* filePath, int fontSize)
{
	fonts.emplace(fontID, FontManager::LoadFont(filePath, fontSize));
}

//Returns the Texture for the given TextureID
SDL_Texture* AssetManager::GetTexture(std::string TextureID)
{
	return textures[TextureID];
}

//Returns the wanted font using the specified fontID
TTF_Font* AssetManager::GetFont(std::string fontID)
{
	return fonts[fontID];
}
