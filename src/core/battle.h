#pragma once

#include <queue>

#include "../map/npc.h"
#include "../gui/wholescreengui.h"
#include "../map/player.h"
#include "../gui/popup.h"
#include "animatabletexture.h"
#include "../util/pivot.h"

/*!
 * @author kovlev
 */


/*!
 * @enum BattleType 
 * The type of battle
 */
enum class BattleType {
	PLAYER_NPC,
	PLAYER_STRUCT,
	NPC_NPC,
	NPC_STRUCT
};


/*!
 * @class Battle
 * A battle is between two characters
 * On an npc-player (or npc-interactive) duel, the battle is visible, animated and controllable
 * On npc-npc duel or npc vs interactive, the battle is done instantly without gui
 */
class Battle {
public:
	//Quick Battles
	Battle(NPC* n1, NPC* n2);
	
	Battle(NPC* n1, InteractiveWorldObject* iwo);
	
	//Interactive Battle with player
	Battle(NPC* e);
	
	//Interactive Battle with Structure
	Battle(InteractiveWorldObject* iwo);
	
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
	BattleType getBattleType();
	
	//Setters
	void setAttackTexturePosition(Point newAttackTexturePosition);
	
	//Stores the animation texture
	ATexture* attackTexture;
	
	//Hopefuly this solves some threading issues
	static bool isMainLoopEnded;
private:
	WholeScreenGUI* gui;
	BattleType battleType;
	
	//Initializes gui
	void initGUIBattle();
	
	//If the player is participating in the battle
	NPC* enemy;
	InteractiveWorldObject* enemyStruct;
	Player* player;
	int playerUnitCount, enemyUnitCount;
	Army* enemyArmy;
	
	//If the battle is between two npcs
	//A valuable lesson was learned here
	NPC* npc1;
	NPC* npc2;
	InteractiveWorldObject* structToFight;
	Army* army1;
	Army* army2;
	int army1UnitCount, army2UnitCount;
	//This function does not require threading magic
	void quickBattle();
	
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
	
	//AI decides which unit to attack next
	//Some movement had been implemented however it is far from comlete
	//After the target is chosen, calls the dealDamage method
	void aiWhichUnitToAttack(Unit* currentUnit, Army* unitsArmy, Army* relativeEnemyArmy);
};
