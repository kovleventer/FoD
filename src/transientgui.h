#pragma once

#include <SDL2/SDL.h>

//This class is only a shared parent
class TransientGUI {
public:
	TransientGUI(int xp, int yp, int wp, int hp);
	virtual ~TransientGUI();
	
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
	
	virtual void render();
protected:
	int x, y, w, h;
};
