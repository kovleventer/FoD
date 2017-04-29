#include "battle.h"

#include "global.h"
#include "game.h"

Battle::Battle(NPC* npc1, NPC* npc2) {
	//TODO implementation of battle between two npcs
	gui = NULL;
}

Battle::Battle(NPC* e) {
	player = Global::player;
	enemy = e;
	currentAttackingUnit = NULL;
}

Battle::~Battle() {
	//Deleting the gui is now safe
	delete gui;
	
	Global::player->getArmy()->getUnitInfo()->setUnit(NULL);
}

void Battle::start() {
	//Intializing the battle
	gui = new WholeScreenGUI(Global::permaGUI->getDim());
	gui->addPart(player->getArmy());
	gui->addPart(enemy->getArmy());
	gui->addPart(player->getArmy()->getUnitInfo());
	
	//Setting this battle as current gui
	Global::guiHandler->setGUI(gui);
	Global::guiHandler->setBattle(this);
	
	maxTurns = 100;
	currentTurn = 1;
	
	playerUnitCount = 0;
	enemyUnitCount = 0;
	
	continueBattle();
}

void Battle::continueBattle() {
	
	removeUnitFromQueue(currentAttackingUnit);
	
	while (currentTurn < maxTurns) {
		
		if (speedPQ.empty()) {
			currentTurn++;
			
			playerUnitCount = 0;
			enemyUnitCount = 0;
			
			//We add all units in the battlefield into the queue
			for (int j = 0; j < player->getArmy()->getWidth(); j++) {
				for (int k = 0; k < player->getArmy()->getHeight(); k++) {
					Unit* tempPlayerUnit = player->getArmy()->getUnit(j, k);
					Unit* tempEnemyUnit = enemy->getArmy()->getUnit(j, k);
					
					if (tempPlayerUnit != NULL && !tempPlayerUnit->isDead()) {
						tempPlayerUnit->setTeamOne(true);
						tempPlayerUnit->statsWithItems["currentNumberOfActions"] = tempPlayerUnit->statsWithItems["numberOfActions"];
						speedPQ.push(tempPlayerUnit);
						playerUnitCount++;
					}
					if (tempEnemyUnit != NULL && !tempEnemyUnit->isDead()) {
						tempEnemyUnit->setTeamOne(false);
						tempEnemyUnit->statsWithItems["currentNumberOfActions"] = tempEnemyUnit->statsWithItems["numberOfActions"];
						speedPQ.push(tempEnemyUnit);
						enemyUnitCount++;
					}
				}
			}
		}
		
		if (playerUnitCount == 0) {
			enemy->getArmy()->finalizeUnitExperiences();
			Popup* popup = new Popup(800, 400, PopupType::POPUP_OK);
			popup->setText("You died");
			popup->buttonOK->setOnClick(Game::quit);
			Global::guiHandler->clear();
			Global::guiHandler->setGUI(popup);
			delete this;
			return;
		}
		if (enemyUnitCount == 0) {
			player->getArmy()->finalizeUnitExperiences();
			Global::guiHandler->clear();
			
			
			Popup* popup = new Popup(800, 400, PopupType::POPUP_OK);
			int acquiredItems = 0;
			//Adding enemy inventory
			for (unsigned int i = 0; i < enemy->getInventory()->getInventorySize(); i++) {
				Item* currentItem = enemy->getInventory()->getItem(i);
				if (currentItem != NULL) {
					popup->addItem(currentItem);
					acquiredItems++;
				}
			}
			
			for (int i = 0; i < enemy->getArmy()->getWidth(); i++) {
				for (int j = 0; j < enemy->getArmy()->getHeight(); j++) {
					Unit* tempEnemyUnit = enemy->getArmy()->getUnit(i, j);
					if (tempEnemyUnit != NULL) {
						for (int k = 0; k < tempEnemyUnit->getUnitInventorySize(); k++) {
							Item* currentItem = tempEnemyUnit->getItem(k);
							if (currentItem != NULL) {
								popup->addItem(currentItem);
								acquiredItems++;
							}
						}
					}
					
					//NOTE completely irrelevant stuff
					Unit* tempPlayerUnit = player->getArmy()->getUnit(i, j);
					if (tempPlayerUnit != NULL) {
						tempPlayerUnit->recalculateInventory();
					}
				}
			}
			
			for (int i = 0; i < acquiredItems; i++) {
				player->getInventory()->addItem(popup->getItem(i));
			}
			
			Global::guiHandler->clear();
			Global::guiHandler->setGUI(popup);
			
			delete enemy;
			enemy = NULL;
			
			delete this;
			return;
		}
		
		while(!speedPQ.empty()) {
			Unit* currentUnit = speedPQ.top();
			if (currentUnit == NULL || currentUnit->isDead()) {
				speedPQ.pop();
				continue;
			}
			
			if (currentUnit->getTeamOne()) {
				//If a player-controlled unit is attacking
				player->getArmy()->getUnitInfo()->setUnit(currentUnit);
				player->getArmy()->setSelectedUnitPos(currentUnit->getPosition());
				
				//removeUnitFromQueue(currentUnit);
				
				player->getArmy()->setAllowAttack(true);
				
				//Setting flags based on enemy
				unsigned char flags = Army::ALLOW_FRONTLINE;
				if (enemy->getArmy()->isFrontRowEmpty()) {
					flags |= Army::ALLOW_BACKLINE;
				}
				
				//Setting flags based on current attacking unit's position
				switch(player->getArmy()->getUPFromPos(currentUnit->getPosition())) {
					case UnitPosition::BACKROW:
						if (currentUnit->isRanged()) {
							flags |= Army::ALLOW_BACKLINE;
						} else {
							if (!player->getArmy()->isFrontRowEmpty()) {
								flags = Army::DISALLOW_ALL;
							}
						}
						break;
					case UnitPosition::SUPPORT:
						flags = Army::DISALLOW_ALL;
						break;
					case UnitPosition::FRONTROW:
						break;
				}
				
				enemy->getArmy()->setAttackRestrictionFlags(flags);
				
				enemy->getArmy()->setAllowAttack(true);
				currentAttackingUnit = currentUnit;
				return;
			} else {
				//If the enemy attacks
				currentAttackingUnit = currentUnit;
				
				if (enemy->getArmy()->getUPFromPos(currentUnit->getPosition()) == UnitPosition::SUPPORT) {
					//Places a unit sitting in support to an open position if possible
					Point openPosition;
					if (currentUnit->isMelee()) {
						openPosition = enemy->getArmy()->getFirstOpenFrontRowPosition();
					} else {
						openPosition = enemy->getArmy()->getFirstOpenBackRowPosition();
					}
					if (openPosition != Point(-1, -1)) {
						enemy->getArmy()->switchUnits(currentUnit->getPosition(), openPosition);
					}
					
					removeUnitFromQueue(currentUnit);
					
					continue;
				}
				
				std::vector<Unit*> attackableUnits;
				
				switch (enemy->getArmy()->getUPFromPos(currentUnit->getPosition())) {
					case UnitPosition::BACKROW:
						if (currentUnit->isRanged()) {
							//Ranged attack case
							
							//Adding front and back row
							for (int j = 0; j <= 1; j++) {
								for (int i = 1; i < player->getArmy()->getWidth() - 1; i++) {
									Unit* possibleAttackableUnit = player->getArmy()->getUnit(i, j);
									if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
										attackableUnits.push_back(possibleAttackableUnit);
									}
								}
							}
						} else {
							//Melee attack case
							
							//Adding front row
							for (int i = 1; i < player->getArmy()->getWidth() - 1; i++) {
								Unit* possibleAttackableUnit = player->getArmy()->getUnit(i, 0);
								if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
									attackableUnits.push_back(possibleAttackableUnit);
								}
							}
							
							if (player->getArmy()->isFrontRowEmpty()) {
								//If front row is empty, adding back row
								for (int i = 1; i < player->getArmy()->getWidth() - 1; i++) {
									Unit* possibleAttackableUnit = player->getArmy()->getUnit(i, 1);
									if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
										attackableUnits.push_back(possibleAttackableUnit);
									}
								}
							}
						}
						
						//If front&back row is empty, then we add support too
						if (player->getArmy()->areFrontAndBackRowsEmpty()) {
							std::vector<Point> supportPoints = {Point(0, 0), Point(0, 1),
								Point(player->getArmy()->getWidth() - 1, 0),
								Point(player->getArmy()->getWidth() - 1, 1)};
							
							for (unsigned int i = 0; i < supportPoints.size(); i++) {
								Unit* possibleAttackableUnit = player->getArmy()->getUnit(supportPoints[i]);
								if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
									attackableUnits.push_back(possibleAttackableUnit);
								}
							}
						}
						break;
					case UnitPosition::SUPPORT:
						break;
					case UnitPosition::FRONTROW:
						//Adding front row
						for (int i = 1; i < player->getArmy()->getWidth() - 1; i++) {
							Unit* possibleAttackableUnit = player->getArmy()->getUnit(i, 0);
							if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
								attackableUnits.push_back(possibleAttackableUnit);
							}
						}
						
						if (player->getArmy()->isFrontRowEmpty()) {
							//If front row is empty, adding back row
							for (int i = 1; i < player->getArmy()->getWidth() - 1; i++) {
								Unit* possibleAttackableUnit = player->getArmy()->getUnit(i, 1);
								if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
									attackableUnits.push_back(possibleAttackableUnit);
								}
							}
						}
						
						//If front&back row is empty, then we add support too
						if (player->getArmy()->areFrontAndBackRowsEmpty()) {
							std::vector<Point> supportPoints = {Point(0, 0), Point(0, 1),
								Point(player->getArmy()->getWidth() - 1, 0),
								Point(player->getArmy()->getWidth() - 1, 1)};
							
							for (unsigned int i = 0; i < supportPoints.size(); i++) {
								Unit* possibleAttackableUnit = player->getArmy()->getUnit(supportPoints[i]);
								if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
									attackableUnits.push_back(possibleAttackableUnit);
								}
							}
						}
						break;
				}
				
				if (attackableUnits.size() != 0) {
					//Maximum finding
					double maxDamageRatio = 0;
					int maxDamageRatioIndex = -1;
					
					for (unsigned int i = 0; i < attackableUnits.size(); i++) {
						int damage = getPossibleDamage(attackableUnits[i]);
						int enemyAttack = attackableUnits[i]->getAttack();
						int enemyHP = attackableUnits[i]->statsWithItems["currentLife"];
						double ratio = (double)damage * enemyAttack / enemyHP;
						if (ratio > maxDamageRatio) {
							maxDamageRatio = ratio;
							maxDamageRatioIndex = i;
						}
					}
					
					int damage = getPossibleDamage(attackableUnits[maxDamageRatioIndex]);
					dealDamage(attackableUnits[maxDamageRatioIndex], damage, false);
				}
				
				removeUnitFromQueue(currentUnit);
			}
		}
	}
}

