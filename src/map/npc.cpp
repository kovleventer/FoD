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
	
	//NOTE this will be very slow since it runs n^2 times
	//Using an optimized solution requires further reading from me
	if (movementType == NPCMovement::VISIT_OWN_STRUCT) {
		
		if (followed == NULL) {
			
			//Player checking
			if (enemy &&
				Global::player->getPosition().distanceTo(position) < aggroRange &&
				Global::player->getArmy()->getArmyValue(false) < army->getArmyValue(false)) {
				
				followed = Global::player;
				
				if (dynamic_cast<CircularPath*>(path) != NULL) {
					//IF PATH IS CIRCULARPATH
					//Does not delete nextpath (might be unsafe)
					nextPath = path;
				} else {
					//IF PATH IS SIMPLEPATH
					delete path;
				}
				path = new SimplePath(position, followed->getPosition());
			}
			
			//NPC checking
			for (unsigned int i = 0; i < Global::npcHandler->npcs.size(); i++) {
				if (Global::npcHandler->npcs[i] == this) continue;
				
				if (Global::npcHandler->npcs[i]->enemy != enemy) {
					//If the two npcs are hostile towards each other
					
					if (Global::npcHandler->npcs[i]->getPosition().distanceTo(position) < aggroRange) {
						//If npcs are close enough to each other
						
						if (Global::npcHandler->npcs[i]->getArmy()->getArmyValue(false) < army->getArmyValue(false)) {
							//If the enemy is weaker
							
							if (Global::npcHandler->npcs[i]->getPosition() == position) {
								//Activating npc
								Global::npcHandler->npcs[i]->activate(this);
							} else {
								
								followed = Global::npcHandler->npcs[i];
								
								
								if (dynamic_cast<CircularPath*>(path) != NULL) {
									//IF PATH IS CIRCULARPATH
									//Does not delete nextpath (might be unsafe)
									nextPath = path;
								} else {
									//IF PATH IS SIMPLEPATH
									delete path;
								}
								path = new SimplePath(position, followed->getPosition());
							}
						}
					}
				}
			}
		} else {
			if (followed->getPosition().distanceTo(position) >= aggroRange) {
				followed = NULL;
				
				setupPath();
			} else if (followed->getPosition() == position) {
				NPC* followNPC = dynamic_cast<NPC*>(followed);
				if (followNPC != NULL) {
					followNPC->activate(this);
				} else if (Global::player == followed) {
					activate();
				}
				
				setupPath();
			}
		}
	}
	
	
	Point nextTile = path->next();
	Point currentTile = path->current();
	progressVector += PointD(nextTile - currentTile) * speed;
	
	//Calculating texture rotation
	calcRotation(nextTile - currentTile);
	
	if (fabs(progressVector.getX()) > 0.5 || fabs(progressVector.getY()) > 0.5) {
	//if (fabs(progressVector.getX()) > 1 || fabs(progressVector.getY()) > 1) {
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
		if (path->isAtLast()) {
			delete path;
			path = nextPath;
			nextPath = NULL;
		}
		progressVector = PointD();
		if (followed != NULL) {
			delete path;
			path = new SimplePath(position, followed->getPosition());
		}
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

void NPC::activate(NPC* npc) {
	if (enemy != npc->enemy) {
		//Quick battle between npcs
		new Battle(this, npc);
		followed = NULL;
	}
}

NPCMovement NPC::getMovementType() {
	return movementType;
}

BasePath* NPC::getPath() {
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
	if (path != NULL) {
		delete path;
		path = NULL;
	}
	if (nextPath != NULL) {
		delete nextPath;
		nextPath = NULL;
	}
	
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
	
	//Add items to units
	for (unsigned int i = 0; i < inventory->getInventorySize(); i++) {
		Item* currentItem = inventory->getItem(i);
		if (currentItem != NULL) {
			for (int j = 0; j < army->getWidth(); j++) {
				for (int k = 0; k < army->getHeight(); k++) {
					Unit* currentUnit = army->getUnit(j, k);
					if (currentUnit != NULL && !currentUnit->isDead() && currentUnit->addItem(currentItem)) {
						inventory->setItem(i, NULL);
						//>:(
						goto cont;
					}
				}
			}
		}
		//>:(
		cont: ;
	}
	
	for (int i = 0; i < army->getWidth(); i++) {
		for (int j = 0; j < army->getHeight(); j++) {
			Unit* currentUnit = army->getUnit(i, j);
			
			if (currentUnit != NULL) {
				currentUnit->recalculateInventory();
			}
		}
	}
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
	nextPath = NULL;
	enemy = true;
	atBackground = false;
	inventory = new Inventory(20);
	dead = false;
	positionSet = false;
	temporaryStanding = false;
	followed = NULL;
	aggroRange = 10;
}

void NPC::setPositionAI(Point newPosition) {
	setPosition(newPosition);
	
	InteractiveWorldObject* currentInteractive = Global::map->getInteractiveOnTile(position);
	if (currentInteractive != NULL) {
		currentInteractive->activate(this);
	}
}

void NPC::recalculatePathByOwned() {
	if (dead) return;
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
	
	setupPath();
}

void NPC::setupPath() {
	//This is not the fastest method so don't call it too often
	std::vector<Point> ownedPoses;
	
	for (unsigned int i = 0; i < ownedBuildings.size(); i++) {
		ownedPoses.push_back(ownedBuildings[i]->getPosition());
	}
	
	if (Character::characterPlaceholderNeutral != NULL) {
		for (unsigned int i = 0; i < Character::characterPlaceholderNeutral->getOwnedBuildings().size(); i++) {
			ownedPoses.push_back(Character::characterPlaceholderNeutral->getOwnedBuildings()[i]->getPosition());
		}
	}
	
	if (path != NULL) {
		delete path;
		path = NULL;
	}
	if (nextPath != NULL) {
		delete nextPath;
		nextPath = NULL;
	}
	
	CircularPath* tempCP = new CircularPath(ownedPoses);
	if (tempCP->current() == position) {
		path = tempCP;
	} else {
		nextPath = tempCP;
		path = new SimplePath(position, tempCP->current());
	}
}
