#pragma once

#include <vector>
#include <chrono>
#include <thread>

#include "animatabletexture.h"
#include "point.h"

class AnimationHandler {
public:
	AnimationHandler();
	~AnimationHandler();
	
	void nextTick();
	
	void addAnimatedTexture(ATexture* animText);
	void removeAnimatedTexture(ATexture* animText);
	
	//Used in battle
	void animateBattleAction(Point startCoord, Point endCoord);
private:
	std::vector<ATexture*> animatedTextures;
	
	//Total ticks since the beginning
	int totalTicks;
	
	//Battle animation stuff
	Point moveVector;
	bool allowAnimatingBattle;
};
