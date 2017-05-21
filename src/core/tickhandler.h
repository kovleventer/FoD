#pragma once

#include <vector>
#include <chrono>
#include <thread>

#include "animatabletexture.h"
#include "../util/point.h"

/*!
 * @author kovlev
 */


/*!
 * @class TickHandler
 * Changes animation phases
 * Handles game ticks and events too
 */
class TickHandler {
public:
	TickHandler();
	~TickHandler();
	
	void nextTick(bool isGameTickToo);
	
	void addAnimatedTexture(ATexture* animText);
	void removeAnimatedTexture(ATexture* animText);
	
	//Used in battle
	void animateBattleAction(Point startCoord, Point endCoord);
	
	//Getters
	int getTotalTicks();
	int getGameTicks();
private:
	std::vector<ATexture*> animatedTextures;
	
	//Total ticks since the beginning
	int totalTicks;
	
	//Total gameTicks (incremented when player is moving)
	int gameTicks;
	
	//Battle animation stuff
	Point moveVector;
	bool allowAnimatingBattle;
};
