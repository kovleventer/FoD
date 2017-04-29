#include "npchandler.h"

#include "global.h"

NPCHandler::NPCHandler() {
	basePath = "data/map/npc/";
}

NPCHandler::~NPCHandler() {
	for (unsigned int i = 0; i < npcs.size(); i++) {
		delete npcs[i];
	}
}

void NPCHandler::loadAll() {
	std::vector<std::string> npcNames = FilesystemHandler::getFilesInDir(basePath);
	
	//NOTE uses file IO
	std::fstream file;
	
	for (unsigned int i = 0; i < npcNames.size(); i++) {
		file.open(basePath + npcNames[i], std::ios::in);
		//File pattern
		//Name
		//Texture Name
		//Armys unit names ("" if empty)
		//Armys unit unit levels
		// a [b c (a times)]
		// a: number of points in path
		// b: the x coordinate of the current tile
		// c: the y coordinate of the current tile
		//Scale
		//'enemy' if enemy, anything else if friendly
		std::string name;
		std::getline(file, name);
		
		std::string textureName;
		file >> textureName;
		
		std::vector<std::string> unitNames;
		std::vector<int> unitLevels;
		
		for (int i = 0; i < 10; i++) {
			std::string unitName;
			file >> unitName;
			//Removes quotes
			unitName = unitName.substr(1, unitName.size() - 2);
			unitNames.push_back(unitName);
		}
		
		for (int i = 0; i < 10; i++) {
			int unitLevel;
			file >> unitLevel;
			unitLevels.push_back(unitLevel);
		}
		
		int pathLen;
		file >> pathLen;
		
		std::vector<Point> pathPieces;
		
		for (int i = 0; i < pathLen; i++) {
			int pathX, pathY;
			file >> pathX;
			file >> pathY;
			pathPieces.push_back(Point(pathX, pathY));
		}
		
		double scale;
		file >> scale;
		
		std::string isEnemyString;
		file >> isEnemyString;
		bool isEnemy = isEnemyString == "enemy";
		
		file.close();
		
		
		if (textureName == "player") {
			//Player counts as an npc
			Global::player->setPosition(pathPieces[0]);
			Global::player->setScale(scale);
			Global::player->setName(name);
			for (int i = 0; i < 10; i++) {
				if (unitNames[i] == "") continue;
				
				Global::player->getArmy()->setUnit(i, Global::unitHandler->getUnit(unitNames[i], unitLevels[i]));
			}
			Global::map->getTile(pathPieces[0])->entities.push_back(Global::player);
		} else {
			//Initializing NPC
			//NOTE a little bit of redundancy here
			NPC* loaded;
			
			if (pathPieces.size() == 1) {
				loaded = new NPC(textureName, pathPieces[0]);
				Global::map->getTile(pathPieces[0])->entities.push_back(loaded);
			} else if (pathPieces.size() == 0) {
				loaded = new NPC(textureName, Point(0, 0));
				Global::map->getTile(0, 0)->entities.push_back(loaded);
			} else {
				loaded = new NPC(textureName, pathPieces);
				Global::map->getTile(pathPieces[0])->entities.push_back(loaded);
			}
			
			loaded->setName(name);
			loaded->setIsEnemy(isEnemy);
			loaded->setScale(scale);
			
			for (int i = 0; i < 10; i++) {
				if (unitNames[i] == "") continue;
				
				loaded->getArmy()->setUnit(i, Global::unitHandler->getUnit(unitNames[i], unitLevels[i]));
			}
			
			//Setting path
			if (!loaded->getStanding()) {
				loaded->setPath(new CircularPath(loaded->getTempCont()));
			}
			
			npcs.push_back(loaded);
		}
	}
}

void NPCHandler::updateNPCsPosition() {
	for (unsigned int i = 0; i < npcs.size(); i++) {
		npcs[i]->updateNPCPosition();
	}
}
