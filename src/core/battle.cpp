#include "battle.h"

#include "global.h"
#include "game.h"

/*!
 * @author kovlev
 */

Battle::Battle(NPC* n1, NPC* n2) {
	gui = NULL;
	npc1 = n1;
	npc2 = n2;
	battleType = BattleType::NPC_NPC;
	army1 = npc1->getArmy();
	army2 = npc2->getArmy();
	
	quickBattle();
	delete this;
}

Battle::Battle(NPC* n1, InteractiveWorldObject* iwo) {
	gui = NULL;
	npc1 = n1;
	structToFight = iwo;
	battleType = BattleType::NPC_STRUCT;
	army1 = npc1->getArmy();
	army2 = structToFight->getGarrisonArmy();
	
	quickBattle();
	delete this;
}

Battle::Battle(NPC* e) {
	player = Global::player;
	enemy = e;
	enemyArmy = enemy->getArmy();
	battleType = BattleType::PLAYER_NPC;
	
	initGUIBattle();
}

Battle::Battle(InteractiveWorldObject* iwo) {
	player = Global::player;
	enemyStruct = iwo;
	enemyArmy = enemyStruct->getGarrisonArmy();
	battleType = BattleType::PLAYER_STRUCT;
	
	initGUIBattle();
}

Battle::~Battle() {
	if (battleType == BattleType::PLAYER_NPC || battleType == BattleType::PLAYER_STRUCT) {
		//Deleting the gui is now safe
		delete gui;
		gui = NULL;
		Global::player->getArmy()->getUnitInfo()->setUnit(NULL);
		Global::player->setState(PlayerState::STANDING);
	}
}

void Battle::render() {
	if (this == NULL) {
		return;
	}
	gui->render();
	
	if (attackTexture != NULL) {
		//Setting rectangle
		SDL_Rect destinationRect;
		int dimParam = Global::player->getArmy()->getUnitSize();
		destinationRect.x = attackTexturePosition.getX() - dimParam / 2;
		destinationRect.y = attackTexturePosition.getY() - dimParam / 2;
		destinationRect.w = dimParam;
		destinationRect.h = dimParam;
		attackTexture->render(destinationRect);
	}
}

void Battle::start() {
	continueBattle();
}

void Battle::continueBattle() {
	if (Battle::isMainLoopEnded) {
		delete this;
		return;
	}
	
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
					Unit* tempEnemyUnit = enemyArmy->getUnit(j, k);
					
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
			//If player lost
			enemyArmy->finalizeUnitExperiences();
			Popup* popup = new Popup(Popup::DEFAULT_DIM, PopupType::POPUP_OK);
			popup->setText("You died");
			popup->buttonOK->setOnClick(Game::quit);
			Global::guiHandler->clear();
			Global::guiHandler->addPopup(popup);
			
			delete this;
			return;
		}
		if (enemyUnitCount == 0) {
			//If player won
			player->getArmy()->finalizeUnitExperiences();
			player->getArmy()->setAllowAttack(false);
			Global::guiHandler->clear();
			
			
			Popup* popup = new Popup(Popup::DEFAULT_DIM, PopupType::POPUP_OK);
			int acquiredItems = 0;
			//Adding enemy inventory
			if (battleType == BattleType::PLAYER_NPC) {
				//Adding gold
				player->giveGold(enemy->getGold());
				for (unsigned int i = 0; i < enemy->getInventory()->getInventorySize(); i++) {
					Item* currentItem = enemy->getInventory()->getItem(i);
					if (currentItem != NULL) {
						popup->addItem(currentItem);
						acquiredItems++;
					}
				}
			}
			
			for (int i = 0; i < enemyArmy->getWidth(); i++) {
				for (int j = 0; j < enemyArmy->getHeight(); j++) {
					Unit* tempEnemyUnit = enemyArmy->getUnit(i, j);
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
			Global::guiHandler->addPopup(popup);
			Global::guiHandler->setBattle(NULL);
			
			if (battleType == BattleType::PLAYER_NPC) {
				enemy->kill();
			}
			if (battleType == BattleType::PLAYER_STRUCT) {
				enemyStruct->setOwner(Global::player);
				enemyStruct->clearGarrisonArmy();
			}
			
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
				
				player->getArmy()->setAllowAttack(true);
				enemyArmy->setAllowAttack(true);
				
				//Setting flags based on enemy
				unsigned char flags = Army::ALLOW_FRONTLINE;
				if (enemyArmy->isFrontRowEmpty()) {
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
				
				enemyArmy->setAttackRestrictionFlags(flags);
				
				enemyArmy->setAllowAttack(true);
				currentAttackingUnit = currentUnit;
				
				return;
			} else {
				aiWhichUnitToAttack(currentUnit, enemyArmy, player->getArmy());
			}
		}
	}
}

