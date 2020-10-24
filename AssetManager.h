#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include "TextureManager.h"
#include "EntityManager.h"

//Responsible for loading textures from Disk
class AssetManager
{
private:
	EntityManager* manager;
	std::map<std::string, SDL_Texture*> textures;

public:
	AssetManager(EntityManager* Manager);
	~AssetManager();
	void ClearData();
	void AddTexture(std::string TextureID, const char* FilePath);
	SDL_Texture* GetTexture(std::string TextureID);
};


#endif