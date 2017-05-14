#include "resourcehandler.h"

#include "global.h"
#include "../util/exceptions.h"

/*!
 * @author kovlev
 */

ResourceHandler::ResourceHandler() {
	terrainImagePath = "data/img/terrain/";
	worldObjectImagePath = "data/img/worldobject/";
	cursorImagePath = "data/img/cursor/";
	pathImagePath = "data/img/path/";
	npcImagePath = "data/img/npc/";
	guiImagePath = "data/img/gui/";
	itemImagePath = "data/img/items/";
	itemRarityIndicatorImagePath = "data/img/iind/";
	unitImagePath = "data/img/units/";
	
	fontPath = "data/MerriweatherSans-Regular.ttf";
	
	chunkPath = "data/audio/sound/";
	musicPath = "data/audio/music/";
}

ResourceHandler::~ResourceHandler() {
	//Free all the textures
	for(std::map<std::string, ATexture*>::const_iterator it = tileTextures.begin(); it != tileTextures.end(); ++it) {
		delete it->second;
	}
	tileTextures.erase(tileTextures.begin(), tileTextures.end());
	
	for(std::map<std::string, ATexture*>::const_iterator it = worldObjectTextures.begin(); it != worldObjectTextures.end(); ++it) {
		delete it->second;
	}
	worldObjectTextures.erase(worldObjectTextures.begin(), worldObjectTextures.end());
	
	for(std::map<std::string, ATexture*>::const_iterator it = cursorTextures.begin(); it != cursorTextures.end(); ++it) {
		delete it->second;
	}
	cursorTextures.erase(cursorTextures.begin(), cursorTextures.end());
	
	for(std::map<std::string, ATexture*>::const_iterator it = pathTextures.begin(); it != pathTextures.end(); ++it) {
		delete it->second;
	}
	pathTextures.erase(pathTextures.begin(), pathTextures.end());
	
	for(std::map<std::string, ATexture*>::const_iterator it = npcTextures.begin(); it != npcTextures.end(); ++it) {
		delete it->second;
	}
	npcTextures.erase(npcTextures.begin(), npcTextures.end());
	
	for(std::map<std::string, ATexture*>::const_iterator it = guiTextures.begin(); it != guiTextures.end(); ++it) {
		delete it->second;
	}
	guiTextures.erase(guiTextures.begin(), guiTextures.end());
	
	for(std::map<std::string, ATexture*>::const_iterator it = itemTextures.begin(); it != itemTextures.end(); ++it) {
		delete it->second;
	}
	itemTextures.erase(itemTextures.begin(), itemTextures.end());
	
	for(std::map<std::string, ATexture*>::const_iterator it = itemRarityIndicatorTextures.begin(); it != itemRarityIndicatorTextures.end(); ++it) {
		delete it->second;
	}
	itemRarityIndicatorTextures.erase(itemRarityIndicatorTextures.begin(), itemRarityIndicatorTextures.end());
	
	for(std::map<std::string, ATexture*>::const_iterator it = unitTextures.begin(); it != unitTextures.end(); ++it) {
		delete it->second;
	}
	unitTextures.erase(unitTextures.begin(), unitTextures.end());
	
	
	clearTextTextures();
	
	TTF_CloseFont(font);
	
	
	//Free all the sounds
	for(std::map<std::string, Mix_Chunk*>::const_iterator it = chunks.begin(); it != chunks.end(); ++it) {
		Mix_FreeChunk(it->second);
	}
	chunks.erase(chunks.begin(), chunks.end());
	
	for(std::map<std::string, Mix_Music*>::const_iterator it = music.begin(); it != music.end(); ++it) {
		Mix_FreeMusic(it->second);
	}
	music.erase(music.begin(), music.end());
}

void ResourceHandler::loadAll() {
	loadImages();
	loadFont();
	loadColors();
	loadAudio();
}

