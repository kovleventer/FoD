#include "pathfinding.h"

#include "../core/global.h"
#include "../map/tile.h"

/*!
 * @author kovlev
 */

std::vector<Point> Pathfinding::findPath(Point start, Point destination) {
	//See for resources:
	//http://www.redblobgames.com/pathfinding/a-star/introduction.html
	
	//We keep track the previos point
	std::map<Point, Point> cameFrom;
	cameFrom[start] = Point::INVALID;
	
	//Tiles to check
	std::priority_queue< PathEntry, std::vector<PathEntry>, PathPriority> frontier;
	frontier.push({start, 0, 0});
	
	int count = 0;
	int maxIterations = 1000;
	
	std::vector<Point> path;
	
	while (!frontier.empty()) {
		//Store the first element and then remove it from the queue
		Point current = frontier.top().point;
		int cost = frontier.top().cost;
		frontier.pop();
		
		//If we found it, we do an early exit
		if (current == destination) break;
		
		std::vector<Point> neighbours = getNeighbours(current);
		
		for (unsigned int i = 0; i < neighbours.size(); i++) {
			Point next = neighbours[i];
			
			//If not passable, we skip that tile
			if (Global::map->getTile(next)->getTileInfo() == TileInfo::IMPASSABLE) continue;
			
			//If we have not visited the tile, we add it to the queue and the map
			std::map<Point, Point>::iterator it = cameFrom.find(next);
			if (it == cameFrom.end()) {
				frontier.push({next, destination.distanceToManhattan(next) + cost + 1, cost + 1});
				cameFrom[next] = current;
			}
		}
		count++;
		if (count == maxIterations) {
			wasLastPathfindingSuccessful = false;
			return path;
		}
	}
	
	if (cameFrom.find(destination) == cameFrom.end()) {
		wasLastPathfindingSuccessful = false;
		return path;
	}
	
	//Get the final path from the map
	//NOTE both the insertion and reverse have linear complexity
	//As a vector is not the fastest container for front insertion
	//And we can not note preallocation since we do not know the length of the path before iterating through it
	//It might be the best solution
	Point current = destination;
	do {
		path.push_back(current);
		current = cameFrom[current];
	} while (current != start);
	path.push_back(start);
	std::reverse(path.begin(), path.end());
	
	wasLastPathfindingSuccessful = true;
	return path;
}

bool Pathfinding::good() {
	return wasLastPathfindingSuccessful;
}

std::vector<Point> Pathfinding::getNeighbours(Point p) {
	//NOTE it does not check if the tiles are passable or not
	std::vector<Point> neighbours;
	
	//Would be segfault if the gameboard's width or height is only 1 tile
	if (Global::gameBoardWidth == 1 || Global::gameBoardHeight == 1) {
		throw std::runtime_error("Game board dimension can not be 1");
	}
	if (p.getX() == 0) {
		if (p.getY() == 0) {
			//Top left corner
			neighbours.push_back(p + Point(1, 0));
			neighbours.push_back(p + Point(0, 1));
			neighbours.push_back(p + Point(1, 1));
		} else if (p.getY() == Global::gameBoardHeight - 1) {
			//Bottom left corner
			neighbours.push_back(p + Point(1, 0));
			neighbours.push_back(p + Point(0, -1));
			neighbours.push_back(p + Point(1, -1));
		} else {
			//Left, but not corner
			neighbours.push_back(p + Point(0, -1));
			neighbours.push_back(p + Point(1, 0));
			neighbours.push_back(p + Point(0, 1));
			neighbours.push_back(p + Point(1, -1));
			neighbours.push_back(p + Point(1, 1));
		}
	} else if (p.getX() == Global::gameBoardWidth - 1) {
		if (p.getY() == 0) {
			//Top right corner
			neighbours.push_back(p + Point(-1, 0));
			neighbours.push_back(p + Point(0, 1));
			neighbours.push_back(p + Point(-1, 1));
		} else if (p.getY() == Global::gameBoardHeight - 1) {
			//Bottom right corner
			neighbours.push_back(p + Point(-1, 0));
			neighbours.push_back(p + Point(0, -1));
			neighbours.push_back(p + Point(-1, -1));
		} else {
			//Right, but not corner
			neighbours.push_back(p + Point(0, -1));
			neighbours.push_back(p + Point(-1, 0));
			neighbours.push_back(p + Point(0, 1));
			neighbours.push_back(p + Point(-1, -1));
			neighbours.push_back(p + Point(-1, 1));
		}
	} else {
		if (p.getY() == 0) {
			//Top, but not corner
			neighbours.push_back(p + Point(-1, 0));
			neighbours.push_back(p + Point(0, 1));
			neighbours.push_back(p + Point(1, 0));
			neighbours.push_back(p + Point(-1, 1));
			neighbours.push_back(p + Point(1, 1));
		} else if (p.getY() == Global::gameBoardHeight - 1) {
			//Bottom, but not corner
			neighbours.push_back(p + Point(-1, 0));
			neighbours.push_back(p + Point(0, -1));
			neighbours.push_back(p + Point(1, 0));
			neighbours.push_back(p + Point(-1, -1));
			neighbours.push_back(p + Point(1, -1));
		} else {
			//Not corner, not edge
			neighbours.push_back(p + Point(-1, 0));
			neighbours.push_back(p + Point(0, -1));
			neighbours.push_back(p + Point(0, 1));
			neighbours.push_back(p + Point(1, 0));
			neighbours.push_back(p + Point(1, 1));
			neighbours.push_back(p + Point(-1, -1));
			neighbours.push_back(p + Point(-1, 1));
			neighbours.push_back(p + Point(1, -1));
		}
	}
	
	return neighbours;
}

bool Pathfinding::wasLastPathfindingSuccessful = false;

bool PathPriority::operator()(PathEntry a, PathEntry b) {
	return (a.distance > b.distance);
}
