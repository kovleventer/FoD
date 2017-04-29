#pragma once

#include <string>
#include <vector>

#include "worldobject.h"
#include "util/point.h"

/*!
 * @author kovlev
 */


/*!
 * @class ImpassableWorldObject a world object that has tiles that other characters can not pass through
 * For example a mountain
 */
class ImpassableWorldObject : public WorldObject {
public:
	ImpassableWorldObject(std::string text, int x, int y, std::vector<Point> impTiles);
	ImpassableWorldObject(std::string text, Point pos, std::vector<Point> impTiles);
	
	//Getters
	std::vector<Point> getImpassableTiles();
private:
	std::vector<Point> impassableTiles;
};
