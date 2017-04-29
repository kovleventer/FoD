#include "interactiveworldobject.h"

#include "global.h"

/*!
 * @author kovlev
 */

InteractiveWorldObject::InteractiveWorldObject(std::string text, int x, int y, std::vector<Point> intTiles, bool isNeut) :
	InteractiveWorldObject(text, Point(x, y), intTiles, isNeut) {}

InteractiveWorldObject::InteractiveWorldObject(std::string text, Point pos, std::vector<Point> intTiles, bool isNeut) : WorldObject(text, pos) {
	interactiveTiles = intTiles;
	atBackground = false;
	neutral = isNeut;
	name = "";
	owner = NULL;
	
	gui = new InteractiveGUI(Global::permaGUI->getDim());
}

InteractiveWorldObject::~InteractiveWorldObject() {
	delete gui;
}

std::vector<Point> InteractiveWorldObject::getInteractiveTiles() {
	return interactiveTiles;
}

bool InteractiveWorldObject::isNeutral() {
	return neutral;
}

std::string InteractiveWorldObject::getName() {
	return name;
}

Character* InteractiveWorldObject::getOwner() {
	return owner;
}

InteractiveGUI* InteractiveWorldObject::getGUI() {
	return gui;
}

void InteractiveWorldObject::setName(std::string newName) {
	name = newName;
}

void InteractiveWorldObject::setOwner(Character* newOwner) {
	owner = newOwner;
}

void InteractiveWorldObject::activate() {
	//TODO implementation
	std::cout << name << " (owned by " << owner->getName() << ") activated" << std::endl;
	Global::guiHandler->setGUI(gui);
}
