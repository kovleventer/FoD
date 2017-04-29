#include "npc.h"

#include "resourcehandler.h"
#include "global.h"
#include "battle.h"

NPC::NPC(std::string text, int x, int y) : NPC(text, Point(x, y)) {}

NPC::NPC(std::string text, Point pos) : MapEntity(pos) {
	texture = Global::resourceHandler->getATexture(TT::NPC, text);
	isStanding = true;
	path = NULL;
	init();
}

NPC::NPC(std::string text, std::vector<Point> pathPoints) : MapEntity(pathPoints[0]) {
	texture = Global::resourceHandler->getATexture(TT::NPC, text);
	isStanding = false;
	speed = 0.07 / Global::fps * 60;
	temporaryContainer = pathPoints;
	init();
}

NPC::~NPC() {
	//Check is NOT done by the delete operation
	if (path != NULL)
		delete path;
	
	Global::npcHandler->npcs.erase(std::remove(Global::npcHandler->npcs.begin(), Global::npcHandler->npcs.end(), this));
	
	delete army;
	delete inventory;
}

void NPC::updateNPCPosition() {
	if (isStanding) return;
	Point nextTile = path->next();
	Point currentTile = path->current();
	progressVector += PointD(nextTile - currentTile) * speed;
	
	//Calculating texture rotation
	calcRotation(nextTile - currentTile);
	
	if (fabs(progressVector.getX()) > 0.5 || fabs(progressVector.getY()) > 0.5) {
		//We can not simply change the positon, we need the update the references on the tile
		setPosition(nextTile);
	}
	
	if (fabs(progressVector.getX()) > 1 || fabs(progressVector.getY()) > 1) {
		path->moveForward();
		progressVector = PointD();
	}
}

void NPC::activate() {
	if (enemy) {
		Battle* battle = new Battle(this);
		Global::player->setState(PlayerState::BATTLING);
		std::thread t([battle] {
			battle->start();
		});
		t.detach();
	}
}

PointD NPC::getProgressVector() {
	return progressVector;
}

double NPC::getSpeed() {
	return speed;
}

bool NPC::getStanding() {
	return isStanding;
}

std::vector<Point> NPC::getTempCont() {
	return temporaryContainer;
}

CircularPath* NPC::getPath() {
	return path;
}

Army* NPC::getArmy() {
	return army;
}

Inventory* NPC::getInventory() {
	return inventory;
}

std::string NPC::getName() {
	return name;
}

bool NPC::isEnemy() {
	return enemy;
}

void NPC::setProgressVector(PointD newProgressVector) {
	progressVector = newProgressVector;
}

void NPC::setSpeed(double newSpeed) {
	speed = newSpeed;
}

void NPC::setPath(CircularPath* newPath) {
	path = newPath;
}

void NPC::setName(std::string newName) {
	name = newName;
}

void NPC::setIsEnemy(bool newIsEnemy) {
	enemy = newIsEnemy;
}

void NPC::init() {
	army = new Army(Global::permaGUI->getDim().x + Global::permaGUI->getDim().w / 5,
					Global::permaGUI->getDim().y,
					Global::permaGUI->getDim().w * 4 / 5,
					Global::permaGUI->getDim().h / 2,
					5, 2, true);
	enemy = true;
	atBackground = false;
	inventory = new Inventory(20);
}

void NPC::calcRotation(Point pRot) {
	//NOTE you can the find the same exact code at player
	//So its code duplication
	if (pRot == Point(1, -1)) {
		//Up right
		texture->setRotation(RotationType::UPRIGHT);
	} else if (pRot == Point(1, 0)) {
		//Right
		texture->setRotation(RotationType::RIGHT);
	} else if (pRot == Point(1, 1)) {
		//Down right
		texture->setRotation(RotationType::DOWNRIGHT);
	} else if (pRot == Point(0, 1)) {
		//Down
		texture->setRotation(RotationType::DOWN);
	} else if (pRot == Point(-1, 1)) {
		//Down left
		texture->setRotation(RotationType::DOWNLEFT);
	} else if (pRot == Point(-1, 0)) {
		//Left
		texture->setRotation(RotationType::LEFT);
	} else if (pRot == Point(-1, -1)) {
		//Up left
		texture->setRotation(RotationType::UPLEFT);
	} else if (pRot == Point(0, -1)) {
		//Up
		texture->setRotation(RotationType::UP);
	} else {
		std::clog << "Warning: Rotation calculating bug" << std::endl;
	}
}