ATexture* ResourceHandler::getATexture(TT tType, std::string name) {
	switch(tType) {
		case TT::TILE:
			if (tileTextures.find(name) == tileTextures.end()) {
				throw MediaNotFoundError(name);
			}
			return tileTextures[name];
		case TT::WORLDOBJECT:
			if (worldObjectTextures.find(name) == worldObjectTextures.end()) {
				throw MediaNotFoundError(name);
			}
			return worldObjectTextures[name];
		case TT::CURSOR:
			if (cursorTextures.find(name) == cursorTextures.end()) {
				throw MediaNotFoundError(name);
			}
			return cursorTextures[name];
		case TT::PATH:
			if (pathTextures.find(name) == pathTextures.end()) {
				throw MediaNotFoundError(name);
			}
			return pathTextures[name];
		case TT::NPC:
			if (npcTextures.find(name) == npcTextures.end()) {
				throw MediaNotFoundError(name);
			}
			return npcTextures[name];
		case TT::GUI:
			if (guiTextures.find(name) == guiTextures.end()) {
				throw MediaNotFoundError(name);
			}
			return guiTextures[name];
		case TT::ITEM:
			if (itemTextures.find(name) == itemTextures.end()) {
				throw MediaNotFoundError(name);
			}
			return itemTextures[name];
		case TT::ITRAIN:
			if (itemRarityIndicatorTextures.find(name) == itemRarityIndicatorTextures.end()) {
				throw MediaNotFoundError(name);
			}
			return itemRarityIndicatorTextures[name];
		case TT::UNIT:
			if (unitTextures.find(name) == unitTextures.end()) {
				throw MediaNotFoundError(name);
			}
			return unitTextures[name];
	}
	
	//Not needed possibly
	return NULL;
}

ATexture* ResourceHandler::getTextTexture(std::string text, SDL_Color color) {
	return getTextTexture(Text(text, color));
}

ATexture* ResourceHandler::getTextTexture(Text text) {
	std::map<Text, ATexture*>::iterator it = renderedTexts.find(text);
	if (it == renderedTexts.end()) {
		SDL_Surface* tempSurface;
		tempSurface = TTF_RenderText_Blended(font, text.getText().c_str(), text.getColor());
		SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(Global::renderer, tempSurface);
		SDL_FreeSurface(tempSurface);
		ATexture* tempATexture = new ATexture(tempTexture);
		renderedTexts[text] = tempATexture;
		return tempATexture;
	} else {
		return it->second;
	}
}

void ResourceHandler::clearTextTextures() {
	for(std::map<Text, ATexture*>::const_iterator it = renderedTexts.begin(); it != renderedTexts.end(); ++it) {
		delete it->second;
	}
	renderedTexts.erase(renderedTexts.begin(), renderedTexts.end());
}

Mix_Chunk* ResourceHandler::getSound(std::string name) {
	if (chunks.find(name) == chunks.end()) {
		throw MediaNotFoundError(name);
	}
	return chunks[name];
}

Mix_Music* ResourceHandler::getMusic(std::string name) {
	if (music.find(name) == music.end()) {
		throw MediaNotFoundError(name);
	}
	return music[name];
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
	std::pair< std::vector<std::string>, std::vector<std::string> > textureNamesWrapped = FilesystemHandler::getFilesAndDirsInDir(terrainImagePath);
	std::vector<std::string> textureNames = textureNamesWrapped.first;
	std::vector<std::string> directoryNames = textureNamesWrapped.second;
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		tileTextures[FilesystemHandler::removeExtension(textureNames[i])] = new ATexture(loadTexture(terrainImagePath + textureNames[i]));
	}
	
	for (unsigned int i = 0; i < directoryNames.size(); i++) {
		std::vector< std::pair<SDL_Texture*, int> > animationParts;
		//Gets all files in subdirectory
		std::vector<std::string> textureNamesInSubdir = FilesystemHandler::getFilesInDir(terrainImagePath + directoryNames[i]);
		for (unsigned int j = 0; j < textureNamesInSubdir.size(); j++) {
			animationParts.push_back({loadTexture(terrainImagePath + directoryNames[i] + "/" + textureNamesInSubdir[j]),
							/* string to int*/	std::stoi(textureNamesInSubdir[j], NULL)});
		}
		tileTextures[directoryNames[i]] = new ATexture(animationParts);
	}
}

void ResourceHandler::loadWorldObjectImages() {
	std::pair< std::vector<std::string>, std::vector<std::string> > textureNamesWrapped = FilesystemHandler::getFilesAndDirsInDir(worldObjectImagePath);
	std::vector<std::string> textureNames = textureNamesWrapped.first;
	std::vector<std::string> directoryNames = textureNamesWrapped.second;
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		worldObjectTextures[FilesystemHandler::removeExtension(textureNames[i])] = new ATexture(loadTexture(worldObjectImagePath + textureNames[i]));
	}
	
	for (unsigned int i = 0; i < directoryNames.size(); i++) {
		std::vector< std::pair<SDL_Texture*, int> > animationParts;
		//Gets all files in subdirectory
		std::vector<std::string> textureNamesInSubdir = FilesystemHandler::getFilesInDir(worldObjectImagePath + directoryNames[i]);
		for (unsigned int j = 0; j < textureNamesInSubdir.size(); j++) {
			animationParts.push_back({loadTexture(worldObjectImagePath + directoryNames[i] + "/" + textureNamesInSubdir[j]),
							/* string to int*/	std::stoi(textureNamesInSubdir[j], NULL)});
		}
		worldObjectTextures[directoryNames[i]] = new ATexture(animationParts);
	}
}

