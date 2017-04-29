#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "filesystemhandler.h"
#include "worldobject.h"
#include "impassableworldobject.h"
#include "interactiveworldobject.h"

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
	
	//For easier deletion on closeup
	std::vector<WorldObject*> worldObjects;
	std::vector<ImpassableWorldObject*> impassables;
	std::vector<InteractiveWorldObject*> interactives;
private:
	std::string worldObjectBasePath;
	std::string impassableBasePath;
	std::string interactiveBasePath;
	
	void loadWorldObjects();
	void loadImpassableWorldObjects();
	void loadInteractiveWorldObjects();
};
