#include "resourcehandler.h"

#include "global.h"

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
	
	colorsPath = "data/colors.data";
	
	chunkPath = "data/audio/sound/";
	musicPath = "data/audio/music/";
}

ResourceHandler::~ResourceHandler() {
	//Free all the textures
	stdex::clear_ptr_map(tileTextures);
	stdex::clear_ptr_map(worldObjectTextures);
	stdex::clear_ptr_map(cursorTextures);
	stdex::clear_ptr_map(pathTextures);
	stdex::clear_ptr_map(npcTextures);
	stdex::clear_ptr_map(guiTextures);
	stdex::clear_ptr_map(itemTextures);
	stdex::clear_ptr_map(itemRarityIndicatorTextures);
	stdex::clear_ptr_map(unitTextures);
	
	clearTextTextures();
	
	//Close all opened fonts
	for(auto it = loadedFonts.begin(); it != loadedFonts.end(); ++it) {
		TTF_CloseFont(it->second);
	}
	
	
	//Free all the sounds
	for(auto it = chunks.begin(); it != chunks.end(); ++it) {
		Mix_FreeChunk(it->second);
	}
	chunks.erase(chunks.begin(), chunks.end());
	
	for(auto it = music.begin(); it != music.end(); ++it) {
		Mix_FreeMusic(it->second);
	}
	music.erase(music.begin(), music.end());
}

void ResourceHandler::loadAll() {
	loadImages();
	loadColors();
	loadAudio();
}

ATexture* ResourceHandler::getATexture(TT tType, std::string name) {
	switch(tType) {
		case TT::TILE:
			if (tileTextures.find(name) == tileTextures.end()) {
				throw std::runtime_error("Image data missing: " + name);
			}
			return tileTextures[name];
		case TT::WORLDOBJECT:
			if (worldObjectTextures.find(name) == worldObjectTextures.end()) {
				throw std::runtime_error("Image data missing: " + name);
			}
			return worldObjectTextures[name];
		case TT::CURSOR:
			if (cursorTextures.find(name) == cursorTextures.end()) {
				throw std::runtime_error("Image data missing: " + name);
			}
			return cursorTextures[name];
		case TT::PATH:
			if (pathTextures.find(name) == pathTextures.end()) {
				throw std::runtime_error("Image data missing: " + name);
			}
			return pathTextures[name];
		case TT::NPC:
			if (npcTextures.find(name) == npcTextures.end()) {
				throw std::runtime_error("Image data missing: " + name);
			}
			return npcTextures[name];
		case TT::GUI:
			if (guiTextures.find(name) == guiTextures.end()) {
				throw std::runtime_error("Image data missing: " + name);
			}
			return guiTextures[name];
		case TT::ITEM:
			if (itemTextures.find(name) == itemTextures.end()) {
				throw std::runtime_error("Image data missing: " + name);
			}
			return itemTextures[name];
		case TT::ITRAIN:
			if (itemRarityIndicatorTextures.find(name) == itemRarityIndicatorTextures.end()) {
				throw std::runtime_error("Image data missing: " + name);
			}
			return itemRarityIndicatorTextures[name];
		case TT::UNIT:
			if (unitTextures.find(name) == unitTextures.end()) {
				throw std::runtime_error("Image data missing: " + name);
			}
			return unitTextures[name];
	}
	
	//Not needed possibly
	return NULL;
}

ATexture* ResourceHandler::getTextTexture(std::string text, SDL_Color color, uint8_t size) {
	return getTextTexture(Text(text, color, size));
}

