#include "worldobject.h"

#include "global.h"

WorldObject::WorldObject(std::string text, int x, int y) : WorldObject(text, Point(x, y)) {}

WorldObject::WorldObject(std::string text, Point pos) : MapEntity(pos) {
	texture = Global::resourceHandler->worldObjectTextures[text];
	atBackground = true;
}
