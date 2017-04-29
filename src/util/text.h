#pragma once

#include <SDL2/SDL.h>
#include <string>

/*!
 * @author kovlev
 */


/*!
 * @class Text a pair of color and text
 * Used on text rendering
 * Fontsize is not specified, since it is not easily configurable
 * Has a custom hashing system
 */
class Text {
public:
	Text(std::string t, SDL_Color col);
	
	//Getters
	std::string getText();
	SDL_Color getColor();
	
	//Setters
	void setText(std::string newText);
	void setColor(SDL_Color newColor);
	
	//Hash code with shifting magic
	uint64_t hashCode();
	
	//NOTE only for map comparission
	//Do not use this anywhere else
	bool operator<(const Text& other) const;
private:
	std::string text;
	//uint8_t size;
	SDL_Color color;
	
	uint32_t stringHash32;
	uint32_t scHash32;
};
