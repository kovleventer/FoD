#pragma once

#include <SDL.h>
#include <vector>
#include <cmath>
#include <string>

#include "../util/point.h"
#include "npc.h"
#include "../player/inventory.h"
#include "../player/army.h"
#include "character.h"
#include "../player/questdisplay.h"

/*!
 * @author kovlev
 */


/*!
 * @enum PlayerState represents the different player states
 * The game flow will be different on these cases
 * The game ticks only increment when the player is moving
 */
enum class PlayerState {
	STANDING,
	HAS_WAY,
	MOVING,
	BATTLING
};


/*!
 * @class Player the character which is controlled by us
 * Has everything that a regular NPC also has
 */
class Player : public Character {
public:
	Player(std::string text, int x, int y);
	Player(std::string text, Point pos);
	~Player();
	
	void updatePlayerPosition();
	
	//Getters
	bool getHasPlannedPath();
	PlayerState getState();
	std::vector<Point> getPath();
	int getTileProgress();
	NPC* getFollow();
	QuestLister* getQuestLister();
	
	//Setters
	void setHasPlannedPath(bool newHasPlannedPath);
	void setState(PlayerState newState);
	void setPath(std::vector<Point> newPath);
	void setTileProgress(int newTileProgress);
	void setFollow(NPC* toFollow);
	void setInventory(Inventory* newInventory);
	void setArmy(Army* newArmy);
	void setQuestLister(QuestLister* newQuestLister);
	
	//Clears player path
	void clearPath();
private:
	//If the player has a planned way
	bool hasPlannedPath;
	
	//Is the player standing, ready to move, or already moving
	PlayerState state;
	
	std::vector<Point> path;
	
	int tileProgress;
	
	//Pointer to the NPC, that the player follows
	NPC* follow;
	
	QuestLister* questLister;
};
