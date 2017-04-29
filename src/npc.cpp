#include "npc.h"

#include "resourcehandler.h"
#include "global.h"
#include "battle.h"

NPC::NPC(std::string text, int x, int y) : NPC(text, Point(x, y)) {}

NPC::NPC(std::string text, Point pos) : MapEntity(pos) {
	texture = Global::resourceHandler->npcTextures[text];
	isStanding = true;
	path = NULL;
	init();
}

NPC::NPC(std::string text, std::vector<Point> pathPoints) : MapEntity(pathPoints[0]) {
	texture = Global::resourceHandler->npcTextures[text];
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
}

void NPC::updateNPCPosition() {
	if (isStanding) return;
	Point nextTile = path->next();
	Point currentTile = path->current();
	progressVector += PointD(nextTile - currentTile) * speed;
	
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
	
		battle->start();
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
}
