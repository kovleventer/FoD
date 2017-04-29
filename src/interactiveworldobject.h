#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "point.h"
#include "mapentity.h"

class InteractiveWorldObject : public MapEntity {
public:
	InteractiveWorldObject(int textID, Point tile, std::vector<Point> inter);
	virtual ~InteractiveWorldObject();
	
	//Getters
	std::vector<Point> getInteractiveTiles();
	
	//TODO activation method
protected:
	//Impassable tiles are relative(0, 0) means the positionTile
	std::vector<Point> interactiveTiles;
};
