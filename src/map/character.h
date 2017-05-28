#pragma once

#include <string>

#include "../util/point.h"
#include "mapentity.h"
#include "../player/army.h"
#include "../player/inventory.h"

class InteractiveWorldObject; // Forward declaration

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
	virtual ~Character();
	
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
	
	//Owned controllers
	virtual void addOwned(InteractiveWorldObject* interactiveToAdd);
	virtual void removeOwned(InteractiveWorldObject* interactiveToRemove);
	
	//Indicates if a structure is takeable by anyone
	static Character* characterPlaceholderTakeable;
	//Indicates if a structure can not have any owner
	static Character* characterPlaceholderNeutral;
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
	
	//The interactive world objects owned by the character
	std::vector<InteractiveWorldObject*> ownedBuildings;
};