ATexture* ResourceHandler::getTextTexture(Text text) {
	std::map<Text, ATexture*>::iterator it = renderedTexts.find(text);
	if (it == renderedTexts.end()) {
		SDL_Surface* tempSurface;
		tempSurface = TTF_RenderText_Blended(getFont(text.getSize()), text.getText().c_str(), text.getColor());
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
	stdex::clear_ptr_map(renderedTexts);
}

Mix_Chunk* ResourceHandler::getSound(std::string name) {
	if (chunks.find(name) == chunks.end()) {
		throw std::runtime_error("Audio data missing: " + name);
	}
	return chunks[name];
}

Mix_Music* ResourceHandler::getMusic(std::string name) {
	if (music.find(name) == music.end()) {
		throw std::runtime_error("Audio data missing: " + name);
	}
	return music[name];
}

SDL_Color ResourceHandler::getColor(std::string name) {
	if (colors.find(name) == colors.end()) {
		std::clog << "Error: Color missing: " << name << " Using black as fallback" << std::endl;
		return {0, 0, 0};
	}
	return colors[name];
}

SDL_Color ResourceHandler::getTileColor(std::string name) {
	if (tileAvgColors.find(name) == tileAvgColors.end()) {
		std::clog << "Error: Tile Color missing: " << name << " Using black as fallback" << std::endl;
		return {0, 0, 0};
	}
	return tileAvgColors[name];
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
		tileAvgColors[FilesystemHandler::removeExtension(textureNames[i])] = analyzeSurface(terrainImagePath + textureNames[i]);
	}
	
	for (unsigned int i = 0; i < directoryNames.size(); i++) {
		std::vector< std::pair<SDL_Texture*, int> > animationParts;
		//Gets all files in subdirectory
		std::vector<std::string> textureNamesInSubdir = FilesystemHandler::getFilesInDir(terrainImagePath + directoryNames[i]);
		for (unsigned int j = 0; j < textureNamesInSubdir.size(); j++) {
			animationParts.push_back({loadTexture(terrainImagePath + directoryNames[i] + "/" + textureNamesInSubdir[j]),
							/* string to int*/	std::stoi(textureNamesInSubdir[j], NULL)});
			if (j == 0) {
				//Only check the first tile
				//NOTE still unoptimized since we read most texture files twice
				tileAvgColors[directoryNames[i]] = analyzeSurface(terrainImagePath + directoryNames[i] + "/" + textureNamesInSubdir[j]);
			}
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
		std::clog << "Texture conversion failed at " << path << std::endl;
	}
	
	SDL_FreeSurface(loadedSurface);
	
	return newTexture;
}

SDL_Color ResourceHandler::analyzeSurface(std::string path) {
	//IMG_Load needs a c-type string (char*)
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	
	long rS = 0;
	long gS = 0;
	long bS = 0;
	
	int magicNumber = 123; // Does magic, shold be 3k
	
	//https://gamedev.stackexchange.com/questions/38186/trying-to-figure-out-sdl-pixel-manipulation
	//Uses a random pointer for temporarily storing the location of the pixels
	uint32_t* pixels = (uint32_t*)loadedSurface->pixels;
	
	//Optimized traversing
	for (int i = 0; i < loadedSurface->w * loadedSurface->h; i+=magicNumber) {
		//Does not really works with non 32 bit pixel formats so dont use them
		unsigned char r, g, b;
		SDL_GetRGB(pixels[i], loadedSurface->format, &r, &g, &b);
		rS += r;
		gS += g;
		bS += b;
	}
	
	//NOTE For some reason, this method generates slightly different shades in each run
	rS /= loadedSurface->w * loadedSurface->h / magicNumber;
	gS /= loadedSurface->w * loadedSurface->h / magicNumber;
	bS /= loadedSurface->w * loadedSurface->h / magicNumber;
	SDL_FreeSurface(loadedSurface);
	return {(unsigned char)rS, (unsigned char)gS, (unsigned char)bS};
}

TTF_Font* ResourceHandler::getFont(int size) {
	std::map<int, TTF_Font*>::iterator it = loadedFonts.find(size);
	if (it == loadedFonts.end()) {
		TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
		if (font == NULL) {
			throw std::runtime_error("Font data missing: " + fontPath);
		}
		loadedFonts[size] = font;
		return font;
	} else {
		return it->second;
	}
}

void ResourceHandler::loadColors() {
	//Uses file IO to get the color list
	std::fstream file;
	file.open(colorsPath, std::ios::in);
	
	std::string name;
	int r, g, b;
	
	while (file >> name && file >> r && file >> g && file >> b) {
		colors[name] = {(uint8_t)r, (uint8_t)g, (uint8_t)b};
	}
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
	Mix_Chunk* newChunk = Mix_LoadWAV(path.c_str());
	if (newChunk == NULL) {
		std::clog << "Error: Audio loading failed at " << path << std::endl;
	}
	return newChunk;
}

Mix_Music* ResourceHandler::loadMusic(std::string path) {
	//LoadWAV needs a c-type string (char*)
	Mix_Music* newMusic = Mix_LoadMUS(path.c_str());
	if (newMusic == NULL) {
		std::clog << "Error: Audio loading failed at " << path << std::endl;
	}
	return newMusic;
}
