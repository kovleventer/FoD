#include "battle.h"

#include "global.h"
#include "game.h"

Battle::Battle(NPC* npc1, NPC* npc2) {
	//TODO implementation
	gui = NULL;
}

Battle::Battle(NPC* e) {
	player = Global::player;
	enemy = e;
	currentAttackingUnit = NULL;
}

Battle::~Battle() {
	//Removes gui
	//NOTE possibly unsafe since this runs when a random battle between npc's occurs
	//But we can assume that in that case no gui will be present
	/*Global::guiHandler->setBattle(NULL);
	Global::guiHandler->setGUI(NULL);*/
}

void Battle::start() {
	//Intializing the battle
	//FIXME gui is never deleted and its unsafe to delete this
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
						speedPQ.push(tempPlayerUnit);
						playerUnitCount++;
					}
					if (tempEnemyUnit != NULL && !tempEnemyUnit->isDead()) {
						tempEnemyUnit->setTeamOne(false);
						speedPQ.push(tempEnemyUnit);
						enemyUnitCount++;
					}
				}
			}
		}
		
		if (playerUnitCount == 0) {
			Popup* popup = new Popup(800, 400, PopupType::POPUP_OK);
			popup->setText("You died");
			popup->buttonOK->setOnClick(Game::quit);
			Global::guiHandler->clear();
			Global::guiHandler->setGUI(popup);
			delete this;
			return;
		}
		if (enemyUnitCount == 0) {
			Global::guiHandler->clear();
			//FIXME implementing npchandling is urgent
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
				speedPQ.pop();
				player->getArmy()->setAllowAttack(true);
				enemy->getArmy()->setAllowAttack(true);
				currentAttackingUnit = currentUnit;
				return;
			} else {
				//If the enemy attacks
				currentAttackingUnit = currentUnit;
				for (int j = 0; j < player->getArmy()->getWidth(); j++) {
					for (int k = 0; k < player->getArmy()->getHeight(); k++) {
						Unit* unitToAttack = player->getArmy()->getUnit(j, k);
						if (unitToAttack != NULL && !unitToAttack->isDead()) {
							attack(unitToAttack, false);
							//   ¯\_ツ_/¯
							//I do not have any other options sorry
							goto end;
						}
					}
				}
				
				//Goto points here
				end:
				
				speedPQ.pop();
			}
		}
	}
}

void Battle::attack(Unit* unitToAttack, bool isContinuation) {
	//TODO better implementation
	
	
	//Calculating damage
	int attackValue;
	int defenseValue;
	switch(currentAttackingUnit->getUnitType()) {
		case UnitType::FIGHTER:
		case UnitType::OTHER:
			attackValue = currentAttackingUnit->statsWithItems["meleeDamage"];
			defenseValue = unitToAttack->statsWithItems["physicalDefense"];
			break;
		case UnitType::ARCHER:
			attackValue = currentAttackingUnit->statsWithItems["shootingDamage"];
			defenseValue = unitToAttack->statsWithItems["physicalDefense"];
			break;
		case UnitType::MAGE:
			attackValue = currentAttackingUnit->statsWithItems["magicDamage"];
			defenseValue = unitToAttack->statsWithItems["magicDefense"];
			break;
		default:
			attackValue = 0;
			defenseValue = 0;
			break;
	}
	
	if (attackValue <= defenseValue) {
		attackValue = 1;
	} else {
		attackValue -= defenseValue;
	}
	
	unitToAttack->statsWithItems["currentLife"] -= attackValue;
	
	if (unitToAttack->statsWithItems["currentLife"] <= 0) {
		unitToAttack->statsWithItems["currentLife"] = 0;
		if (unitToAttack->getTeamOne()) {
			playerUnitCount--;
		} else {
			enemyUnitCount--;
		}
		unitToAttack->kill();
	}
	
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
