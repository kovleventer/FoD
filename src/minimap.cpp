#include "minimap.h"

#include "global.h"

/*!
 * @author kovlev
 */

Minimap::Minimap(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	//https://wiki.libsdl.org/SDL_CreateRGBSurface
	minimapSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
	generatedTexture = NULL;
}

Minimap::~Minimap() {
	//Frees surface and destroys texture
	SDL_FreeSurface(minimapSurface);
	SDL_DestroyTexture(generatedTexture);
}

void Minimap::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, h};
	SDL_RenderCopy(Global::renderer, generatedTexture, NULL, &destinationRect);
}

void Minimap::regenerateMinimap() {
	//NOTE does not check height
	int tileSizeInPixels = w / Global::gameBoardWidth;
	
	for (int i = 0; i < Global::map->getWidth(); i++) {
		for (int j = 0; j < Global::map->getHeight(); j++) {
			std::string currentType = Global::map->getTile(i, j)->getType();
			
			for (int k = 0; k < tileSizeInPixels; k++) {
				for (int l = 0; l < tileSizeInPixels; l++) {
					//https://stackoverflow.com/questions/650162/why-switch-statement-cannot-be-applied-on-strings
					if (currentType == "grass") {
						setPixel(minimapSurface, i * tileSizeInPixels + k, j * tileSizeInPixels + l, 0x0000FF00);
					} else if (currentType == "water") {
						setPixel(minimapSurface, i * tileSizeInPixels + k, j * tileSizeInPixels + l, 0x000000FF);
					} else if (currentType == "sand") {
						setPixel(minimapSurface, i * tileSizeInPixels + k, j * tileSizeInPixels + l, 0x00FFFF00);
					} else if (currentType == "snow") {
						setPixel(minimapSurface, i * tileSizeInPixels + k, j * tileSizeInPixels + l, 0x00FFFFFF);
					}
				}
			}
		}
	}
	
	generatedTexture = SDL_CreateTextureFromSurface(Global::renderer, minimapSurface);
}

void Minimap::setPixel(SDL_Surface *surface, int x, int y, uint32_t pixel) {
	//https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface
	uint8_t *target_pixel = (uint8_t *)surface->pixels + y * surface->pitch + x * 4;
	*(uint32_t *)target_pixel = pixel;
}
