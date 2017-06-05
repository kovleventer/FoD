#pragma once

#include <SDL.h>
#include <string>
#include <vector>

#include "../util/point.h"
#include "mapentity.h"
#include "../gui/wholescreengui.h"
#include "../player/army.h"

//Forward declarations
class Character;
class InteractiveGUI;
class Quest;
class NPC;

/*!
 * @author kovlev
 */


/*!
 * @class WorldObject This class is the most redundant thing I've ever written
 * TODO maybe pivot system
 */
class WorldObject : public MapEntity {
public:
	WorldObject(std::string text, int x, int y);
	WorldObject(std::string text, Point pos);
	virtual ~WorldObject();
};


/*!
 * @class ImpassableWorldObject a world object that has tiles that other characters can not pass through
 * For example a mountain
 */
class ImpassableWorldObject : public WorldObject {
public:
	ImpassableWorldObject(std::string text, int x, int y, std::vector<Point> impTiles);
	ImpassableWorldObject(std::string text, Point pos, std::vector<Point> impTiles);
	
	//Getters
	std::vector<Point> getImpassableTiles();
private:
	std::vector<Point> impassableTiles;
};


/*!
 * @class InteractiveWorldObject a world objects that has tiles, which trigger some kind of events
 * Has properties like owner or name
 * Often referred to as 'structure' or 'struct' which may cause confusion
 */
class InteractiveWorldObject : public WorldObject {
public:
	InteractiveWorldObject(std::string text, int x, int y, std::vector<Point> intTiles, bool isNeut);
	InteractiveWorldObject(std::string text, Point pos, std::vector<Point> intTiles, bool isNeut);
	~InteractiveWorldObject();
	
	//Getters
	std::vector<Point> getInteractiveTiles();
	bool isNeutral();
	std::string getName();
	Character* getOwner();
	InteractiveGUI* getGUI();
	
	//Setters
	void setName(std::string newName);
	void setOwner(Character* newOwner);
	
	//Activates the current IWO
	//If the player does this
	void activate();
	//If an NPC does this
	void activate(NPC* npc);
	
	//NOTE temporary variable
	std::string tempOwnerHolder;
	
	//Returns the garrison army if such a thing exists
	//Returns NULL otherwise
	Army* getGarrisonArmy();
	
	//Deletes all units from garrison army if that is possible
	void clearGarrisonArmy();
	
	//Quest stuff
	void addQuestTriggerCapture(Quest* questTriggerCaptureToAdd);
	void addQuestObjectiveVisit(Quest* questObjectiveVisitToAdd);
protected:
	//Impassable tiles are relative; (0, 0) means the positionTile
	std::vector<Point> interactiveTiles;
	
	//Is the worldobject neutral, or it has an owner?
	bool neutral;
	
	//The structure's name
	std::string name;
	
	//Either player or an npc
	Character* owner;
	
	InteractiveGUI* gui;
	
	//Quest stuff
	std::vector<Quest*> questTriggerCaptures;
	std::vector<Quest*> questObjectiveVisits;
};
