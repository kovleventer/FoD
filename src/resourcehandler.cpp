#include "resourcehandler.h"

#include "global.h"
#include "exceptions.h"

ResourceHandler::ResourceHandler() {
	terrainImagePath = "data/img/terrain/";
	worldObjectImagePath = "data/img/worldobject/";
	cursorImagePath = "data/img/cursor/";
	pathImagePath = "data/img/path/";
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
	tileTextures.erase(tileTextures.begin(), tileTextures.end());
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = worldObjectTextures.begin(); it != worldObjectTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	worldObjectTextures.erase(worldObjectTextures.begin(), worldObjectTextures.end());
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = cursorTextures.begin(); it != cursorTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	cursorTextures.erase(cursorTextures.begin(), cursorTextures.end());
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = pathTextures.begin(); it != pathTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	pathTextures.erase(pathTextures.begin(), pathTextures.end());
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = npcTextures.begin(); it != npcTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	npcTextures.erase(npcTextures.begin(), npcTextures.end());
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = guiTextures.begin(); it != guiTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	guiTextures.erase(guiTextures.begin(), guiTextures.end());
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = itemTextures.begin(); it != itemTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	itemTextures.erase(itemTextures.begin(), itemTextures.end());
	
	for(std::map<std::string, SDL_Texture*>::const_iterator it = itemRarityIndicatorTextures.begin(); it != itemRarityIndicatorTextures.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	itemRarityIndicatorTextures.erase(itemRarityIndicatorTextures.begin(), itemRarityIndicatorTextures.end());
	
	clearTextTextures();
	
	TTF_CloseFont(font);
	
	//Free all the sounds
	for(std::map<std::string, Mix_Chunk*>::const_iterator it = sounds.begin(); it != sounds.end(); ++it) {
		Mix_FreeChunk(it->second);
	}
	sounds.erase(sounds.begin(), sounds.end());
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

void ResourceHandler::clearTextTextures() {
	for(std::map<Text, SDL_Texture*>::const_iterator it = renderedTexts.begin(); it != renderedTexts.end(); ++it) {
		SDL_DestroyTexture(it->second);
	}
	renderedTexts.erase(renderedTexts.begin(), renderedTexts.end());
}

void ResourceHandler::loadImages() {
	loadTerrainImages();
	loadWorldObjectImages();
	loadCursorImages();
	loadPathImages();
	loadNPCImages();
	loadGUIImages();
	loadItemImages();
	loadItemRarityIndicatorImages();
	loadUnitImages();
}

void ResourceHandler::loadTerrainImages() {
	std::vector<std::string> textureNames = {"grass", "water", "sand", "snow"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		tileTextures[textureNames[i]] = loadTexture(terrainImagePath + textureNames[i] + ".png");
		tileTextureIDs[i] = textureNames[i];
	}
}

void ResourceHandler::loadWorldObjectImages() {
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(worldObjectImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		worldObjectTextures[FilesystemHandler::removeExtension(textureNames[i])] = loadTexture(worldObjectImagePath + textureNames[i]);
	}
}

void ResourceHandler::loadCursorImages() {
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(cursorImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		cursorTextures[FilesystemHandler::removeExtension(textureNames[i])] = loadTexture(cursorImagePath + textureNames[i]);
	}
}

void ResourceHandler::loadPathImages() {
	std::vector<std::string> textureNames = {"destination", "up", "down", "left", "right", "upleft", "upright", "downleft", "downright"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		pathTextures[textureNames[i]] = loadTexture(pathImagePath + textureNames[i] + ".png");
	}
}

void ResourceHandler::loadNPCImages() {
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(npcImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		npcTextures[FilesystemHandler::removeExtension(textureNames[i])] = loadTexture(npcImagePath + textureNames[i]);
	}
}

void ResourceHandler::loadGUIImages() {
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(guiImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		guiTextures[FilesystemHandler::removeExtension(textureNames[i])] = loadTexture(guiImagePath + textureNames[i]);
	}
}

void ResourceHandler::loadItemImages() {
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(itemImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		itemTextures[FilesystemHandler::removeExtension(textureNames[i])] = loadTexture(itemImagePath + textureNames[i]);
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
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(unitImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		unitTextures[FilesystemHandler::removeExtension(textureNames[i])] = loadTexture(unitImagePath + textureNames[i]);
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
