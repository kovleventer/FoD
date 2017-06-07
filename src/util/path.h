#pragma once

#include <vector>

#include "point.h"

/*!
 * @author kovlev
 */


/*!
 * @class BasePath
 */
class BasePath {
public:
	BasePath();
	virtual ~BasePath();
	
	//Getters
	unsigned int getPositionIndex();
	
	//Gets the next point in the path
	virtual Point next() = 0;
	//Gets the current point in the path
	virtual Point current() = 0;
	
	virtual bool isAtFirst() = 0;
	virtual bool isAtLast() = 0;
	
	//Moves forward one tile (sets the position index)
	virtual void moveForward() = 0;
protected:
	//Indicates the current position
	unsigned int positionIndex;
};


/*!
 * @class SimplePath represents a path
 */
class SimplePath : public BasePath {
public:
	SimplePath(Point start, Point end);
	
	Point next();
	Point current();
	
	bool isAtFirst();
	bool isAtLast();
	
	void moveForward();
private:
	std::vector<Point> path;
};


/*!
 * @class CircularPath A wrapper for a list of points
 * The first point comes after the last one, therefore it makes a circle
 */
class CircularPath : public BasePath {
public:
	//We pass a list of points representing the npc's destinations
	CircularPath(std::vector<Point> points);
	
	Point next();
	Point current();
	
	bool isAtFirst();
	bool isAtLast();
	
	//Moves forward one tile (sets the position index)
	void moveForward();
private:
	//Uses pathfinding on initialization
	void generatePath(std::vector<Point> points);
	
	//The actual path is represented by a vector
	//This class is a wrapper on this
	std::vector<Point> path;
};
