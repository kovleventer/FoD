#include "worldobject.h"

#include "../core/global.h"

/*!
 * @author kovlev
 */

// WorldObject stuff

WorldObject::WorldObject(std::string text, int x, int y) : WorldObject(text, Point(x, y)) {}

WorldObject::WorldObject(std::string text, Point pos) : MapEntity(pos) {
	texture = Global::resourceHandler->getATexture(TT::WORLDOBJECT, text);
	atBackground = true;
}


// ImpassableWorldObject stuff

ImpassableWorldObject::ImpassableWorldObject(std::string text, int x, int y, std::vector<Point> impTiles) : ImpassableWorldObject(text, Point(x, y), impTiles) {}

ImpassableWorldObject::ImpassableWorldObject(std::string text, Point pos, std::vector<Point> impTiles) : WorldObject(text, pos) {
	impassableTiles = impTiles;
	atBackground = false;
}

std::vector<Point> ImpassableWorldObject::getImpassableTiles() {
	return impassableTiles;
}


// InteractiveWorldObject stuff

InteractiveWorldObject::InteractiveWorldObject(std::string text, int x, int y, std::vector<Point> intTiles, bool isNeut) :
	InteractiveWorldObject(text, Point(x, y), intTiles, isNeut) {}

InteractiveWorldObject::InteractiveWorldObject(std::string text, Point pos, std::vector<Point> intTiles, bool isNeut) : WorldObject(text, pos) {
	interactiveTiles = intTiles;
	atBackground = false;
	neutral = isNeut;
	name = "";
	owner = NULL;
	
	gui = new InteractiveGUI(Global::permaGUI->getDim());
	gui->setParent(this);
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
	if (owner->getName() == "__takeable__") {
		owner = Global::player;
	}
	
	if (dynamic_cast<NPC*>(owner) != NULL && dynamic_cast<NPC*>(owner)->isEnemy()) {
		std::cout << "Battle with structures is not yet implemented" << std::endl;
	} else {
		Global::guiHandler->setGUI(gui);
	}
}
