#pragma once

#include <SDL.h>
#include <string>

/*!
 * @author kovlev
 */


/*!
 * @class Text
 * Consists of color, text and size
 * Used on text rendering
 * Fontsize is now speciable and works hopefully
 * Has a custom hashing system
 */
class Text {
public:
	Text(std::string t, SDL_Color col, uint8_t s);
	
	//Getters
	std::string getText();
	SDL_Color getColor();
	uint8_t getSize();
	
	//Setters
	void setText(std::string newText);
	void setColor(SDL_Color newColor);
	void setSize(uint8_t newSize);
	
	//Hash code with shifting magic
	uint64_t hashCode();
	
	//NOTE only for map comparission
	//Do not use this anywhere else
	bool operator<(const Text& other) const;
private:
	std::string text;
	SDL_Color color;
	uint8_t size;
	
	//Hashing stuff
	uint32_t stringHash32;
	uint32_t scHash32;
};
