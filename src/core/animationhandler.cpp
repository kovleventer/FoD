#include "animationhandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

AnimationHandler::AnimationHandler() {
	totalTicks = 0;
	gameTicks = 0;
	allowAnimatingBattle = false;
}

AnimationHandler::~AnimationHandler() {}

void AnimationHandler::nextTick(bool isGameTickToo) {
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
		gameTicks++;
		//Small quest stuff
		/*if (gameTicks % 10 == 0 && gameTicks >= 100) {
			Popup* popup = new Popup(800, 400, PopupType::POPUP_OK);
			popup->setText("Trial version expired");
			Global::guiHandler->clear();
			Global::guiHandler->setGUI(popup);
		}*/
	}
}

void AnimationHandler::addAnimatedTexture(ATexture* animText) {
	animatedTextures.push_back(animText);
}

void AnimationHandler::removeAnimatedTexture(ATexture* animText) {
	animatedTextures.erase(std::remove(animatedTextures.begin(), animatedTextures.end(), animText));
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

int AnimationHandler::getTotalTicks() {
	return totalTicks;
}

int AnimationHandler::getGameTicks() {
	return gameTicks;
}