int Battle::getPossibleDamage(Unit* unitToAttack) {
	if (currentAttackingUnit == NULL) return 0;
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
	if (battleType == BattleType::PLAYER_NPC || battleType == BattleType::PLAYER_STRUCT) {
		//Animation settings
		//Wew thats ugly
		Point startCoord, endCoord;
		int sourcePosX = currentAttackingUnit->getPosition().getX();
		int sourcePosY = (!isContinuation) ? (currentAttackingUnit->getPosition().getY() == 0 ? 1 : 0) : currentAttackingUnit->getPosition().getY();
		int destPosX = unitToAttack->getPosition().getX();
		int destPosY = isContinuation ? (unitToAttack->getPosition().getY() == 0 ? 1 : 0) : unitToAttack->getPosition().getY();
		if (isContinuation) {
			startCoord = Point(player->getArmy()->getX()
					+ player->getArmy()->getPaddingH() * (sourcePosX + 1)
					+ player->getArmy()->getUnitSize() * sourcePosX
					+ player->getArmy()->getUnitSize() / 2,
				player->getArmy()->getY()
					+ player->getArmy()->getPaddingV() * (sourcePosY + 1)
					+ player->getArmy()->getUnitSize() * sourcePosY
					+ player->getArmy()->getUnitSize() / 2);
			endCoord = Point(enemyArmy->getX()
					+ enemyArmy->getPaddingH() * (destPosX + 1)
					+ enemyArmy->getUnitSize() * destPosX
					+ enemyArmy->getUnitSize() / 2,
				enemyArmy->getY()
					+ enemyArmy->getPaddingV() * (destPosY + 1)
					+ enemyArmy->getUnitSize() * destPosY
					+ enemyArmy->getUnitSize() / 2);
		} else {
			startCoord = Point(enemyArmy->getX()
					+ enemyArmy->getPaddingH() * (sourcePosX + 1)
					+ enemyArmy->getUnitSize() * sourcePosX
					+ enemyArmy->getUnitSize() / 2,
				enemyArmy->getY()
					+ enemyArmy->getPaddingV() * (sourcePosY + 1)
					+ enemyArmy->getUnitSize() * sourcePosY
					+ enemyArmy->getUnitSize() / 2);
			endCoord = Point(player->getArmy()->getX()
					+ player->getArmy()->getPaddingH() * (destPosX + 1)
					+ player->getArmy()->getUnitSize() * destPosX
					+ player->getArmy()->getUnitSize() / 2,
				player->getArmy()->getY()
					+ player->getArmy()->getPaddingV() * (destPosY + 1)
					+ player->getArmy()->getUnitSize() * destPosY
					+ player->getArmy()->getUnitSize() / 2);
		}
		
		
		switch (currentAttackingUnit->getUnitType()) {
			case UnitType::FIGHTER:
				attackTexture = Global::resourceHandler->getATexture(TT::GUI, "attack_fighter");
				break;
			case UnitType::ARCHER:
				attackTexture = Global::resourceHandler->getATexture(TT::GUI, "attack_archer");
				break;
			case UnitType::MAGE:
				attackTexture = Global::resourceHandler->getATexture(TT::GUI, "attack_mage");
				break;
			case UnitType::OTHER:
				attackTexture = Global::resourceHandler->getATexture(TT::GUI, "attack_other");
				break;
		}
		
		Global::tickHandler->animateBattleAction(startCoord, endCoord);
		
		
		
		
		enemyArmy->setAllowAttack(false);
		player->getArmy()->setAllowAttack(false);
		player->getArmy()->setSelectedUnitPos(Point::INVALID);
	}
	
	unitToAttack->statsWithItems["currentLife"] -= damage;
	unitToAttack->stats["currentLife"] -= damage;
	
	if (unitToAttack->statsWithItems["currentLife"] <= 0) {
		unitToAttack->statsWithItems["currentLife"] = 0;
		if (unitToAttack->getTeamOne()) {
			if (battleType == BattleType::PLAYER_NPC || battleType == BattleType::PLAYER_STRUCT) {
				playerUnitCount--;
			} else {
				army1UnitCount--;
			}
		} else {
			if (battleType == BattleType::PLAYER_NPC || battleType == BattleType::PLAYER_STRUCT) {
				enemyUnitCount--;
			} else {
				army2UnitCount++;
			}
		}
		unitToAttack->kill();
	}
	
	currentAttackingUnit->addTempXP(damage);
	
	if (battleType == BattleType::PLAYER_NPC || battleType == BattleType::PLAYER_STRUCT) {
		if (isContinuation) {
			continueBattle();
		}
	}
}

