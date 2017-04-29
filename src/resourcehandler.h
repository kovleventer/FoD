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
#include <utility>

#include "util/text.h"
#include "filesystemhandler.h"
#include "animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @enum TT Stands for texturetype
 * Used on texture-getting
 */
enum class TT {
	TILE,
	WORLDOBJECT,
	CURSOR,
	PATH,
	NPC,
	GUI,
	ITEM,
	ITRAIN, //Stands for Item Rarity Indicator
	UNIT
};

class ResourceHandler {
public:
	ResourceHandler();
	~ResourceHandler();
	
	void loadAll();
	
	ATexture* getATexture(TT tType, std::string name);
	
	ATexture* getTextTexture(std::string text, SDL_Color color);
	ATexture* getTextTexture(Text text);
	//Clears all rendered text textures thus clearing memory
	void clearTextTextures();
	
	
	//Stores the colors that can be later referenced
	std::map<std::string, SDL_Color> colors;
	
	
	//NOTE might need better sound and music types
	std::map<std::string, Mix_Chunk*> sounds;
private:
	//***IMAGES***
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
	
	
	//Every texture is loaded once
	std::map<std::string, ATexture*> tileTextures;
	std::map<std::string, ATexture*> worldObjectTextures;
	std::map<std::string, ATexture*> cursorTextures;
	std::map<std::string, ATexture*> pathTextures;
	std::map<std::string, ATexture*> npcTextures;
	std::map<std::string, ATexture*> guiTextures;
	std::map<std::string, ATexture*> itemTextures;
	std::map<std::string, ATexture*> itemRarityIndicatorTextures;
	std::map<std::string, ATexture*> unitTextures;
	
	
	
	//***TEXTS***
	void loadFont();
	TTF_Font* font;
	
	std::map<Text, ATexture*> renderedTexts;
	
	
	//***COLORS***
	void loadColors();
	
	
	//***AUDIO***
	//Loads the given audio files
	void loadAudio();
	//This method does the exact Mix_LoadWAV
	Mix_Chunk* loadSound(std::string path);
	
	
	
	//***PATHS***
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
