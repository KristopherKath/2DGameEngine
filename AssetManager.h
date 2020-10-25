#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H
#pragma once

#include <map>
#include <string>
#include <SDL_ttf.h>
#include "TextureManager.h"
#include "FontManager.h"
#include "EntityManager.h"

//Responsible for loading textures from Disk
class AssetManager
{
private:
	EntityManager* manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
public:
	AssetManager(EntityManager* Manager);
	~AssetManager();
	void ClearData();
	void AddTexture(std::string TextureID, const char* FilePath);
	void AddFont(std::string fontID, const char* filePath, int fontSize);
	SDL_Texture* GetTexture(std::string TextureID);
	TTF_Font* GetFont(std::string fontID);
};


#endif