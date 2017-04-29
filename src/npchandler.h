#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include "point.h"
#include "npc.h"
#include "circularpath.h"
#include "filesystemhandler.h"

class NPCHandler {
public:
	NPCHandler();
	~NPCHandler();
	
	//Loads all NPCs
	void loadAll();
	
	//Moves all NPCs
	void updateNPCsPosition();
	
	//Storing the NPCs in a vector for handilng movement
	std::vector<NPC*> npcs;
private:
	std::string basePath;
};
