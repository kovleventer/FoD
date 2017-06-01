#pragma once

#include <SDL.h>
#include <vector>
#include <string>
#include <queue>

#include "../util/point.h"
#include "../util/path.h"
#include "../player/army.h"
#include "../player/inventory.h"
#include "character.h"
#include "../util/stdextension.h"

class Quest;

/*!
 * @author kovlev
 */


/*!
 * @enum NPCMovement
 * Determinates what controls the npc's movement
 */
enum class NPCMovement {
	STANDING,
	CIRCULATING,
	VISIT_OWN_STRUCT
};


/*!
 * @class NPC non-player character, non-person character or non-playable character as wikipedia states
 * Controlled by AI
 * Sometimes moves
 */
class NPC : public Character {
	friend class NPCHandler;
public:
	NPC(std::string text, int x, int y, NPCMovement nm);
	NPC(std::string text, Point pos, NPCMovement nm);
	NPC(std::string text, CircularPath* cp);
	
	//deletes path
	~NPC();
	
	//Calculates the NPC's path
	void initPath();
	
	//called once per frame
	void updateNPCPosition();
	
	//When the player meets with this NPC
	void activate();
	
	//Getters
	double getSpeed();
	NPCMovement getMovementType();
	CircularPath* getPath();
	bool isEnemy();
	bool isDead();
	
	//Setters
	void setSpeed(double newSpeed);
	void setIsEnemy(bool newIsEnemy);
	
	//Overriding from character (adding functionality)
	void addOwned(InteractiveWorldObject* interactiveToAdd);
	void removeOwned(InteractiveWorldObject* interactiveToRemove);
	
	//This kills the npc
	void kill();
	
	//Recalculates the optimal positions for units in army
	//Redistributes items
	void rearrangeArmy();
	
	//Quest stuff
	void addQuestTriggerTalk(Quest* questTriggerTalkToAdd);
	void addQuestObjectiveTalk(Quest* questObjectiveTalkToAdd);
	void addQuestObjectiveKill(Quest* questObjectiveKillToAdd);
private:
	void init();
	
	CircularPath* path;
	
	//Is NPC standing
	NPCMovement movementType;
	
	//Sets NPC's position and the AI makes decisions
	void setPositionAI(Point newPosition);
	
	//TODO factions
	bool enemy;
	
	//Is npc dead
	bool dead;
	
	//If position is set already for the current path piece
	bool positionSet;
	
	//If the NPC is standing only temporarily (has currently only one or zero structures)
	bool temporaryStanding;
	
	//Regenerates path by taking all owned structures
	void recalculatePathByOwned();
	
	//Storing data dor quests
	std::vector<Quest*> questTriggerTalks;
	std::vector<Quest*> questObjectiveTalks;
	std::vector<Quest*> questObjectiveKills;
};
