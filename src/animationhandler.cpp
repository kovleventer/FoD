#include "animationhandler.h"

#include "global.h"

AnimationHandler::AnimationHandler() {
	totalTicks = 0;
	allowAnimatingBattle = false;
}

AnimationHandler::~AnimationHandler() {}

void AnimationHandler::nextTick() {
	for (unsigned int i = 0; i < animatedTextures.size(); i++) {
		int currentRelativeTick = totalTicks % animatedTextures[i]->getAnimCycleDur() == 0 ?
									animatedTextures[i]->getAnimCycleDur() :
									totalTicks % animatedTextures[i]->getAnimCycleDur();
		if (animatedTextures[i]->getCurrentDur() == currentRelativeTick) {
			animatedTextures[i]->nextTexture();
			
		}
	}
	
	if (allowAnimatingBattle) {
		Global::guiHandler->getBattle()->setAttackTexturePosition(Global::guiHandler->getBattle()->getAttackTexturePosition() + moveVector);
	}
	
	totalTicks++;
}

void AnimationHandler::addAnimatedTexture(ATexture* animText) {
	animatedTextures.push_back(animText);
}

void AnimationHandler::removeAnimatedTexture(ATexture* animText) {
	//TODO fix this nonsense bug here
	//animatedTextures.erase(std::remove(animatedTextures.begin(), animatedTextures.end(), animText));
}

void AnimationHandler::animateBattleAction(Point startCoord, Point endCoord) {
	//t = s / v
	double s = startCoord.distanceTo(endCoord);
	int v = Global::guiHandler->getBattle()->getAnimSpeed();
	int t = s / v;
	
	//Normalizing moveVector
	PointD normalized = PointD((endCoord - startCoord).getX() / s, (endCoord - startCoord).getY() / s);
	normalized *= v;
	moveVector = Point(normalized);
	
	Global::guiHandler->getBattle()->setAttackTexturePosition(startCoord);
	
	allowAnimatingBattle = true;
	
	std::this_thread::sleep_for(std::chrono::milliseconds(t * Global::ticks / 4));
	Global::guiHandler->getBattle()->attackTexture = NULL;
	allowAnimatingBattle = false;
}
