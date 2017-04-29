#pragma once

#include <SDL2/SDL.h>
#include <algorithm>

#include "point.h"

class MapEntity {
public:
	MapEntity(Point pos);
	//Virtual destructor
	virtual ~MapEntity();
	
	//NOTE might become deprecated when working with animations
	SDL_Texture* texture;
	
	//Getters
	Point getPosition();
	
	//Setters
	void setPosition(Point newPosition);
//Protected, because we are going to inherit from this class
protected:
	//Every world ovject is connected to a tile
	//While the tile itself does not contain its position (it would be too memory-expensive)
	//We must store the coordinates of the tile
	Point position;
};
