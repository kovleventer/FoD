#include "npc.h"

#include "../core/resourcehandler.h"
#include "../core/global.h"
#include "../core/battle.h"
#include "../player/quest.h"

/*!
 * @author kovlev
 */

NPC::NPC(std::string text, int x, int y, NPCMovement nm) : NPC(text, Point(x, y), nm) {}

NPC::NPC(std::string text, Point pos, NPCMovement nm) : Character(pos) {
	texture = Global::resourceHandler->getATexture(TT::NPC, text);
	movementType = nm;
	if (movementType == NPCMovement::VISIT_OWN_STRUCT) {
		speed = 0.07 / Global::ticks * 64;
	}
	path = NULL;
	init();
}

NPC::NPC(std::string text, CircularPath* cp) : Character(cp->current()) {
	texture = Global::resourceHandler->getATexture(TT::NPC, text);
	movementType = NPCMovement::CIRCULATING;
	speed = 0.07 / Global::ticks * 64;
	path = cp;
	init();
}

NPC::~NPC() {
	if (!this->dead) {
		kill();
	}
}

void NPC::updateNPCPosition() {
	if (movementType == NPCMovement::STANDING) return;
	Point nextTile = path->next();
	Point currentTile = path->current();
	progressVector += PointD(nextTile - currentTile) * speed;
	
	//Calculating texture rotation
	calcRotation(nextTile - currentTile);
	
	if (fabs(progressVector.getX()) > 0.5 || fabs(progressVector.getY()) > 0.5) {
		//We can not simply change the positon, we need the update the references on the tile
		if (!positionSet) {
			setPositionAI(nextTile);
			positionSet = true;
		}
	} else {
		positionSet = false;
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
		Global::player->clearPath();
		Global::player->setState(PlayerState::BATTLING);
	}
}

NPCMovement NPC::getMovementType() {
	return movementType;
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

void NPC::setIsEnemy(bool newIsEnemy) {
	enemy = newIsEnemy;
}

void NPC::addOwned(InteractiveWorldObject* interactiveToAdd) {
	Character::addOwned(interactiveToAdd);
	if (movementType == NPCMovement::VISIT_OWN_STRUCT || temporaryStanding) {
		recalculatePathByOwned();
	}
}

void NPC::removeOwned(InteractiveWorldObject* interactiveToRemove) {
	Character::removeOwned(interactiveToRemove);
	if (movementType == NPCMovement::VISIT_OWN_STRUCT || temporaryStanding) {
		recalculatePathByOwned();
	}
}

void NPC::kill() {
	for (unsigned int i = 0; i < questObjectiveKills.size(); i++) {
		questObjectiveKills[i]->complete();
	}
	
	//Check is NOT done by the delete operation
	if (path != NULL)
		delete path;
	
	stdex::remove_value_vec(Global::npcHandler->npcs, this);
	
	delete army;
	delete inventory;
	
	//Code from MapEntity destructor
	if (Global::map != NULL) {
		Tile* thisTile = Global::map->getTile(position);
		if (thisTile != NULL) {
			if (atBackground) {
				stdex::remove_value_vec(thisTile->backgroundEntities, this);
			} else {
				stdex::remove_value_vec(thisTile->entities, this);
			}
		}
	}
	
	position = Point::INVALID;
	dead = true;
}

void NPC::rearrangeArmy() {
	std::priority_queue<Unit*, std::vector<Unit*>, UnitValueComparatorNoItems> meleeUnits;
	std::priority_queue<Unit*, std::vector<Unit*>, UnitValueComparatorNoItems> rangedUnits;
	//Removing all units while saving them and all their items
	for (int i = 0; i < army->getWidth(); i++) {
		for (int j = 0; j < army->getHeight(); j++) {
			Unit* currentUnit = army->getUnit(i, j);
			
			if (currentUnit != NULL) {
				for (int k = 0; k < currentUnit->getUnitInventorySize(); k++) {
					inventory->addItem(currentUnit->removeItem(k));
				}
				
				if (currentUnit->isDead()) {
					//Not preserving unit if dead
					delete currentUnit;
					army->setUnit(i, j, NULL);
					continue;
				}
				if (currentUnit->isMelee()) {
					meleeUnits.push(currentUnit);
				} else {
					rangedUnits.push(currentUnit);
				}
			}
		}
	}
	
	army->nullifyUnits();
	
	//Readding units to army
	//Choosing the most valuable units to the rows
	int pushableToMeleeCount = meleeUnits.size() < 4 ? meleeUnits.size() : 4;
	int pushableToRangedCount = rangedUnits.size() < 4 ? rangedUnits.size() : 4;
	for (int i = 0; i < pushableToMeleeCount; i++) {
		army->addUnit(meleeUnits.top());
		meleeUnits.pop();
	}
	for (int i = 0; i < pushableToRangedCount; i++) {
		army->addUnit(rangedUnits.top());
		rangedUnits.pop();
	}
	//Adding the rest to the support
	while (!meleeUnits.empty()) {
		army->addUnit(meleeUnits.top(), UnitAddingPreference::SUPPORTFIRST);
		meleeUnits.pop();
	}
	while (!meleeUnits.empty()) {
		army->addUnit(rangedUnits.top(), UnitAddingPreference::SUPPORTFIRST);
		rangedUnits.pop();
	}
	
	//TODO add items to units
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
	positionSet = false;
	temporaryStanding = false;
}

void NPC::setPositionAI(Point newPosition) {
	setPosition(newPosition);
	
	InteractiveWorldObject* currentInteractive = Global::map->getInteractiveOnTile(position);
	if (currentInteractive != NULL) {
		currentInteractive->activate(this);
	}
}

void NPC::recalculatePathByOwned() {
	std::vector<Point> ownedPoses;
	if (ownedBuildings.size() <= 1) {
		if (path != NULL) {
			delete path;
			path = NULL;
		}
		movementType = NPCMovement::STANDING;
		temporaryStanding = true;
		return;
	}
	
	temporaryStanding = false;
	movementType = NPCMovement::VISIT_OWN_STRUCT;
	
	for (unsigned int i = 0; i < ownedBuildings.size(); i++) {
		ownedPoses.push_back(ownedBuildings[i]->getPosition());
	}
	if (path != NULL) {
		delete path;
	}
	//TODO add temp path so that the npc can walk to its structure
	path = new CircularPath(ownedPoses);
}
