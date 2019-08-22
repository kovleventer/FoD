#include "tickhandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

TickHandler::TickHandler() {
	totalTicks = 0;
	gameTicks = 0;
	allowAnimatingBattle = false;
}

TickHandler::~TickHandler() {}

void TickHandler::nextTick(bool isGameTickToo) {
	for (unsigned int i = 0; i < animatedTextures.size(); i++) {
		int currentRelativeTick = totalTicks % animatedTextures[i]->getAnimCycleDur() == 0 ?
									animatedTextures[i]->getAnimCycleDur() :
									totalTicks % animatedTextures[i]->getAnimCycleDur();
		if (animatedTextures[i]->getCurrentDur() == currentRelativeTick) {
			animatedTextures[i]->nextTexture();
			
		}
	}
	
	if (!Global::audioHandler->isPlayingMusic()) {
		Global::audioHandler->playMusic("testmusic");
	}
	
	if (allowAnimatingBattle) {
		Global::guiHandler->getBattle()->setAttackTexturePosition(Global::guiHandler->getBattle()->getAttackTexturePosition() + moveVector);
	}
	
	totalTicks++;
	if (isGameTickToo) {
		//FIXME probably error when more quests exist with the same time value
		if (!Global::questHandler->qtTimePQ.empty()) {
			Quest* currentQuest = Global::questHandler->qtTimePQ.top();
			if (currentQuest->getQTTimeInTicks() == gameTicks) {
				currentQuest->start();
				Global::questHandler->qtTimePQ.pop();
			}
		}
		if (gameTicks % 100 == 0) {
			Global::worldObjectHandler->refillStocks();
		}
		gameTicks++;
	}
}

void TickHandler::addAnimatedTexture(ATexture* animText) {
	animatedTextures.push_back(animText);
}

void TickHandler::removeAnimatedTexture(ATexture* animText) {
	stdex::remove_value_vec(animatedTextures, animText);
}

void TickHandler::animateBattleAction(Point startCoord, Point endCoord) {
	//Required due to threading
	//It is possible that the battle animation still runs when cleanup is happening
	if (Global::guiHandler == NULL) return;
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
	//Required due to threading also down here
	//It is possible that the battle animation still runs when cleanup is happening
	if (Global::guiHandler == NULL || Global::guiHandler->getBattle() == NULL) return;
	Global::guiHandler->getBattle()->attackTexture = NULL;
	allowAnimatingBattle = false;
}

int TickHandler::getTotalTicks() {
	return totalTicks;
}

int TickHandler::getGameTicks() {
	return gameTicks;
}
