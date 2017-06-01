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
 * @class Pathfinding basically a one method class
 * Uses a highy unoptimized algorithm (BFS) and an unsafe implementation
 */
class Pathfinding {
public:
	//Pathfinding uses the data from the tilemap
	//NOTE uses BFS
	static std::vector<Point> findPath(Point start, Point destination);
private:
	//Helper functions
	static std::vector<Point> getNeighbours(Point p);
};
