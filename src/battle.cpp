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
	
	//Setting this battle as current gui
	Global::guiHandler->setGUI(gui);
	Global::guiHandler->setHardlocked(true);
}

Battle::~Battle() {
	//Removes gui
	//NOTE possibly unsafe since this runs when a random battle between npc's occurs
	//But we can assume that in that case no gui will be present
	Global::guiHandler->setHardlocked(false);
	Global::guiHandler->setGUI(NULL);
}
