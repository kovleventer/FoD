#include "worldobjecthandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

WorldObjectHandler::WorldObjectHandler() {
	worldObjectBasePath = "data/map/wo/";
	impassableBasePath = "data/map/imp/";
	interactiveBasePath = "data/map/int/";
}

WorldObjectHandler::~WorldObjectHandler() {
	//Deleting WorldObjects
	for (unsigned int i = 0; i < worldObjects.size(); i++) {
		delete worldObjects[i];
	}
	
	//Deleting ImpassableWorldObjects
	for (unsigned int i = 0; i < impassables.size(); i++) {
		delete impassables[i];
	}
	
	//Deleting InteractiveWorldObjects
	for (unsigned int i = 0; i < interactives.size(); i++) {
		delete interactives[i];
	}
}

void WorldObjectHandler::loadAll() {
	loadWorldObjects();
	loadImpassableWorldObjects();
	loadInteractiveWorldObjects();
}

void WorldObjectHandler::setOwnershipRelations() {
	for (unsigned int i = 0; i < interactives.size(); i++) {
		interactives[i]->setOwner(Global::npcHandler->getCharacterByName(interactives[i]->tempOwnerHolder));
	}
}

void WorldObjectHandler::loadWorldObjects() {
	std::vector<std::string> worldObjectNames = FilesystemHandler::getFilesInDir(worldObjectBasePath);
	
	//NOTE uses file IO
	std::fstream file;
	for (unsigned int i = 0; i < worldObjectNames.size(); i++) {
		file.open(worldObjectBasePath + worldObjectNames[i], std::ios::in);
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
		
		WorldObject* loaded = new WorldObject(textureName, posX, posY);
		loaded->setScale(scale);
		Global::map->getTile(posX, posY)->backgroundEntities.push_back(loaded);
		worldObjects.push_back(loaded);
		
		file.close();
	}
}

void WorldObjectHandler::loadImpassableWorldObjects() {
	std::vector<std::string> impassableNames = FilesystemHandler::getFilesInDir(impassableBasePath);
	
	//NOTE uses file IO
	std::fstream file;
	for (unsigned int i = 0; i < impassableNames.size(); i++) {
		file.open(impassableBasePath + impassableNames[i], std::ios::in);
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
		for (int i = 0; i < impLen; i++) {
			int x, y;
			file >> x;
			file >> y;
			impassableTiles.push_back(Point(x, y));
		}
		
		double scale;
		file >> scale;
		
		ImpassableWorldObject* loaded = new ImpassableWorldObject(textureName, posX, posY, impassableTiles);
		loaded->setScale(scale);
		Global::map->getTile(posX, posY)->entities.push_back(loaded);
		impassables.push_back(loaded);
		
		file.close();
	}
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
		
		InteractiveWorldObject* loaded = new InteractiveWorldObject(textureName, posX, posY, interactiveTiles, false);
		loaded->setName(name);
		
		loaded->tempOwnerHolder = ownerName;
		
		//Setting interactive properties
		loaded->getGUI()->addPart({"TEST", new TestGUIPart(loaded->getGUI())});
		loaded->getGUI()->addPart({"TEST2", new TestGUIPart2(loaded->getGUI())});
		loaded->getGUI()->addPart({"TEST3", new TestGUIPart3(loaded->getGUI())});
		
		loaded->setScale(scale);
		Global::map->getTile(posX, posY)->entities.push_back(loaded);
		interactives.push_back(loaded);
		
		file.close();
	}
}
