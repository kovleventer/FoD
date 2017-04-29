#include "battle.h"

#include "global.h"

Battle::Battle(NPC* npc1, NPC* npc2) {
	//TODO implementation
	gui = NULL;
}

Battle::Battle(NPC* enemy) {
	gui = new WholeScreenGUI(Global::permaGUI->getDim());
	gui->addPart(Global::player->getArmy());
	gui->addPart(enemy->getArmy());
	gui->addPart(Global::player->getArmy()->getUnitInfo());
	
	//Setting this battle as current gui
	Global::guiHandler->setGUI(gui);
	Global::guiHandler->setHardlocked(true);
	
	//100 turns max
	for (int i = 1; i <= 100; i++) {
		std::priority_queue<Unit*, std::vector<Unit*>, UnitSpeedComparator> speedPQ;
		for (int j = 0; j < Global::player->getArmy()->getWidth(); j++) {
			for (int k = 0; k < Global::player->getArmy()->getHeight(); k++) {
				Unit* tempPlayerUnit = Global::player->getArmy()->getUnit(j, k);
				Unit* tempEnemyUnit = enemy->getArmy()->getUnit(j, k);
				
				if (tempPlayerUnit != NULL) {
					speedPQ.push(tempPlayerUnit);
				}
				if (tempEnemyUnit != NULL) {
					speedPQ.push(tempEnemyUnit);
				}
			}
		}
		
		while(!speedPQ.empty()) {
			std::cout << speedPQ.top()->statsWithItems["speed"] << std::endl;
			speedPQ.pop();
		}
	}
}

Battle::~Battle() {
	//Removes gui
	//NOTE possibly unsafe since this runs when a random battle between npc's occurs
	//But we can assume that in that case no gui will be present
	Global::guiHandler->setHardlocked(false);
	Global::guiHandler->setGUI(NULL);
}
