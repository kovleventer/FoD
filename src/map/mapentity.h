#pragma once

#include <SDL.h>
#include <algorithm>

#include "../util/point.h"
#include "../core/animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @class MapEntity the base class for all things in the map (except tiles)
 */
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
