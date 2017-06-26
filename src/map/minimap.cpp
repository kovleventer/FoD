#include "minimap.h"

#include "../core/global.h"

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
	if (tileSizeInPixels == 0) tileSizeInPixels = 1;
	
	for (int i = 0; i < Global::map->getWidth(); i++) {
		for (int j = 0; j < Global::map->getHeight(); j++) {
			std::string currentType = Global::map->getTile(i, j)->getType();
			SDL_Color color = Global::resourceHandler->getTileColor(currentType);
			uint32_t currColor = 0x00 | (color.b << 0) | (color.g << 8) | (color.r << 16);
			
			for (int k = 0; k <= tileSizeInPixels; k++) {
				for (int l = 0; l <= tileSizeInPixels; l++) {
					setPixel(minimapSurface, i * tileSizeInPixels + k, j * tileSizeInPixels + l, currColor);
				}
			}
		}
	}
	if (generatedTexture != NULL) {
		SDL_DestroyTexture(generatedTexture);
	}
	generatedTexture = SDL_CreateTextureFromSurface(Global::renderer, minimapSurface);
}

void Minimap::setPixel(SDL_Surface* surface, int x, int y, uint32_t pixel) {
	if (x >= surface->w || y >= surface->h) return;
	//https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface
	uint8_t *target_pixel = (uint8_t *)surface->pixels + y * surface->pitch + x * 4;
	*(uint32_t *)target_pixel = pixel;
}
