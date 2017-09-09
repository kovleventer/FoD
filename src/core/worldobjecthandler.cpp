#include "worldobjecthandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

WorldObjectHandler::WorldObjectHandler() {
	worldObjectPath = "data/map/worldobjects.data";
	impassablePath = "data/map/impassables.data";
	interactiveBasePath = "data/map/int/";
}

WorldObjectHandler::~WorldObjectHandler() {
	//Deleting WorldObjects
	stdex::clear_ptr_vec(worldObjects);
	
	//Deleting ImpassableWorldObjects
	stdex::clear_ptr_vec(impassables);
	
	//Deleting InteractiveWorldObjects
	stdex::clear_ptr_vec(interactives);
}

void WorldObjectHandler::loadAll() {
	loadWorldObjects();
	loadImpassableWorldObjects();
	loadInteractiveWorldObjects();
	setParentRelations();
}

void WorldObjectHandler::setOwnershipRelations() {
	for (unsigned int i = 0; i < interactives.size(); i++) {
		interactives[i]->setOwner(Global::npcHandler->getCharacterByName(interactives[i]->tempOwnerHolder));
	}
}

void WorldObjectHandler::refillStocks() {
	for (unsigned int i = 0; i < interactives.size(); i++) {
		interactives[i]->refillStocks();
	}
}

InteractiveWorldObject* WorldObjectHandler::getInteractiveByName(std::string interactiveName) {
	if (interactivesByName.find(interactiveName) == interactivesByName.end()) {
		return NULL;
	}
	return interactivesByName[interactiveName];
}

void WorldObjectHandler::loadWorldObjects() {
	//Every entry is stored in a single file
	//NOTE uses file IO
	std::fstream file;
	file.open(worldObjectPath, std::ios::in);
	int count;
	file >> count;
	for (int i = 0; i < count; i++) {
		//File pattern
		//Texture name
		//Position point
		//Scale
		std::string textureName;
		file >> textureName;
		
		int posX, posY;
		file >> posX;
		file >> posY;
		
		double scale;
		file >> scale;
		
		if (!file) {
			//If the file pattern is wrong
			std::clog << "Error! Skipping entry" << std::endl;
			continue;
		}
		
		WorldObject* loaded = new WorldObject(textureName, posX, posY);
		loaded->setScale(scale);
		//Error checking for position
		if (Global::map->getTile(posX, posY) == NULL) throw std::runtime_error("Invalid world object position (" + textureName + ", (" + std::to_string(posX) + ";" + std::to_string(posY) + "))");
		Global::map->getTile(posX, posY)->backgroundEntities.push_back(loaded);
		worldObjects.push_back(loaded);
	}
	file.close();
}

void WorldObjectHandler::loadImpassableWorldObjects() {
	//Every entry is stored in a single file
	//NOTE uses file IO
	std::fstream file;
	file.open(impassablePath, std::ios::in);
	int count;
	file >> count;
	for (int i = 0; i < count; i++) {
		//File pattern:
		//Texture name
		//Position point
		// a [b c (a times)]
		// a: number of impassable tiles
		// b: the x coordinate of current impassable tile (relative, so (0,0) means the position tile itself)
		// c: the x coordinate of current impassable tile (relative, so (0,0) means the position tile itself)
		//Scale
		std::string textureName;
		file >> textureName;
		
		int posX, posY;
		file >> posX;
		file >> posY;
		
		int impLen;
		std::vector<Point> impassableTiles;
		file >> impLen;
		for (int j = 0; j < impLen; j++) {
			int x, y;
			file >> x;
			file >> y;
			impassableTiles.push_back(Point(x, y));
		}
		
		double scale;
		file >> scale;
		
		if (!file) {
			//If the file pattern is wrong
			std::clog << "Error! Skipping impassable entry" << std::endl;
			continue;
		}
		
		ImpassableWorldObject* loaded = new ImpassableWorldObject(textureName, posX, posY, impassableTiles);
		loaded->setScale(scale);
		//Error checking for position
		if (Global::map->getTile(posX, posY) == NULL) throw std::runtime_error("Invalid world object position (" + textureName + ", (" + std::to_string(posX) + ";" + std::to_string(posY) + "))");
		Global::map->getTile(posX, posY)->entities.push_back(loaded);
		impassables.push_back(loaded);
	}
	file.close();
}

