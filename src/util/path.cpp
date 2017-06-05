#include "path.h"

#include "pathfinding.h"

/*!
 * @author kovlev
 */


// BasePath stuff

BasePath::BasePath()  {
	positionIndex = 0;
}

BasePath::~BasePath() {}


// SimplePath stuff

SimplePath::SimplePath(Point start, Point end) : BasePath() {
	path = Pathfinding::findPath(start, end);
}

Point SimplePath::next() {
	if (positionIndex >= path.size() - 1) {
		return Point::INVALID;
	}
	return path[positionIndex + 1];
}

Point SimplePath::current() {
	return path[positionIndex];
}

bool SimplePath::isAtFirst() {
	return positionIndex == 0;
}

bool SimplePath::isAtLast() {
	return positionIndex == path.size() - 1;
}

void SimplePath::moveForward() {
	if (!isAtLast()) {
		positionIndex++;
	}
}


// CircularPath stuff

CircularPath::CircularPath(std::vector<Point> points) : BasePath() {
	//Init
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

//Since this type of path is circular, first and last points make no sense
bool CircularPath::isAtFirst() {
	return false;
}

bool CircularPath::isAtLast() {
	return false;
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
