#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "filesystemhandler.h"
#include "worldobject.h"
#include "impassableworldobject.h"
#include "interactiveworldobject.h"

class WorldObjectHandler {
public:
	WorldObjectHandler();
	~WorldObjectHandler();
	
	void loadAll();
	
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
