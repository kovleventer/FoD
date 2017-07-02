#include "npchandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

NPCHandler::NPCHandler() {
	basePath = "data/map/npc/";
}

NPCHandler::~NPCHandler() {
	//Can NOT use stdex here
	for (unsigned int i = 0; i < npcs.size(); i++) {
		delete npcs[i];
	}
	
	delete Character::characterPlaceholderTakeable;
	Character::characterPlaceholderTakeable = NULL;
	delete Character::characterPlaceholderNeutral;
	Character::characterPlaceholderNeutral = NULL;
}

void NPCHandler::loadAll() {
	Character::characterPlaceholderTakeable = new Character(Point::INVALID);
	Character::characterPlaceholderTakeable->setName("__takeable__");
	Character::characterPlaceholderNeutral = new Character(Point::INVALID);
	Character::characterPlaceholderNeutral->setName("__neutral__");
	
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
		//Armys unit hps lost
		//Armys unit xp gained
		//4*10 rectangle containig unit inventory as quoted strings
		//Number of items (items listed with "")
		//Gold
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
		
		int gold = 0;
		file >> gold;
		
		int pathLen = 0;
		file >> pathLen;
		
		//If NPC
		std::vector<Point> pathPieces;
		//If player or npc with VISIT_OWN_STRUCT
		Point position;
		bool visitOwnStruct = false;
		
		if (textureName == "player") {
			int px, py;
			file >> px;
			file >> py;
			position = Point(px, py);
		} else {
			if (pathLen == -1) {
				//-1 indicates that it is only for position
				int px, py;
				file >> px;
				file >> py;
				position = Point(px, py);
				visitOwnStruct = true;
			} else {
				for (int i = 0; i < pathLen; i++) {
					int pathX, pathY;
					file >> pathX;
					file >> pathY;
					pathPieces.push_back(Point(pathX, pathY));
				}
			}
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
			Global::player->setPosition(position);
			Global::player->setScale(scale);
			Global::player->setName(name);
			//Adding units to army
			for (int i = 0; i < 10; i++) {
				if (unitNames[i] == "") continue;
				
				Unit* currentUnit = Global::unitHandler->getUnit(unitNames[i], unitLevels[i]);
				
				//Adding items to units
				for (int j = 0; j < 4; j++) {
					currentUnit->addItem(Global::itemHandler->getItem(unitInventory[i * 4 + j]));
				}
				
				currentUnit->recalculateInventory();
				
				currentUnit->statsWithItems["currentLife"] -= unitDamages[i];
				currentUnit->stats["currentLife"] -= unitDamages[i];
				currentUnit->statsWithItems["currentExperience"] += unitExperiences[i];
				currentUnit->stats["currentExperience"] += unitExperiences[i];
				
				Global::player->getArmy()->setUnit(i, currentUnit);
			}
			
			//Adding items to invetory
			for (int i = 0; i < inventoryLen; i++) {
				Global::player->getInventory()->addItem(Global::itemHandler->getItem(inventory[i]));
			}
			
			Global::player->giveGold(gold);
			
			Global::camera->setPosition(position);
		} else {
			//Initializing NPC
			//NOTE a little bit of redundancy here
			NPC* loaded;
			
			if (pathPieces.size() == 1) {
				loaded = new NPC(textureName, pathPieces[0], NPCMovement::STANDING);
				Global::map->getTile(pathPieces[0])->entities.push_back(loaded);
			} else if (pathPieces.size() == 0) {
				if (visitOwnStruct) {
					loaded = new NPC(textureName, position, NPCMovement::VISIT_OWN_STRUCT);
					Global::map->getTile(position)->entities.push_back(loaded);
				} else {
					loaded = new NPC(textureName, Point(0, 0), NPCMovement::STANDING);
					Global::map->getTile(0, 0)->entities.push_back(loaded);
				}
			} else {
				loaded = new NPC(textureName, new CircularPath(pathPieces));
				Global::map->getTile(pathPieces[0])->entities.push_back(loaded);
			}
			
			loaded->setName(name);
			loaded->setIsEnemy(isEnemy);
			loaded->setScale(scale);
			
			//Adding units to army
			for (int i = 0; i < 10; i++) {
				if (unitNames[i] == "") continue;
				
				Unit* currentUnit = Global::unitHandler->getUnit(unitNames[i], unitLevels[i]);
				
				//Adding items to units
				for (int j = 0; j < 4; j++) {
					currentUnit->addItem(Global::itemHandler->getItem(unitInventory[i * 4 + j]));
				}
				
				currentUnit->recalculateInventory();
				
				currentUnit->statsWithItems["currentLife"] -= unitDamages[i];
				currentUnit->stats["currentLife"] -= unitDamages[i];
				currentUnit->statsWithItems["currentExperience"] += unitExperiences[i];
				currentUnit->stats["currentExperience"] += unitExperiences[i];
				
				loaded->getArmy()->setUnit(i, currentUnit);
			}
			
			//Adding items to invetory
			for (int i = 0; i < inventoryLen; i++) {
				loaded->getInventory()->addItem(Global::itemHandler->getItem(inventory[i]));
			}
			
			loaded->giveGold(gold);
			
			npcs.push_back(loaded);
			
			if (npcsByName.find(name) != npcsByName.end()) {
				throw std::runtime_error("Duplicate name: \"" + name + "\"");
			}
			npcsByName[name] = loaded;
		}
	}
}

void NPCHandler::updateNPCsPosition() {
	for (unsigned int i = 0; i < npcs.size(); i++) {
		npcs[i]->updateNPCPosition();
	}
}

Character* NPCHandler::getCharacterByName(std::string characterName) {
	//Some python like special-checking
	if (characterName == "__takeable__") {
		return Character::characterPlaceholderTakeable;
	}
	if (characterName == "__neutral__") {
		return Character::characterPlaceholderNeutral;
	}
	if (characterName == "__player__") {
		return Global::player;
	}
	if (npcsByName.find(characterName) == npcsByName.end()) {
		return NULL;
	}
	return npcsByName[characterName];
}
