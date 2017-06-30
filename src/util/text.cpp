#include "text.h"

/*!
 * @author kovlev
 */

Text::Text(std::string t, SDL_Color col, uint8_t s) {
	text = t;
	color = col;
	size = s;
	
	//On 32-bit sytems std::hash returns a 32-bit size_t, which does not require any additional shifting
	#if _WIN64 || __x86_64__ || __ppc64__
        //64 bit case
		stringHash32 = std::hash<std::string>()(text) >> 32;
	#else
		//32 bit case
		stringHash32 = std::hash<std::string>()(text);
	#endif
	scHash32 = 0x00 | (color.r << 8) | (color.g << 16) | (color.b << 24) | size;
}

std::string Text::getText() {
	return text;
}

SDL_Color Text::getColor() {
	return color;
}

uint8_t Text::getSize() {
	return size;
}

void Text::setText(std::string newText) {
	text = newText;
}

void Text::setColor(SDL_Color newColor) {
	color = newColor;
}

void Text::setSize(uint8_t newSize) {
	size = newSize;
}

uint64_t Text::hashCode() {
	return ((uint64_t)stringHash32) << 32 | scHash32;
}

bool Text::operator<(const Text& other) const {
	//Shifting magic
	uint64_t hashT1 = ((uint64_t)stringHash32) << 32 | scHash32;
	uint64_t hashT2 = ((uint64_t)other.stringHash32) << 32 | other.scHash32;
	return hashT1 < hashT2;
}
