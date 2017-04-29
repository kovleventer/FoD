#include "player.h"

#include "global.h"
#include "pathfinding.h"

Player::Player() : MapEntity(Point(0, 0)) {
	init();
}

Player::Player(Point pos) : MapEntity(pos) {
	init();
}

void Player::updatePlayerPosition() {
	Global::camera->setPosition(PointD(PointD(position) + progressVector) * Global::tileSize - PointD(Global::screenWidth / 2, Global::screenHeight / 2));
	
	//If we are following an npc
	if (follow != NULL) {
		//NOTE this slows down the program a lot
		path = Pathfinding::findPath(position, follow->getPosition());
		tileProgress = 0;
		if (position == follow->getPosition()) {
			follow->activate();
			state = PlayerState::STANDING;
			clearPath();
		}
	}
	
	Point nextTile = path[tileProgress + 1];
	progressVector += PointD(nextTile - position) * speed;
	
	if (fabs(progressVector.getX()) > 1 || fabs(progressVector.getY()) > 1) {
		//We can not simply change the positon, we need the update the references on the tile
		setPosition(nextTile);
		tileProgress++;
		progressVector = PointD();
		if (position == path[path.size()-1]) {
			state = PlayerState::STANDING;
			tileProgress = 0;
			clearPath();
		}
	}
}

PointD Player::getProgressVector() {
	return progressVector;
}

bool Player::getHasPlannedPath() {
	return hasPlannedPath;
}

PlayerState Player::getState() {
	return state;
}

std::vector<Point> Player::getPath() {
	return path;
}

int Player::getTileProgress() {
	return tileProgress;
}

double Player::getSpeed() {
	return speed;
}

NPC* Player::getFollow() {
	return follow;
}

Inventory* Player::getInventory() {
	return inventory;
}

Army* Player::getArmy() {
	return army;
}

void Player::setProgressVector(PointD newProgressVector) {
	progressVector = newProgressVector;
}

void Player::setHasPlannedPath(bool newHasPlannedPath) {
	hasPlannedPath = newHasPlannedPath;
}

void Player::setState(PlayerState newState) {
	state = newState;
}

void Player::setPath(std::vector<Point> newPath) {
	path = newPath;
}

void Player::setTileProgress(int newTileProgress) {
	tileProgress = newTileProgress;
}

void Player::setSpeed(double newSpeed) {
	speed = newSpeed;
}

void Player::setFollow(NPC* toFollow) {
	follow = toFollow;
}

void Player::setInventory(Inventory* newInventory) {
	inventory = newInventory;
}

void Player::setArmy(Army* newArmy) {
	army = newArmy;
}

void Player::clearPath() {
	follow = NULL;
	hasPlannedPath = false;
	path.clear();
}

void Player::init() {
	hasPlannedPath = false;
	state = PlayerState::STANDING;
	texture = Global::resourceHandler->playerTexture;
	//NOTE temporary workaround
	speed = 0.1 / Global::fps * 60;
	tileProgress = 0;
	follow = NULL;
	inventory = NULL;
	army = NULL;
}