int Battle::getMaxTurns() {
	return maxTurns;
}

int Battle::getCurrentTurn() {
	return currentTurn;
}

Point Battle::getAttackTexturePosition() {
	return attackTexturePosition;
}

int Battle::getAnimSpeed() {
	return animSpeed;
}

BattleType Battle::getBattleType() {
	return battleType;
}

void Battle::setAttackTexturePosition(Point newAttackTexturePosition) {
	attackTexturePosition = newAttackTexturePosition;
}

bool Battle::isMainLoopEnded = false;

void Battle::initGUIBattle() {
	//Intializing the battle
	currentAttackingUnit = NULL;
	attackTexture = NULL;
	attackTexturePosition = Point(0, 0);
	animSpeed = Global::ticks / 4;
	
	gui = new WholeScreenGUI(Global::permaGUI->getDim());
	gui->setBelongsToBattle(true);
	gui->addPart(player->getArmy());
	gui->addPart(enemyArmy);
	gui->addPart(player->getArmy()->getUnitInfo());
	
	maxTurns = 100;
	currentTurn = 0;
	
	//Displayes the current turn
	class TurnCounter : public BasicGUI {
	public:
		TurnCounter(int xp, int yp, int wp, int hp, int& tC) : BasicGUI(xp, yp, wp, hp), turnCounter(tC) {}
		
		void render() {
			SDL_Rect destinationRect = {x, y, w, h};
			Global::resourceHandler->getATexture(TT::GUI, "guiheader")->render(destinationRect);
			
			ATexture* turnCounterTexture = Global::resourceHandler->getTextTexture(std::to_string(turnCounter), Global::resourceHandler->getColor("whole-header"), 64);
			Dimension d = turnCounterTexture->getDimensions();
			destinationRect.x = x + w / 2 - d.W() / 2;
			destinationRect.y = y + h / 2 - d.H() / 2;
			destinationRect.w = d.W();
			destinationRect.h = d.H();
			turnCounterTexture->render(destinationRect);
		}
	private:
		int& turnCounter;
	};
	
	if (battleType == BattleType::PLAYER_NPC) {
		gui->addTempPart(new TurnCounter(player->getArmy()->getUnitInfo()->getX(),
									enemyArmy->getY(),
									player->getArmy()->getUnitInfo()->getW(),
									gui->getHeaderSize(),
									currentTurn));
	}
	
	//Setting this battle as current gui
	Global::guiHandler->setBattle(this);
	Global::guiHandler->setGUI(gui);
	
	playerUnitCount = 0;
	enemyUnitCount = 0;
}

