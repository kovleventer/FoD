#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "point.h"
#include "mapentity.h"
#include "circularpath.h"
#include "army.h"

class NPC : public MapEntity {
public:
	NPC();
	NPC(int textID, Point pos);
	NPC(int textID, std::vector<Point> pathPoints);
	
	//deletes path
	~NPC();
	
	//Calculates the NPC's path
	void initPath();
	
	//called once per frame
	void updateNPCPosition();
	
	//When the player meets with this NPC
	void activate();
	
	//Getters
	PointD getProgressVector();
	double getSpeed();
	bool getStanding();
	std::vector<Point> getTempCont();
	CircularPath* getPath();
	Army* getArmy();
	
	//Setters
	void setProgressVector(PointD newProgressVector);
	void setSpeed(double newSpeed);
	void setPath(CircularPath* newPath);
private:
	void init();
	
	//The position used for rendering is calculated
	PointD progressVector;
	
	//NPC's speed
	double speed;
	
	CircularPath* path;
	std::vector<Point> temporaryContainer;
	
	//Is NPC standing
	bool isStanding;
	
	//NPC's army
	Army* army;
};
