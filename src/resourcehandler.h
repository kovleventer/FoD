#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <iterator>
#include <vector>
#include <iostream>

#include "text.h"

class ResourceHandler {
public:
	ResourceHandler();
	~ResourceHandler();
	void loadAll();
	
	//Every texture is loaded once
	//NOTE private variables with getters would be safer to use
	std::map<std::string, SDL_Texture*> tileTextures;
	std::map<int, std::string> tileTextureIDs;
	
	std::map<std::string, SDL_Texture*> worldObjectTextures;
	std::map<int, std::string> worldObjectTextureIDs;
	
	std::map<std::string, SDL_Texture*> interactiveWorldObjectTextures;
	std::map<int, std::string> interactiveWorldObjectTextureIDs;
	
	std::map<std::string, SDL_Texture*> cursorTextures;
	
	//NOTE we might need more textures for animations
	SDL_Texture* playerTexture;
	
	std::map<std::string, SDL_Texture*> pathTextures;
	
	std::map<std::string, SDL_Texture*> npcTextures;
	std::map<int, std::string> npcTextureIDs;
	
	std::map<std::string, SDL_Texture*> guiTextures;
	
	std::map<std::string, SDL_Texture*> itemTextures;
	
	std::map<std::string, SDL_Texture*> itemRarityIndicatorTextures;
	
	std::map<std::string, SDL_Texture*> unitTextures;
	
	
	
	SDL_Texture* getTextTexture(std::string text, SDL_Color color);
	SDL_Texture* getTextTexture(Text text);
	
	
	//Stores the colors that can be later referenced
	std::map<std::string, SDL_Color> colors;
private:
	void loadImages();
	void loadTerrainImages();
	void loadWorldObjectImages();
	void loadInteractiveWorldObjectImages();
	void loadCursorImages();
	void loadPlayerImages();
	void loadPathImages();
	void loadNPCImages();
	void loadGUIImages();
	void loadItemImages();
	void loadItemRarityIndicatorImages();
	void loadUnitImages();
	
	SDL_Texture* loadTexture(std::string path);
	
	
	void loadFont();
	TTF_Font* font;
	
	std::map<Text, SDL_Texture*> renderedTexts;
	
	
	void loadColors();
	
	std::string terrainImagePath;
	std::string worldObjectImagePath;
	std::string interactiveWorldObjectImagePath;
	std::string cursorImagePath;
	std::string playerImagePath;
	std::string pathImagePath;
	std::string npcImagePath;
	std::string guiImagePath;
	std::string itemImagePath;
	std::string itemRarityIndicatorImagePath;
	std::string unitImagePath;
	
	std::string fontPath;
};
