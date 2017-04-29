#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>
#include <iterator>
#include <vector>
#include <iostream>

#include "text.h"
#include "filesystemhandler.h"

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
	
	std::map<std::string, SDL_Texture*> cursorTextures;
	
	std::map<std::string, SDL_Texture*> pathTextures;
	
	std::map<std::string, SDL_Texture*> npcTextures;
	
	std::map<std::string, SDL_Texture*> guiTextures;
	
	std::map<std::string, SDL_Texture*> itemTextures;
	
	std::map<std::string, SDL_Texture*> itemRarityIndicatorTextures;
	
	std::map<std::string, SDL_Texture*> unitTextures;
	
	
	SDL_Texture* getTextTexture(std::string text, SDL_Color color);
	SDL_Texture* getTextTexture(Text text);
	//Clears all rendered text textures thus clearing memory
	void clearTextTextures();
	
	
	//Stores the colors that can be later referenced
	std::map<std::string, SDL_Color> colors;
	
	
	//NOTE might need better sound and music types
	std::map<std::string, Mix_Chunk*> sounds;
private:
	void loadImages();
	void loadTerrainImages();
	void loadWorldObjectImages();
	void loadInteractiveWorldObjectImages();
	void loadCursorImages();
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
	
	
	//Loads the given audio files
	void loadAudio();
	//This method does the exact Mix_LoadWAV
	Mix_Chunk* loadSound(std::string path);
	
	std::string terrainImagePath;
	std::string worldObjectImagePath;
	std::string cursorImagePath;
	std::string pathImagePath;
	std::string npcImagePath;
	std::string guiImagePath;
	std::string itemImagePath;
	std::string itemRarityIndicatorImagePath;
	std::string unitImagePath;
	
	std::string fontPath;
	
	std::string audioPath;
};
