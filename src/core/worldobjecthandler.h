#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "../util/filesystemhandler.h"
#include "../map/worldobject.h"
#include "../util/stdextension.h"
#include "../gui/interactivegui.h"

/*!
 * @author kovlev
 */


/*!
 * @class WorldObjectHandler loads all worldobjects into lists
 * All worldobjects are stored here, including
 * 	Basic WordObject classes
 * 	ImpassableWorldObject classes
 * 	InteractiveWorldObject classes
 */
class WorldObjectHandler {
public:
	WorldObjectHandler();
	~WorldObjectHandler();
	
	void loadAll();
	
	//Handles ownership relations
	void setOwnershipRelations();
	
	//Resets market and barracks inventories
	void refillStocks();
	
	//For easier deletion on cleanup
	std::vector<WorldObject*> worldObjects;
	std::vector<ImpassableWorldObject*> impassables;
	std::vector<InteractiveWorldObject*> interactives;
	
	InteractiveWorldObject* getInteractiveByName(std::string interactiveName);
private:
	std::string worldObjectPath;
	std::string impassablePath;
	std::string interactiveBasePath;
	
	void loadWorldObjects();
	void loadImpassableWorldObjects();
	void loadInteractiveWorldObjects();
	
	//Sets parent relationships between interactives
	void setParentRelations();
	
	//For quick search
	//Technically since the keys are string that may not be the most optimized solution
	std::map<std::string, InteractiveWorldObject*> interactivesByName;
};
