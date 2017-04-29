#pragma once

#include "point.h"

class Camera {
public:
	//Initializes the camera
	Camera();
	Camera(int x, int y, int newSpeed);
	Camera(Point pos, int newSpeed);
	
	//Moves the camera with a vector
	void move(int x, int y);
	void move(Point moveVector);
	
	//Getters, setters
	Point getPosition();
	void setPosition(int x, int y);
	void setPosition(Point newPosition);
	void setPosition(PointD newPosition);
	
	int getSpeed();
	void setSpeed(int newSpeed);
private:
	Point position;
	int speed;
};
