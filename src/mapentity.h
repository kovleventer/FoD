#pragma once

#include <SDL2/SDL.h>
#include <algorithm>

#include "point.h"
#include "animatabletexture.h"

class MapEntity {
public:
	MapEntity(int x, int y);
	MapEntity(Point pos);
	MapEntity(int x, int y, double s);
	MapEntity(Point pos, double s);
	//Virtual destructor
	virtual ~MapEntity();
	
	ATexture* texture;
	
	//Getters
	Point getPosition();
	double getScale();
	bool isAtBackground();
	
	//Setters
	void setPosition(Point newPosition);
	void setScale(double newScale);
//Protected, because we are going to inherit from this class
protected:
	//Every world object is connected to a tile
	//While the tile itself does not contain its position (it would be too memory-expensive)
	//We must store the coordinates of the tile
	Point position;
	
	//Texture size multiplier
	double scale;
	
	//Different destructor behaviours
	//NOTE not the best design choice
	bool atBackground;
};