int Battle::getPossibleDamage(Unit* unitToAttack) {
	//Calculating damage
	int attackValue = currentAttackingUnit->getAttack();
	int defenseValue = unitToAttack->getDefense(currentAttackingUnit);
	
	if (attackValue <= defenseValue) {
		attackValue = 1;
	} else {
		attackValue -= defenseValue;
	}
	
	if (attackValue > unitToAttack->statsWithItems["currentLife"]) {
		attackValue = unitToAttack->statsWithItems["currentLife"];
	}
	
	return attackValue;
}

void Battle::dealDamage(Unit* unitToAttack, int damage, bool isContinuation) {
	unitToAttack->statsWithItems["currentLife"] -= damage;
	unitToAttack->stats["currentLife"] -= damage;
	
	if (unitToAttack->statsWithItems["currentLife"] <= 0) {
		unitToAttack->statsWithItems["currentLife"] = 0;
		if (unitToAttack->getTeamOne()) {
			playerUnitCount--;
		} else {
			enemyUnitCount--;
		}
		unitToAttack->kill();
	}
	
	currentAttackingUnit->addTempXP(damage);
	
	if (isContinuation) {
		continueBattle();
	}
}

int Battle::getMaxTurns() {
	return maxTurns;
}

int Battle::getCurrentTurn() {
	return currentTurn;
}

void Battle::removeUnitFromQueue(Unit* unitToRemove) {
	if (unitToRemove == NULL) return;
	
	if (unitToRemove->statsWithItems["currentNumberOfActions"] <= 1) {
		if (!speedPQ.empty()) {
			speedPQ.pop();
		}
	}
	unitToRemove->statsWithItems["currentNumberOfActions"]--;
}
