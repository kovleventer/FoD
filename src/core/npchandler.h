#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../util/point.h"
#include "../map/npc.h"
#include "../util/path.h"
#include "../util/filesystemhandler.h"

/*!
 * @author kovlev
 */


/*!
 * @class NPCHandler loads npcs from files into a list
 * Loads player stats also
 */
class NPCHandler {
public:
	NPCHandler();
	~NPCHandler();
	
	//Loads all NPCs
	void loadAll();
	
	//Moves all NPCs
	void updateNPCsPosition();
	
	Character* getCharacterByName(std::string characterName);
	
	//Storing the NPCs in a vector for handilng movement
	std::vector<NPC*> npcs;
private:
	std::string basePath;
	
	//Map for faster character searching
	std::map<std::string, NPC*> npcsByName;
};
