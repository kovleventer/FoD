#include "player.h"

#include "global.h"
#include "pathfinding.h"

Player::Player(std::string text, int x, int y) : Player(text, Point(x, y)) {}

Player::Player(std::string text, Point pos) : MapEntity(pos) {
	hasPlannedPath = false;
	state = PlayerState::STANDING;
	texture = Global::resourceHandler->npcTextures[text];
	speed = 0.1 / Global::fps * 60;
	tileProgress = 0;
	follow = NULL;
	inventory = NULL;
	army = NULL;
	name = "";
	atBackground = false;
}

Player::~Player() {
	delete army->getUnitInfo();
	delete army;
	delete inventory->getItemInfo();
	delete inventory;
}

void Player::updatePlayerPosition() {
	Global::camera->setPosition(PointD(PointD(position) + progressVector) * Global::tileSize - PointD(Global::screenWidth / 2, Global::screenHeight / 2));
	
	//If we are following an npc
	if (follow != NULL) {
		//NOTE this slows down the program a lot
		path = Pathfinding::findPath(position, follow->getPosition());
		tileProgress = 0;
		
		//TODO clean up this mess
		
		if ((((PointD)position * Global::tileSize) + progressVector * Global::tileSize).distanceTo(((PointD)follow->getPosition() * Global::tileSize) + follow->getProgressVector() * Global::tileSize) <= Global::tileSize * 2 / 3) {
			follow->activate();
			state = PlayerState::STANDING;
			clearPath();
		}
	}
	
	Point nextTile = path[tileProgress + 1];
	progressVector += PointD(nextTile - position) * speed;
	
	if (std::abs(progressVector.getX()) > 1 || std::abs(progressVector.getY()) > 1) {
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

std::string Player::getName() {
	return name;
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

void Player::setName(std::string newName) {
	name = newName;
}

void Player::clearPath() {
	follow = NULL;
	hasPlannedPath = false;
	path.clear();
}
