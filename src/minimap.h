#pragma once

#include <SDL2/SDL.h>

#include "guipart.h"

class Minimap : public GUIPart {
public:
	Minimap(int xp, int yp, int wp, int hp);
	~Minimap();
	
	//Renders the minimap
	void render();
	
	//Generates or regenerates the SDL_Surface* representing the minimap
	void regenerateMinimap();
private:
	//We are working with surfaces, because it is stored in the ram therefore its easier to manuipulate
	//On rendering we convert this into a texture
	SDL_Surface* minimapSurface;
	
	SDL_Texture* generatedTexture;
	
	void setPixel(SDL_Surface* surface, int x, int y, uint32_t pixel);
};
