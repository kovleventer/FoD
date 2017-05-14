#pragma once

#include <SDL.h>

#include "../util/point.h"

/*!
 * @author kovlev
 */


/*!
 * @class BasicGUI
 * Provides a simple gui abstraction. We are going to inherit from this class all our gui elements
 * It is basically a rectangle with width, height and position
 * Virtual functions are used for event handling and rendering
 */
class BasicGUI {
public:
	//Base class for all gui elements
	BasicGUI(int xp, int yp, int wp, int hp);
	BasicGUI(SDL_Rect dimensionRect);
	virtual ~BasicGUI();
	
	//Getters
	int getX();
	int getY();
	int getW();
	int getH();
	SDL_Rect getDimensionRect();
	
	//Setters
	void setX(int newX);
	void setY(int newY);
	void setW(int newW);
	void setH(int newH);
	void setDimensionRect(SDL_Rect newDimensionRect);
	
	//Checks if the given point is in the {xywh} rectangle
	bool contains(int xp, int yp);
	bool contains(Point p);
	
	virtual void render();
	virtual void handleLeftClickEvent(int xp, int yp);
	virtual void handleRightClickEvent(int xp, int yp);
	virtual void handleMouseMotionEvent(int xp, int yp);
	virtual void handleMouseWheelEvent(bool up);
protected:
	int x, y, w, h;
};
