#pragma once

#include <queue>

#include "npc.h"
#include "wholescreengui.h"
#include "player.h"
#include "popup.h"

class Battle {
public:
	//Quick Battle
	Battle(NPC* npc1, NPC* npc2);
	
	//Interactive Battle with player
	Battle(NPC* e);
	
	~Battle();
	
	//Starts the battle
	void start();
	
	//Continues the battle
	void continueBattle();
	
	//Continues the battle by attacking a unit or just simply preforms the attack
	void attack(Unit* unitToAttack, bool isContinuation);
private:
	WholeScreenGUI* gui;
	
	//If the player is participating in the battle
	NPC* enemy;
	Player* player;
	int playerUnitCount, enemyUnitCount;
	
	//If the battle is between two npcs
	NPC* npc1, npc2;
	
	//Turn counters
	int maxTurns, currentTurn;
	
	//Stores the speed values for the units
	std::priority_queue<Unit*, std::vector<Unit*>, UnitSpeedComparator> speedPQ;
	
	//Stores the current attacking unit
	Unit* currentAttackingUnit;
};
