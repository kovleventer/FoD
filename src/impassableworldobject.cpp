#include "impassableworldobject.h"

/*!
 * @author kovlev
 */

ImpassableWorldObject::ImpassableWorldObject(std::string text, int x, int y, std::vector<Point> impTiles) : ImpassableWorldObject(text, Point(x, y), impTiles) {}

ImpassableWorldObject::ImpassableWorldObject(std::string text, Point pos, std::vector<Point> impTiles) : WorldObject(text, pos) {
	impassableTiles = impTiles;
	atBackground = false;
}

std::vector<Point> ImpassableWorldObject::getImpassableTiles() {
	return impassableTiles;
}
