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
	
	//For easier deletion on cleanup
	std::vector<WorldObject*> worldObjects;
	std::vector<ImpassableWorldObject*> impassables;
	std::vector<InteractiveWorldObject*> interactives;
	
	InteractiveWorldObject* getInteractiveByName(std::string interactiveName);
private:
	std::string worldObjectBasePath;
	std::string impassableBasePath;
	std::string interactiveBasePath;
	
	void loadWorldObjects();
	void loadImpassableWorldObjects();
	void loadInteractiveWorldObjects();
	
	//For quick search
	//Technically since the keys are string that may not be the most optimized solution
	std::map<std::string, InteractiveWorldObject*> interactivesByName;
};
