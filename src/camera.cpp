#include "camera.h"

#include "global.h"

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
	
	//FIXME do not return, if we are at the corners, we should set the camera the nearest available position
	//We are at a corner
	if ((newPosition.getX() < 0 || newPosition.getX() >= Global::gameBoardWidth * Global::tileSize - Global::screenWidth)
		&& (newPosition.getY() < 0 || newPosition.getY() >= Global::gameBoardHeight * Global::tileSize - Global::screenHeight)) {
		return;
	}
	
	//we are only moving vertically, if we reached the left or right borders
	if (newPosition.getX() < 0 || newPosition.getX() >= Global::gameBoardWidth * Global::tileSize - Global::screenWidth) {
		position = Point(position.getX(), newPosition.getY());
		return;
	}
	
	//we are only moving horizontally, if we reached the top or bottom borders
	if(newPosition.getY() < 0 || newPosition.getY() >= Global::gameBoardHeight * Global::tileSize - Global::screenHeight) {
		position = Point(newPosition.getX(), position.getY());
		return;
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
