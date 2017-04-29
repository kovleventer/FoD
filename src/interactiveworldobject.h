#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "point.h"
#include "worldobject.h"

class InteractiveWorldObject : public WorldObject {
public:
	InteractiveWorldObject(std::string text, int x, int y, std::vector<Point> intTiles);
	InteractiveWorldObject(std::string text, Point pos, std::vector<Point> intTiles);
	
	//Getters
	std::vector<Point> getInteractiveTiles();
	
	//TODO activation method
protected:
	//Impassable tiles are relative; (0, 0) means the positionTile
	std::vector<Point> interactiveTiles;
};