void Battle::quickBattle() {
	maxTurns = 100;
	currentTurn = 0;
	while (currentTurn < maxTurns) {
		
		if (speedPQ.empty()) {
			currentTurn++;
			
			army1UnitCount = 0;
			army2UnitCount = 0;
			
			//We add all units in the battlefield into the queue
			for (int j = 0; j < army1->getWidth(); j++) {
				for (int k = 0; k < army1->getHeight(); k++) {
					Unit* tempA1Unit = army1->getUnit(j, k);
					Unit* tempA2Unit = army2->getUnit(j, k);
					
					if (tempA1Unit != NULL && !tempA1Unit->isDead()) {
						tempA1Unit->setTeamOne(true);
						tempA1Unit->statsWithItems["currentNumberOfActions"] = tempA1Unit->statsWithItems["numberOfActions"];
						speedPQ.push(tempA1Unit);
						army1UnitCount++;
					}
					if (tempA2Unit != NULL && !tempA2Unit->isDead()) {
						tempA2Unit->setTeamOne(false);
						tempA2Unit->statsWithItems["currentNumberOfActions"] = tempA2Unit->statsWithItems["numberOfActions"];
						speedPQ.push(tempA2Unit);
						army2UnitCount++;
					}
				}
			}
		}
		
		if (army1UnitCount == 0) {
			army2->finalizeUnitExperiences();
			
			//Adding enemy inventory
			if (battleType == BattleType::NPC_NPC) {
				//Adding gold
				npc2->giveGold(npc1->getGold());
				for (unsigned int i = 0; i < npc1->getInventory()->getInventorySize(); i++) {
					Item* currentItem = npc1->getInventory()->getItem(i);
					if (currentItem != NULL) {
						npc2->getInventory()->addItem(currentItem);
					}
				}
			}
			
			if (battleType == BattleType::NPC_NPC) {
				for (int i = 0; i < army1->getWidth(); i++) {
					for (int j = 0; j < army1->getHeight(); j++) {
						Unit* tempUnit = army1->getUnit(i, j);
						if (tempUnit != NULL) {
							for (int k = 0; k < tempUnit->getUnitInventorySize(); k++) {
								Item* currentItem = tempUnit->getItem(k);
								if (currentItem != NULL) {
									npc2->getInventory()->addItem(currentItem);
								}
							}
						}
						
						//NOTE completely irrelevant stuff
						Unit* tempA2Unit = army2->getUnit(i, j);
						if (tempA2Unit != NULL) {
							tempA2Unit->recalculateInventory();
						}
					}
				}
			}
			
			if (battleType == BattleType::NPC_NPC) {
				npc1->kill();
			}
			if (battleType == BattleType::NPC_STRUCT) {
				//If struct won
				npc1->kill();
			}
			return;
		}
		if (army2UnitCount == 0) {
			army1->finalizeUnitExperiences();
			
			//Adding enemy inventory
			if (battleType == BattleType::NPC_NPC) {
				//Adding gold
				npc1->giveGold(npc2->getGold());
				for (unsigned int i = 0; i < npc2->getInventory()->getInventorySize(); i++) {
					Item* currentItem = npc2->getInventory()->getItem(i);
					if (currentItem != NULL) {
						npc1->getInventory()->addItem(currentItem);
					}
				}
			}
			
			for (int i = 0; i < army2->getWidth(); i++) {
				for (int j = 0; j < army2->getHeight(); j++) {
					Unit* tempUnit = army2->getUnit(i, j);
					if (tempUnit != NULL) {
						for (int k = 0; k < tempUnit->getUnitInventorySize(); k++) {
							Item* currentItem = tempUnit->getItem(k);
							if (currentItem != NULL) {
								npc1->getInventory()->addItem(currentItem);
							}
						}
					}
					
					//NOTE completely irrelevant stuff
					Unit* tempA1Unit = army1->getUnit(i, j);
					if (tempA1Unit != NULL) {
						tempA1Unit->recalculateInventory();
					}
				}
			}
			
			if (battleType == BattleType::NPC_NPC) {
				npc2->kill();
			}
			if (battleType == BattleType::NPC_STRUCT) {
				//If npc won
				structToFight->setOwner(npc1);
				structToFight->clearGarrisonArmy();
			}
			return;
		}
		
		while(!speedPQ.empty()) {
			Unit* currentUnit = speedPQ.top();
			if (currentUnit == NULL || currentUnit->isDead()) {
				speedPQ.pop();
				continue;
			}
			
			if (currentUnit->getTeamOne()) {
				//If the unit belongs to 'npc1'
				aiWhichUnitToAttack(currentUnit, army1, army2);
			} else {
				aiWhichUnitToAttack(currentUnit, army2, army1);
			}
		}
	}
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

void Battle::aiWhichUnitToAttack(Unit* currentUnit, Army* unitsArmy, Army* relativeEnemyArmy) {
	//If the enemy attacks
	currentAttackingUnit = currentUnit;
	
	if (unitsArmy->getUPFromPos(currentUnit->getPosition()) == UnitPosition::SUPPORT) {
		//Places a unit sitting in support to an open position if possible
		Point openPosition;
		if (currentUnit->isMelee()) {
			openPosition = unitsArmy->getFirstOpenFrontRowPosition();
		} else {
			openPosition = unitsArmy->getFirstOpenBackRowPosition();
		}
		if (openPosition != Point::INVALID) {
			unitsArmy->switchUnits(currentUnit->getPosition(), openPosition);
		}
					
		removeUnitFromQueue(currentUnit);
		
		return;
	}
	
	if (unitsArmy->getUPFromPos(currentUnit->getPosition()) == UnitPosition::BACKROW && currentUnit->isMelee()) {
		//Places a melee unit in backrow to an open place in frontrow
		Point openPosition = unitsArmy->getFirstOpenFrontRowPosition();
		if (openPosition != Point::INVALID) {
			unitsArmy->switchUnits(currentUnit->getPosition(), openPosition);
		}
		
		removeUnitFromQueue(currentUnit);
		
		return;
	}
	
	if (unitsArmy->getUPFromPos(currentUnit->getPosition()) == UnitPosition::FRONTROW && currentUnit->isRanged()) {
		//Places a ranged unit in backrow to an open place in backrow
		Point openPosition = unitsArmy->getFirstOpenBackRowPosition();
		if (openPosition != Point::INVALID) {
			unitsArmy->switchUnits(currentUnit->getPosition(), openPosition);
		}
		
		removeUnitFromQueue(currentUnit);
		
		return;
	}
	
	std::vector<Unit*> attackableUnits;
	
	switch (unitsArmy->getUPFromPos(currentUnit->getPosition())) {
		case UnitPosition::BACKROW:
			if (currentUnit->isRanged()) {
				//Ranged attack case
				
				//Adding front and back row
				for (int j = 0; j <= 1; j++) {
					for (int i = 1; i < relativeEnemyArmy->getWidth() - 1; i++) {
						Unit* possibleAttackableUnit = relativeEnemyArmy->getUnit(i, j);
						if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
							attackableUnits.push_back(possibleAttackableUnit);
						}
					}
				}
				
				//If front&back row is empty, then we add support too
				if (relativeEnemyArmy->areFrontAndBackRowsEmpty()) {
					std::vector<Point> supportPoints = {Point(0, 0), Point(0, 1),
						Point(relativeEnemyArmy->getWidth() - 1, 0),
						Point(relativeEnemyArmy->getWidth() - 1, 1)};
					
					for (unsigned int i = 0; i < supportPoints.size(); i++) {
						Unit* possibleAttackableUnit = relativeEnemyArmy->getUnit(supportPoints[i]);
						if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
							attackableUnits.push_back(possibleAttackableUnit);
						}
					}
				}
			}
			break;
		case UnitPosition::SUPPORT:
			break;
		case UnitPosition::FRONTROW:
			//Adding front row
			for (int i = 1; i < relativeEnemyArmy->getWidth() - 1; i++) {
				Unit* possibleAttackableUnit = relativeEnemyArmy->getUnit(i, 0);
				if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
					attackableUnits.push_back(possibleAttackableUnit);
				}
			}
			
			if (relativeEnemyArmy->isFrontRowEmpty()) {
				//If front row is empty, adding back row
				for (int i = 1; i < relativeEnemyArmy->getWidth() - 1; i++) {
					Unit* possibleAttackableUnit = relativeEnemyArmy->getUnit(i, 1);
					if (possibleAttackableUnit != NULL && !possibleAttackableUnit->isDead()) {
						attackableUnits.push_back(possibleAttackableUnit);
					}
				}
			}
			
			//If front&back row is empty, then we add support too
			if (relativeEnemyArmy->areFrontAndBackRowsEmpty()) {
				std::vector<Point> supportPoints = {Point(0, 0), Point(0, 1),
					Point(relativeEnemyArmy->getWidth() - 1, 0),
					Point(relativeEnemyArmy->getWidth() - 1, 1)};
				
				for (unsigned int i = 0; i < supportPoints.size(); i++) {
					Unit* possibleAttackableUnit = relativeEnemyArmy->getUnit(supportPoints[i]);
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
