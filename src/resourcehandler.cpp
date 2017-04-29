#include "resourcehandler.h"

#include "global.h"
#include "exceptions.h"

ResourceHandler::ResourceHandler() {
	terrainImagePath = "data/img/terrain/";
	worldObjectImagePath = "data/img/worldobject/";
	interactiveWorldObjectImagePath = "data/img/interactive/";
	cursorImagePath = "data/img/cursor/";
	playerImagePath = "data/img/player/";
	pathImagePath = "data/img/player/path/";
	npcImagePath = "data/img/npc/";
	guiImagePath = "data/img/gui/";
	itemImagePath = "data/img/items/";
	itemRarityIndicatorImagePath = "data/img/items/";
	unitImagePath = "data/img/units/";
	
	fontPath = "data/MerriweatherSans-Regular.ttf";
	
	audioPath = "data/audio/";
}

ResourceHandler::~ResourceHandler() {
	//Free all the textures
	for(std::map<std::string, SDL_Texture*>::const_iterator it = tileTextures.begin(); it != tileTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = worldObjectTextures.begin(); it != worldObjectTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = interactiveWorldObjectTextures.begin(); it != interactiveWorldObjectTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = cursorTextures.begin(); it != cursorTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = pathTextures.begin(); it != pathTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = npcTextures.begin(); it != npcTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = guiTextures.begin(); it != guiTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	for(std::map<Text, SDL_Texture*>::const_iterator it = renderedTexts.begin(); it != renderedTexts.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = itemTextures.begin(); it != itemTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = itemRarityIndicatorTextures.begin(); it != itemRarityIndicatorTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	
	SDL_DestroyTexture(playerTexture);
	
	TTF_CloseFont(font);
	
	//Free all the sounds
	for(std::map<std::string, Mix_Chunk*>::const_iterator it = sounds.begin(); it != sounds.end(); ++it) {
		Mix_FreeChunk(it->second);
	}
}

void ResourceHandler::loadAll() {
	loadImages();
	loadFont();
	loadColors();
	loadAudio();
}

SDL_Texture* ResourceHandler::getTextTexture(std::string text, SDL_Color color) {
	return getTextTexture(Text(text, color));
}

SDL_Texture* ResourceHandler::getTextTexture(Text text) {
	std::map<Text, SDL_Texture*>::iterator it = renderedTexts.find(text);
	if (it == renderedTexts.end()) {
		SDL_Surface* tempSurface;
		tempSurface = TTF_RenderText_Blended(font, text.getText().c_str(), text.getColor());
		SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(Global::renderer, tempSurface);
		SDL_FreeSurface(tempSurface);
		renderedTexts[text] = tempTexture;
		return tempTexture;
	} else {
		return it->second;
	}
}

void ResourceHandler::loadImages() {
	loadTerrainImages();
	loadWorldObjectImages();
	loadInteractiveWorldObjectImages();
	loadCursorImages();
	loadPlayerImages();
	loadPathImages();
	loadNPCImages();
	loadGUIImages();
	loadItemImages();
	loadItemRarityIndicatorImages();
	loadUnitImages();
}

void ResourceHandler::loadTerrainImages() {
	std::vector<std::string> textureNames = {"grass", "water", "sand"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		tileTextures[textureNames[i]] = loadTexture(terrainImagePath + textureNames[i] + ".png");
		tileTextureIDs[i] = textureNames[i];
	}
}

void ResourceHandler::loadWorldObjectImages() {
	std::vector<std::string> textureNames = {"mountain"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		worldObjectTextures[textureNames[i]] = loadTexture(worldObjectImagePath + textureNames[i] + ".png");
		worldObjectTextureIDs[i] = textureNames[i];
	}
}

void ResourceHandler::loadInteractiveWorldObjectImages() {
	std::vector<std::string> textureNames = {"castle"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		interactiveWorldObjectTextures[textureNames[i]] = loadTexture(interactiveWorldObjectImagePath + textureNames[i] + ".png");
		interactiveWorldObjectTextureIDs[i] = textureNames[i];
	}
}

void ResourceHandler::loadCursorImages() {
	std::vector<std::string> textureNames = {"impassable", "move", "friendly", "npc", "gui"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		cursorTextures[textureNames[i]] = loadTexture(cursorImagePath + textureNames[i] + ".png");
	}
}

void ResourceHandler::loadPlayerImages() {
	std::string textureName = "player";
	
	playerTexture = loadTexture(playerImagePath + textureName + ".png");
}

void ResourceHandler::loadPathImages() {
	std::vector<std::string> textureNames = {"destination", "up", "down", "left", "right", "upleft", "upright", "downleft", "downright"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		pathTextures[textureNames[i]] = loadTexture(pathImagePath + textureNames[i] + ".png");
	}
}

void ResourceHandler::loadNPCImages() {
	std::vector<std::string> textureNames = {"friend"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		npcTextures[textureNames[i]] = loadTexture(npcImagePath + textureNames[i] + ".png");
		npcTextureIDs[i] = textureNames[i];
	}
}

void ResourceHandler::loadGUIImages() {
	std::vector<std::string> textureNames = {"permabg", "button", "guiheader", "inventoryslot", "iteminfobg", "unitinfobg", "armybg", "armyslotbg", "selectedunit",
		"hoveredunit", "popupback", "popupfore"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		guiTextures[textureNames[i]] = loadTexture(guiImagePath + textureNames[i] + ".png");
	}
}

void ResourceHandler::loadItemImages() {
	std::vector<std::string> textureNames = {"sword", "shield", "amulet", "helmet", "ring", "staff", "roundshield", "poisonarrow", "frostcloak", "crossbow", "sickle", "boots",
		"bow", "ironarrow", "wand", "oldcloak", "mshield1", "mshield2", "bread"
	};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		itemTextures[textureNames[i]] = loadTexture(itemImagePath + textureNames[i] + ".png");
	}
}

void ResourceHandler::loadItemRarityIndicatorImages() {
	std::vector<std::string> textureNames = {"Common", "Rare", "Legendary", "Util"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		//NOTE the filenames start with an '_' character
		itemRarityIndicatorTextures[textureNames[i]] = loadTexture(itemRarityIndicatorImagePath + "_" + textureNames[i] + ".png");
	}
}

void ResourceHandler::loadUnitImages() {
	std::vector<std::string> textureNames = {"Axeman", "Dog", "Bowman", "Gray Mage", "_Dead"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		//NOTE the filenames start with an '_' character
		unitTextures[textureNames[i]] = loadTexture(unitImagePath + textureNames[i] + ".png");
	}
}

SDL_Texture* ResourceHandler::loadTexture(std::string path) {
	SDL_Texture* newTexture = NULL;
	
	//IMG_Load needs a c-type string (char*)
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		throw MediaNotFoundError();
	}
	
	newTexture = SDL_CreateTextureFromSurface(Global::renderer, loadedSurface);
	if (newTexture == NULL) {
		throw TextureConvertError();
	}
	
	SDL_FreeSurface(loadedSurface);
	
	return newTexture;
}

