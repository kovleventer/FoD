#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "point.h"
#include "mapentity.h"

//This class is the most redundant thing I've ever written
//TODO maybe pivot system
class WorldObject : public MapEntity {
public:
	WorldObject(std::string text, int x, int y);
	WorldObject(std::string text, Point pos);
};
