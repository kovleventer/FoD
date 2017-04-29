#pragma once

#include <SDL2/SDL.h>

#include "point.h"

class GUIPart {
public:
	GUIPart(int xp, int yp, int wp, int hp);
	virtual ~GUIPart();
	
	//Getters
	int getX();
	int getY();
	int getW();
	int getH();
	
	//Setters
	void setX(int newX);
	void setY(int newY);
	void setW(int newW);
	void setH(int newH);
	
	//Checks if the given point is in the {xywh} rectangle
	bool contains(int xp, int yp);
	bool contains(Point p);
	
	virtual void render();
	virtual void handleMousePressEvent(int xp, int yp);
	virtual void handleMouseMotionEvent(int xp, int yp);
	virtual void handleMouseWheelEvent(bool up);
protected:
	int x, y, w, h;
};