void ResourceHandler::loadFont() {
	font = TTF_OpenFont(fontPath.c_str(), Global::defaultFontSize);
	if (font == NULL) {
		throw MediaNotFoundError();
	}
}

void ResourceHandler::loadColors() {
	colors["whole-header"] = {220, 210, 200};
	colors["iteminfo-desc"] = {240, 240, 240};
	colors["unitinfo-desc"] = {235, 235, 235};
	colors["unitinfo-values-unchanged"] = {45, 75, 228};
	colors["unitinfo-values-incremented"] = {75, 228, 45};
	colors["unitinfo-values-decremented"] = {228, 45, 75};
	colors["button-text"] = {210, 220, 190};
	colors["popup-text"] = {210, 220, 190};
}

void ResourceHandler::loadAudio() {
	//http://lazyfoo.net/tutorials/SDL/21_sound_effects_and_music/index.php
	std::vector<std::string> soundNames = {"test"};
	for (unsigned int i = 0; i < soundNames.size(); i++) {
		sounds[soundNames[i]] = loadSound(audioPath + soundNames[i] + ".wav");
	}
}

Mix_Chunk* ResourceHandler::loadSound(std::string path) {
	//LoadWAV needs a c-type string (char*)
	Mix_Chunk* newChunk = Mix_LoadWAV(path.c_str());
	if (newChunk == NULL) {
		throw MediaNotFoundError();
	}
	return newChunk;
}
