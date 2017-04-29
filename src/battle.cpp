#include "battle.h"

#include "global.h"

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
	Global::guiHandler->setBattle(NULL);
	Global::guiHandler->setGUI(NULL);
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
	
	continueBattle();
}

void Battle::continueBattle() {
	while (currentTurn < maxTurns) {
		if (speedPQ.empty()) {
			currentTurn++;
			
			//We add all units in the battlefield into the queue
			for (int j = 0; j < player->getArmy()->getWidth(); j++) {
				for (int k = 0; k < player->getArmy()->getHeight(); k++) {
					Unit* tempPlayerUnit = player->getArmy()->getUnit(j, k);
					Unit* tempEnemyUnit = enemy->getArmy()->getUnit(j, k);
					
					if (tempPlayerUnit != NULL) {
						tempPlayerUnit->setTeamOne(true);
						speedPQ.push(tempPlayerUnit);
					}
					if (tempEnemyUnit != NULL) {
						tempEnemyUnit->setTeamOne(false);
						speedPQ.push(tempEnemyUnit);
					}
				}
			}
		}
		
		while(!speedPQ.empty()) {
			Unit* currentUnit = speedPQ.top();
			if (currentUnit->getTeamOne()) {
				player->getArmy()->getUnitInfo()->setUnit(currentUnit);
				player->getArmy()->setSelectedUnitPos(currentUnit->getPosition());
				speedPQ.pop();
				player->getArmy()->setAllowAttack(true);
				enemy->getArmy()->setAllowAttack(true);
				currentAttackingUnit = currentUnit;
				return;
			} else {
				speedPQ.pop();
			}
		}
	}
}

void Battle::attack(Unit* unitToAttack) {
	//TODO implementation
	std::cout << "Attacked" << std::endl;
	
	continueBattle();
}
