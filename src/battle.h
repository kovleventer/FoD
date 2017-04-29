#pragma once

#include <queue>

#include "npc.h"
#include "wholescreengui.h"
#include "player.h"
#include "popup.h"
#include "animatabletexture.h"

class Battle {
public:
	//Quick Battle
	Battle(NPC* npc1, NPC* npc2);
	
	//Interactive Battle with player
	Battle(NPC* e);
	
	~Battle();
	
	//Its easier to hanlde this way due to animations
	void render();
	
	//Starts the battle
	void start();
	
	//Continues the battle
	void continueBattle();
	
	//Calculates damage
	//Needs to be stored to call dealDamage properly
	int getPossibleDamage(Unit* unitToAttack);
	//Continues the battle by attacking a unit or just simply preforms the attack
	void dealDamage(Unit* unitToAttack, int damage, bool isContinuation);
	
	//Getters (probably not used very much)
	int getMaxTurns();
	int getCurrentTurn();
	Point getAttackTexturePosition();
	int getAnimSpeed();
	
	//Setters
	void setAttackTexturePosition(Point newAttackTexturePosition);
	
	//Stores the animation texture
	ATexture* attackTexture;
private:
	WholeScreenGUI* gui;
	
	//If the player is participating in the battle
	NPC* enemy;
	Player* player;
	int playerUnitCount, enemyUnitCount;
	
	//If the battle is between two npcs
	//A valuable lesson was learned here
	NPC* npc1;
	NPC* npc2;
	
	//Turn counters
	int maxTurns, currentTurn;
	
	//Stores the speed values for the units
	std::priority_queue<Unit*, std::vector<Unit*>, UnitSpeedComparator> speedPQ;
	
	//Stores the current attacking unit
	Unit* currentAttackingUnit;
	
	//To prevent code duplication
	void removeUnitFromQueue(Unit* unitToRemove);
	
	//Anim stuff
	Point attackTexturePosition;
	int animSpeed;
};
