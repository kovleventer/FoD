#pragma once

#include <SDL.h>
#include <vector>
#include <utility>

#include "../util/point.h"

/*!
 * @author kovlev
 */


/*!
 * @enum TextureType which type is the texture
 * 	STATIC for simple textures which can be represented by one SDL_Texture
 * 	ANIMATED textures change over time
 * 	ROTATING textures are used on character, whose movement may change their textures
 */
enum class TextureType {
	STATIC,
	ANIMATED,
	ROTATING
};


/*!
 * @enum RotationType used for easier translation of things
 * It does not make any sense by itself, but it works
 */
enum class RotationType {
	DOWN = 0,
	DOWNLEFT = 1,
	LEFT = 2,
	UPLEFT = 3,
	UP = 4,
	UPRIGHT = 5,
	RIGHT = 6,
	DOWNRIGHT = 7
};


/*!
 * @class ATexture is a renderable texture
 * It is like SDL_Texture but it has extended functionality and more convinient usage
 */
class ATexture {
public:
	//For static textures
	ATexture(SDL_Texture* staticTexture);
	
	//For animated textures
	ATexture(std::vector< std::pair<SDL_Texture*, int> > textParts);
	
	//For rotating textures like npcs
	ATexture(std::vector<SDL_Texture*> textParts);
	
	~ATexture();
	
	//Getters
	Dimension getDimensions();
	TextureType getTextureType();
	int getAnimCycleDur();
	int getCurrentDur();
	
	//Renders the texture using the renderer defined in the Global class
	void render(SDL_Rect destinationRect);
	
	//Sets the next texture in animation circle
	//Can be used only on ANIMATED textures
	void nextTexture();
	
	//Sets the rotation
	//Translation is done by the enum class
	void setRotation(RotationType rotType);
private:
	TextureType textureType;
	
	//For all textures this is used textures
	SDL_Texture* currentTexture;
	
	//For animated textures
	std::vector< std::pair<SDL_Texture*, int> > textureParts;
	//The max value of the durations
	int totalAnimationCycleDuration;
	unsigned int currentIndex;
	
	//Sorts the vector by the second part of the pair
	void sortTextureParts();
	
	//For rotating textures
	std::vector<SDL_Texture*> rotTextureParts;
};
