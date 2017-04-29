#include "circularpath.h"

#include "pathfinding.h"

CircularPath::CircularPath(std::vector<Point> points) {
	//Init
	positionIndex = 0;
	generatePath(points);
}

Point CircularPath::next() {
	//If we are at the last point, we return the first
	if (positionIndex == path.size() - 1) {
		return path[0];
	}
	//Otherwise we return the next
	return path[positionIndex + 1];
}

Point CircularPath::current() {
	return path[positionIndex];
}

void CircularPath::moveForward() {
	if (positionIndex == path.size() - 1) {
		//If we are at the last point, we move to the first
		positionIndex = 0;
	} else {
		//Otherwise we move to the next
		positionIndex++;
	}
}

void CircularPath::generatePath(std::vector<Point> points) {
	for (unsigned int i = 0; i < points.size(); i++) {
		//Invokes pathfinding for every piece of path
		Point start = points[i];
		Point end = (i == points.size() - 1) ? points[0] : points[i + 1];
		std::vector<Point> tempPath = Pathfinding::findPath(start, end);
		//We merge those paths into one circular path
		path.insert(path.end(), tempPath.begin(), tempPath.end());
		//To prevent duplications
		path.pop_back();
	}
}
