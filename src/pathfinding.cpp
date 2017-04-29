#include "pathfinding.h"

#include "global.h"
#include "tile.h"

std::vector<Point> Pathfinding::findPath(Point start, Point destination) {
	//See for resources:
	//http://www.redblobgames.com/pathfinding/a-star/introduction.html
	
	//We keep track the previos point
	std::map<Point, Point> cameFrom;
	cameFrom[start] = Point(-1, -1);
	
	//Tiles to check
	std::queue<Point> frontier;
	frontier.push(start);
	
	//ALERT infinity loop if not implemented properly
	while (!frontier.empty()) {
		//Store the first element and then remove it from the queue
		Point current = frontier.front();
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
				frontier.push(next);
				//std::cout << current << "<-" << next << std::endl;
				cameFrom[next] = current;
			}
		}
	}
	
	//Get the final path from the map
	Point current = destination;
	std::vector<Point> path;
	path.push_back(current);
	while (current != start) {
		current = cameFrom[current];
		path.push_back(current);
	}
	//path.push_back(start);
	std::reverse(path.begin(), path.end());
	
	return path;
}

std::vector<Point> Pathfinding::getNeighbours(Point p) {
	//NOTE it does not check if the tiles are passable or not
	std::vector<Point> neighbours;
	
	//ALERT segfault if the gameboard's width or height is only 1 tile
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
