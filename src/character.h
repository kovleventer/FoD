#pragma once

#include <string>

#include "util/point.h"
#include "mapentity.h"
#include "army.h"
#include "inventory.h"

/*!
 * @author kovlev
 */


/*!
 * @class Character
 * This is not a wrapper class of an alphabetical letter
 * A character can be the player or a simple npc
 */
class Character : public MapEntity {
public:
	Character(Point pos);
	Character(int x, int y);
	~Character();
	
	//Getters
	PointD getProgressVector();
	double getSpeed();
	Inventory* getInventory();
	Army* getArmy();
	std::string getName();
	int getGold();
	
	//Setters
	void setProgressVector(PointD newProgressVector);
	void setSpeed(double newSpeed);
	void setName(std::string newName);
	void giveGold(int goldToGive);
	void takeGold(int goldToTake);
protected:
	//When we are rendering, we are using position+progressVector to get the actual position of the claracter
	PointD progressVector;
	
	//The characters walking speed
	double speed;
	
	//Character's army and inventory
	Army* army;
	Inventory* inventory;
	
	std::string name;
	
	//Used in texture animation
	void calcRotation(Point pRot);
	
	//The amount of gold that the caracter posess
	int gold;
};
