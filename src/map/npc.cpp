#include "npc.h"

#include "../core/resourcehandler.h"
#include "../core/global.h"
#include "../core/battle.h"

/*!
 * @author kovlev
 */

NPC::NPC(std::string text, int x, int y) : NPC(text, Point(x, y)) {}

NPC::NPC(std::string text, Point pos) : Character(pos) {
	texture = Global::resourceHandler->getATexture(TT::NPC, text);
	isStanding = true;
	path = NULL;
	init();
}

NPC::NPC(std::string text, std::vector<Point> pathPoints) : Character(pathPoints[0]) {
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
		Global::audioHandler->playSoundEffect("test");
		Battle* battle = new Battle(this);
		Global::player->setState(PlayerState::BATTLING);
		std::thread t([battle] {
			battle->start();
		});
		t.detach();
	}
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

bool NPC::isEnemy() {
	return enemy;
}

void NPC::setPath(CircularPath* newPath) {
	path = newPath;
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
