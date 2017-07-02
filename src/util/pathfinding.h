#pragma once

#include <vector>
#include <queue>
#include <map>
#include <algorithm>

#include "point.h"

/*!
 * @author kovlev
 */


/*!
 * @class Pathfinding 
 * Basically a one method class
 * Uses the A* algorithm
 */
class Pathfinding {
public:
	//Pathfinding uses the data from the tilemap
	static std::vector<Point> findPath(Point start, Point destination);
	
	//Error getter, true if no error
	static bool good();
private:
	//Helper functions
	static std::vector<Point> getNeighbours(Point p);
	
	//Error management
	static bool wasLastPathfindingSuccessful;
};


/*!
 * @struct PathEntry
 * The frontier priority queue stores pathentries
 */
struct PathEntry {
	Point point;
	int distance;
	int cost;
};


/*!
 * @class PathPriority
 * Used in A*
 */
class PathPriority {
public:
	bool operator()(PathEntry a, PathEntry b);
};