void ResourceHandler::loadCursorImages() {
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(cursorImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		cursorTextures[FilesystemHandler::removeExtension(textureNames[i])] = new ATexture(loadTexture(cursorImagePath + textureNames[i]));
	}
}

void ResourceHandler::loadPathImages() {
	std::vector<std::string> textureNames = {"destination", "up", "down", "left", "right", "upleft", "upright", "downleft", "downright"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		pathTextures[textureNames[i]] = new ATexture(loadTexture(pathImagePath + textureNames[i] + ".png"));
	}
}

void ResourceHandler::loadNPCImages() {
	std::vector<std::string> directoryNames = FilesystemHandler::getDirsInDir(npcImagePath);
	
	for (unsigned int i = 0; i < directoryNames.size(); i++) {
		std::vector<SDL_Texture*> rotationTextures;
		
		std::vector<std::string> rotationNames = {"down", "downleft", "left", "upleft", "up", "upright", "right", "downright"};
		
		for (unsigned int j = 0; j < rotationNames.size(); j++) {
			rotationTextures.push_back(loadTexture(npcImagePath + directoryNames[i] + "/" + rotationNames[j] + ".png"));
		}
		
		npcTextures[directoryNames[i]] = new ATexture(rotationTextures);
	}
}

void ResourceHandler::loadGUIImages() {
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(guiImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		guiTextures[FilesystemHandler::removeExtension(textureNames[i])] = new ATexture(loadTexture(guiImagePath + textureNames[i]));
	}
}

void ResourceHandler::loadItemImages() {
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(itemImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		itemTextures[FilesystemHandler::removeExtension(textureNames[i])] = new ATexture(loadTexture(itemImagePath + textureNames[i]));
	}
}

void ResourceHandler::loadItemRarityIndicatorImages() {
	std::vector<std::string> textureNames = {"Common", "Rare", "Legendary", "Util"};
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		itemRarityIndicatorTextures[textureNames[i]] = new ATexture(loadTexture(itemRarityIndicatorImagePath + textureNames[i] + ".png"));
	}
}

void ResourceHandler::loadUnitImages() {
	std::vector<std::string> textureNames = FilesystemHandler::getFilesInDir(unitImagePath);
	
	for (unsigned int i = 0; i < textureNames.size(); i++) {
		unitTextures[FilesystemHandler::removeExtension(textureNames[i])] = new ATexture(loadTexture(unitImagePath + textureNames[i]));
	}
}

SDL_Texture* ResourceHandler::loadTexture(std::string path) {
	SDL_Texture* newTexture = NULL;
	
	//IMG_Load needs a c-type string (char*)
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::clog << "Error: Invalid media file at " << path << std::endl;
		return NULL;
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
		throw MediaNotFoundError(fontPath);
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
	colors["debug-coord"] = {0, 255, 255};
	colors["gold"] = {220, 220, 14};
}

void ResourceHandler::loadAudio() {
	loadChunkFiles();
	loadMusicFiles();
}

void ResourceHandler::loadChunkFiles() {
	std::vector<std::string> chunkNames = FilesystemHandler::getFilesInDir(chunkPath);
	
	for (unsigned int i = 0; i < chunkNames.size(); i++) {
		chunks[FilesystemHandler::removeExtension(chunkNames[i])] = loadChunk(chunkPath + chunkNames[i]);
	}
}

void ResourceHandler::loadMusicFiles() {
	std::vector<std::string> musicNames = FilesystemHandler::getFilesInDir(musicPath);
	
	for (unsigned int i = 0; i < musicNames.size(); i++) {
		music[FilesystemHandler::removeExtension(musicNames[i])] = loadMusic(musicPath + musicNames[i]);
	}
}

Mix_Chunk* ResourceHandler::loadChunk(std::string path) {
	//LoadWAV needs a c-type string (char*)
	//TODO add support for other formats
	Mix_Chunk* newChunk = Mix_LoadWAV(path.c_str());
	if (newChunk == NULL) {
		throw MediaNotFoundError(path);
	}
	return newChunk;
}

Mix_Music* ResourceHandler::loadMusic(std::string path) {
	//LoadWAV needs a c-type string (char*)
	Mix_Music* newMusic = Mix_LoadMUS(path.c_str());
	if (newMusic == NULL) {
		throw MediaNotFoundError(path);
	}
	return newMusic;
}
