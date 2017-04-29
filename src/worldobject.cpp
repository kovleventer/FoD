#include "worldobject.h"

#include "global.h"

WorldObject::WorldObject(int textID, Point pos, std::vector<Point> imp) : MapEntity(pos) {
	texture = Global::resourceHandler->worldObjectTextures[Global::resourceHandler->worldObjectTextureIDs[textID]];
	impassableTiles = imp;
}

std::vector<Point> WorldObject::getImpassableTiles() {
	return impassableTiles;
}
