#include "interactiveworldobject.h"

InteractiveWorldObject::InteractiveWorldObject(std::string text, int x, int y, std::vector<Point> intTiles) : InteractiveWorldObject(text, Point(x, y), intTiles) {}

InteractiveWorldObject::InteractiveWorldObject(std::string text, Point pos, std::vector<Point> intTiles) : WorldObject(text, pos) {
	interactiveTiles = intTiles;
	atBackground = false;
}

std::vector<Point> InteractiveWorldObject::getInteractiveTiles() {
	return interactiveTiles;
}
