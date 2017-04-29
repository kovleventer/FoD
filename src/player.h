#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <string>

#include "point.h"
#include "mapentity.h"
#include "npc.h"
#include "inventory.h"
#include "army.h"

enum class PlayerState {
	STANDING,
	HAS_WAY,
	MOVING,
	BATTLING
};

class Player : public MapEntity {
public:
	Player(std::string text, int x, int y);
	Player(std::string text, Point pos);
	~Player();
	
	void updatePlayerPosition();
	
	//Getters
	PointD getProgressVector();
	bool getHasPlannedPath();
	PlayerState getState();
	std::vector<Point> getPath();
	int getTileProgress();
	double getSpeed();
	NPC* getFollow();
	Inventory* getInventory();
	Army* getArmy();
	std::string getName();
	
	//Setters
	void setProgressVector(PointD newProgressVector);
	void setHasPlannedPath(bool newHasPlannedPath);
	void setState(PlayerState newState);
	void setPath(std::vector<Point> newPath);
	void setTileProgress(int newTileProgress);
	void setSpeed(double newSpeed);
	void setFollow(NPC* toFollow);
	void setInventory(Inventory* newInventory);
	void setArmy(Army* newArmy);
	void setName(std::string newName);
	
	void clearPath();
private:
	//When we are rendering, we are using position+progressVector to get the actual position of the player
	PointD progressVector;
	
	//If the player has a planned way
	bool hasPlannedPath;
	
	//Is the player standing, ready to move, or already moving
	PlayerState state;
	
	std::vector<Point> path;
	
	int tileProgress;
	double speed;
	
	//Pointer to the NPC, that the player follows
	NPC* follow;
	
	//Inventory and army set by permagui
	Inventory* inventory;
	Army* army;
	
	std::string name;
	
	void calcRotation(Point pRot);
};
