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
		//TODO update this
		//File pattern
		//Name
		//Texture Name
		//Armys unit names ("" if empty)
		//Armys unit unit levels
		//Armys unit hps lost
		//Armys unit xp gained
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
		std::vector<int> unitDamages;
		std::vector<int> unitExperiences;
		//Storing 4 * 10 items
		std::vector<std::string> unitInventory;
		
		for (int i = 0; i < 10; i++) {
			std::string unitName;
			//Removes quotes
			//The beauties of the c++14 standards
			file >> std::quoted(unitName);
			unitNames.push_back(unitName);
		}
		
		for (int i = 0; i < 10; i++) {
			int unitLevel;
			file >> unitLevel;
			unitLevels.push_back(unitLevel);
		}
		
		for (int i = 0; i < 10; i++) {
			int unitDamage;
			file >> unitDamage;
			unitDamages.push_back(unitDamage);
		}
		
		for (int i = 0; i < 10; i++) {
			int unitExperience;
			file >> unitExperience;
			unitExperiences.push_back(unitExperience);
		}
		
		//Loading unit items
		for (int i = 0; i < 40; i++) {
			std::string itemName;
			file >> std::quoted(itemName);
			unitInventory.push_back(itemName);
		}
		
		//Loading inventory
		int inventoryLen = 0;
		file >> inventoryLen;
		std::vector<std::string> inventory;
		
		for (int i = 0; i < inventoryLen; i++) {
			std::string itemName;
			file >> std::quoted(itemName);
			inventory.push_back(itemName);
		}
		
		int pathLen = 0;
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
		
		if (!file) {
			//If the file pattern is wrong
			std::clog << "Error! Skipping " << npcNames[i] << std::endl;
			continue;
		}
		
		
		if (textureName == "player") {
			//Player counts as an npc
			Global::player->setPosition(pathPieces[0]);
			Global::player->setScale(scale);
			Global::player->setName(name);
			//Adding units to army
			for (int i = 0; i < 10; i++) {
				if (unitNames[i] == "") continue;
				
				Unit* currentUnit = Global::unitHandler->getUnit(unitNames[i], unitLevels[i]);
				currentUnit->stats["currentLife"] -= unitDamages[i];
				currentUnit->stats["currentExperience"] += unitExperiences[i];
				
				//Adding items to units
				for (int j = 0; j < 4; j++) {
					currentUnit->addItem(Global::itemHandler->getItem(unitInventory[i * 4 + j]));
				}
				
				currentUnit->recalculateInventory();
				
				Global::player->getArmy()->setUnit(i, currentUnit);
			}
			
			//Adding items to invetory
			for (int i = 0; i < inventoryLen; i++) {
				Global::player->getInventory()->addItem(Global::itemHandler->getItem(inventory[i]));
			}
			
			//Global::map->getTile(pathPieces[0])->entities.push_back(Global::player);
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
			
			//Adding units to army
			for (int i = 0; i < 10; i++) {
				if (unitNames[i] == "") continue;
				
				Unit* currentUnit = Global::unitHandler->getUnit(unitNames[i], unitLevels[i]);
				currentUnit->stats["currentLife"] -= unitDamages[i];
				currentUnit->stats["currentExperience"] += unitExperiences[i];
				
				//Adding items to units
				for (int j = 0; j < 4; j++) {
					currentUnit->addItem(Global::itemHandler->getItem(unitInventory[i * 4 + j]));
				}
				
				currentUnit->recalculateInventory();
				
				loaded->getArmy()->setUnit(i, currentUnit);
			}
			
			//Adding items to invetory
			for (int i = 0; i < inventoryLen; i++) {
				loaded->getInventory()->addItem(Global::itemHandler->getItem(inventory[i]));
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
