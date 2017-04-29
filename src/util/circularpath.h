#pragma once

#include <vector>

#include "point.h"

/*!
 * @author kovlev
 */


/*!
 * @class CircularPath A wrapper for a list of points
 * The first point comes after the last one, therefore it makes a circle
 */
class CircularPath {
public:
	//We pass a list of points representing the npc's destinations
	CircularPath(std::vector<Point> points);
	
	//Gets the next point in the path
	Point next();
	//Gets the current point in the path
	Point current();
	
	//Moves forward one tile (sets the position index)
	void moveForward();
private:
	//Uses pathfinding on initialization
	void generatePath(std::vector<Point> points);
	
	//The actual path is represented by a vector
	//This class is a wrapper on this
	std::vector<Point> path;
	
	//The NPC's current position
	unsigned int positionIndex;
};
