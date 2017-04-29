#include "interactiveworldobject.h"

#include "global.h"

InteractiveWorldObject::InteractiveWorldObject(int textID, Point pos, std::vector<Point> inter) : MapEntity(pos) {
	texture = Global::resourceHandler->interactiveWorldObjectTextures[Global::resourceHandler->interactiveWorldObjectTextureIDs[textID]];
	interactiveTiles = inter;
}

InteractiveWorldObject::~InteractiveWorldObject() {}

std::vector<Point> InteractiveWorldObject::getInteractiveTiles() {
	return interactiveTiles;
}
