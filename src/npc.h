#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <thread>

#include "util/point.h"
#include "util/circularpath.h"
#include "army.h"
#include "inventory.h"
#include "character.h"

/*!
 * @author kovlev
 */


/*!
 * @class NPC non-player character, non-person character or non-playable character as wikipedia states
 * Controlled by AI
 * Sometimes moves
 */
class NPC : public Character {
public:
	NPC(std::string text, int x, int y);
	NPC(std::string text, Point pos);
	NPC(std::string text, std::vector<Point> pathPoints);
	
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
	bool getStanding();
	std::vector<Point> getTempCont();
	CircularPath* getPath();
	bool isEnemy();
	
	//Setters
	void setSpeed(double newSpeed);
	void setPath(CircularPath* newPath);
	void setIsEnemy(bool newIsEnemy);
private:
	void init();
	
	CircularPath* path;
	std::vector<Point> temporaryContainer;
	
	//Is NPC standing
	bool isStanding;
	
	//TODO factions
	bool enemy;
};