void WorldObjectHandler::loadInteractiveWorldObjects() {
	std::vector<std::string> interactiveNames = FilesystemHandler::getFilesInDir(interactiveBasePath);
	
	//NOTE uses file IO
	std::fstream file;
	for (unsigned int i = 0; i < interactiveNames.size(); i++) {
		file.open(interactiveBasePath + interactiveNames[i], std::ios::in);
		//File pattern:
		//Name
		//Texture name
		//Owner name
		//Has ItemBuyingMenu?, Has UnitBuyingMenu?, Has Garrison?, Has TaxCollector? Has Parent? bools
		//IF HAS ItemBuyingMenu
		//	[itemcount] ["itemname"] itemcount times
		//IF HAS UnitBuyingMenu
		//	[unitcount] ["unitname"] unitcount times
		//IF HAS Garrison
		//	{army}
		//IF HAS TaxCollector
		//	maxAccumulableGold currentAccumulatedGold
		//IF HAS Parent
		//	parentname
		//Position point
		// a [b c (a times)]
		// a: number of interactive tiles
		// b: the x coordinate of current interactive tile (relative, so (0,0) means the position tile itself)
		// c: the x coordinate of current interactive tile (relative, so (0,0) means the position tile itself)
		//Scale
		std::string name;
		std::getline(file, name);
		
		std::string textureName;
		std::getline(file, textureName);
		
		std::string ownerName;
		std::getline(file, ownerName);
		
		bool hasIBM, hasUBM, hasGar, hasTax, hasPar;
		file >> hasIBM;
		file >> hasUBM;
		file >> hasGar;
		file >> hasTax;
		file >> hasPar;
		
		std::vector<Item*> itemsToSell;
		if (hasIBM) {
			int invLen;
			file >> invLen;
			for (int i = 0; i < invLen; i++) {
				std::string itemName;
				file >> std::quoted(itemName);
				itemsToSell.push_back(Global::itemHandler->getItem(itemName));
			}
		}
		
		std::vector<std::string> unitsToSellNames;
		if (hasUBM) {
			int armLen;
			file >> armLen;
			for (int i = 0; i < armLen; i++) {
				std::string unitName;
				file >> std::quoted(unitName);
				unitsToSellNames.push_back(unitName);
			}
		}
		
		std::vector<std::string> unitNames;
		std::vector<int> unitLevels;
		std::vector<int> unitDamages;
		std::vector<int> unitExperiences;
		//Storing 4 * 10 items
		std::vector<std::string> unitInventory;
		
		if (hasGar) {
			for (int i = 0; i < 10; i++) {
				std::string unitName;
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
		}
		
		int maxAccGold;
		int curAccGold;
		if (hasTax) {
 			file >> maxAccGold;
			file >> curAccGold;
		}
		
		std::string parentName;
		if (hasPar) {
			file >> std::quoted(parentName);
		}
		
		int posX, posY;
		file >> posX;
		file >> posY;
		
		int impLen;
		std::vector<Point> interactiveTiles;
		file >> impLen;
		for (int i = 0; i < impLen; i++) {
			int x, y;
			file >> x;
			file >> y;
			interactiveTiles.push_back(Point(x, y));
		}
		
		double scale;
		file >> scale;
		
		file.close();
		
		if (!file) {
			//If the file pattern is wrong
			std::clog << "Error! Skipping " << interactiveNames[i] << std::endl;
			continue;
		}
		
		InteractiveWorldObject* loaded = new InteractiveWorldObject(textureName, posX, posY, interactiveTiles, false);
		loaded->setName(name);
		
		loaded->tempOwnerHolder = ownerName;
		
		//Setting interactive properties
		if (hasIBM) {
			ItemMarket* tempItemMarket = new ItemMarket(loaded->getGUI());
			tempItemMarket->getItemBuyingMenu()->setItemList(itemsToSell);
			loaded->getGUI()->addPart({"Market", tempItemMarket});
			
		}
		if (hasUBM) {
			Barracks* tempBarracks = new Barracks(loaded->getGUI());
			tempBarracks->getUnitBuyingMenu()->setUnitList(unitsToSellNames);
			loaded->getGUI()->addPart({"Barracks", tempBarracks});
		}
		if (hasGar) {
			GarrisonWrapper* tempGarrisonWrapper = new GarrisonWrapper(loaded->getGUI());
			tempGarrisonWrapper->setIsRenderableWhenNotOwnedByPlayer(false);
			for (int i = 0; i < 10; i++) {
				if (unitNames[i] == "") continue;
				
				Unit* currentUnit = Global::unitHandler->getUnit(unitNames[i], unitLevels[i]);
				
				//Adding items to units
				for (int j = 0; j < 4; j++) {
					currentUnit->addItem(Global::itemHandler->getItem(unitInventory[i * 4 + j]));
				}
				
				currentUnit->recalculateInventory();
				
				currentUnit->statsWithItems["currentLife"] -= unitDamages[i];
				currentUnit->statsWithItems["currentExperience"] += unitExperiences[i];
				
				tempGarrisonWrapper->getGarrison()->getArmy()->setUnit(i, currentUnit);
			}
			loaded->getGUI()->addPart({"Garrison", tempGarrisonWrapper});
		}
		if (hasTax) {
			TaxCollectorWrapper* tempTaxCollectorWrapper = new TaxCollectorWrapper(loaded->getGUI());
			tempTaxCollectorWrapper->getTaxCollector()->setMaxAccumulableGold(maxAccGold);
			tempTaxCollectorWrapper->getTaxCollector()->addAccumulatedGold(curAccGold);
			loaded->getGUI()->addPart({"Collect Tax", tempTaxCollectorWrapper});
		}
		if (hasPar) {
			loaded->setTempParentName(parentName);
		}
		
		loaded->setScale(scale);
		//Error checking for position
		if (Global::map->getTile(posX, posY) == NULL) throw std::runtime_error("Invalid world object position (" + interactiveNames[i] + ", (" + std::to_string(posX) + ";" + std::to_string(posY) + "))");
		Global::map->getTile(posX, posY)->entities.push_back(loaded);
		interactives.push_back(loaded);
		
		if (interactivesByName.find(name) != interactivesByName.end()) {
			throw std::runtime_error("Duplicate name: \"" + name + "\"");
		}
		interactivesByName[name] = loaded;
	}
}

void WorldObjectHandler::setParentRelations() {
	for (unsigned int i = 0; i < interactives.size(); i++) {
		std::string parentName = interactives[i]->getTempParentName();
		if (parentName != "") {
			if (interactivesByName.find(parentName) != interactivesByName.end()) {
				InteractiveWorldObject* parent = interactivesByName[parentName];
				interactives[i]->setParent(parent);
				parent->addChild(interactives[i]);
			} else {
				std::clog << "Warning: parent not found: " << parentName << std::endl;
			}
		}
	}
}
