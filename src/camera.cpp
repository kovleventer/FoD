#include "camera.h"

#include "global.h"

/*!
 * @author kovlev
 */

Camera::Camera() {
	//Top left corner of the map
	position = Point(0, 0);
	speed = 10 * 60 / Global::fps;
}

Camera::Camera(int x, int y, int newSpeed) {
	position = Point(x, y);
	speed = newSpeed;
}

Camera::Camera(Point pos, int newSpeed) {
	position = pos;
	speed = newSpeed;
}

void Camera::move(int x, int y) {
	move(Point(x, y));
}

void Camera::move(Point moveVector) {
	Point newPosition = position + moveVector;
	
	setPosition(newPosition);
}

Point Camera::getPosition() {
	return position;
}

void Camera::setPosition(int x, int y) {
	setPosition(Point(x, y));
}

void Camera::setPosition(Point newPosition) {
	//NOTE this is the only place where we use setters for a strong reason
	//Check if we reached the border of the gameboard
	
	//Much cleaner and efficent code now
	//We do the bottom left checking first
	//So in case our map is degenerate
	//It will be placed at the top-left corner
	if (newPosition.getX() >= Global::gameBoardWidth * Global::tileSize - Global::screenWidth) {
		newPosition.setX(Global::gameBoardWidth * Global::tileSize - Global::screenWidth);
	}
	if (newPosition.getY() >= Global::gameBoardHeight * Global::tileSize - Global::screenHeight + Global::permaGUI->getLowerHeight()) {
		newPosition.setY(Global::gameBoardHeight * Global::tileSize - Global::screenHeight + Global::permaGUI->getLowerHeight());
	}
	if (newPosition.getX() < 0) {
		newPosition.setX(0);
	}
	if (newPosition.getY() < -Global::permaGUI->getUpperHeight()) {
		newPosition.setY(-Global::permaGUI->getUpperHeight());
	}
	
	position = newPosition;
}

void Camera::setPosition(PointD newPosition) {
	int x = newPosition.getX();
	int y = newPosition.getY();
	setPosition(Point(x, y));
}

int Camera::getSpeed() {
	return speed;
}

void Camera::setSpeed(int newSpeed) {
	//Speed can not be negative
	if (newSpeed < 0) return;
	speed = newSpeed;
}
