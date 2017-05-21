#include "npc.h"

#include "../core/resourcehandler.h"
#include "../core/global.h"
#include "../core/battle.h"
#include "../player/quest.h"

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
	speed = 0.07 / Global::ticks * 64;
	temporaryContainer = pathPoints;
	init();
}

NPC::~NPC() {
	if (!this->dead) {
		kill();
	}
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
	//Handling quests
	for (unsigned int i = 0; i < questObjectiveTalks.size(); i++) {
		questObjectiveTalks[i]->complete();
	}
	for (unsigned int i = 0; i < questTriggerTalks.size(); i++) {
		questTriggerTalks[i]->start();
	}
	
	if (enemy) {
		Global::audioHandler->playSoundEffect("test");
		//The battle handles itself
		new Battle(this);
		Global::player->setState(PlayerState::BATTLING);
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

bool NPC::isDead() {
	return dead;
}

void NPC::setPath(CircularPath* newPath) {
	path = newPath;
}

void NPC::setIsEnemy(bool newIsEnemy) {
	enemy = newIsEnemy;
}

void NPC::kill() {
	for (unsigned int i = 0; i < questObjectiveKills.size(); i++) {
		questObjectiveKills[i]->complete();
	}
	
	//Check is NOT done by the delete operation
	if (path != NULL)
		delete path;
	
	Global::npcHandler->npcs.erase(std::remove(Global::npcHandler->npcs.begin(), Global::npcHandler->npcs.end(), this));
	
	delete army;
	delete inventory;
	
	dead = true;
}

void NPC::addQuestTriggerTalk(Quest* questTriggerTalkToAdd) {
	questTriggerTalks.push_back(questTriggerTalkToAdd);
}

void NPC::addQuestObjectiveTalk(Quest* questObjectiveTalkToAdd) {
	questObjectiveTalks.push_back(questObjectiveTalkToAdd);
}

void NPC::addQuestObjectiveKill(Quest* questObjectiveKillToAdd) {
	questObjectiveKills.push_back(questObjectiveKillToAdd);
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
	dead = false;
}
