#include "worldobject.h"

#include "../core/global.h"
#include "character.h"
#include "../gui/interactivegui.h"
#include "../player/quest.h"
#include "../map/npc.h"

/*!
 * @author kovlev
 */

// WorldObject stuff

WorldObject::WorldObject(std::string text, int x, int y) : WorldObject(text, Point(x, y)) {}

WorldObject::WorldObject(std::string text, Point pos) : MapEntity(pos) {
	texture = Global::resourceHandler->getATexture(TT::WORLDOBJECT, text);
	atBackground = true;
}

WorldObject::~WorldObject() {}


// ImpassableWorldObject stuff

ImpassableWorldObject::ImpassableWorldObject(std::string text, int x, int y, std::vector<Point> impTiles) : ImpassableWorldObject(text, Point(x, y), impTiles) {}

ImpassableWorldObject::ImpassableWorldObject(std::string text, Point pos, std::vector<Point> impTiles) : WorldObject(text, pos) {
	impassableTiles = impTiles;
	atBackground = false;
}

std::vector<Point> ImpassableWorldObject::getImpassableTiles() {
	return impassableTiles;
}


// InteractiveWorldObject stuff

InteractiveWorldObject::InteractiveWorldObject(std::string text, int x, int y, std::vector<Point> intTiles, bool isNeut) :
	InteractiveWorldObject(text, Point(x, y), intTiles, isNeut) {}

InteractiveWorldObject::InteractiveWorldObject(std::string text, Point pos, std::vector<Point> intTiles, bool isNeut) : WorldObject(text, pos) {
	interactiveTiles = intTiles;
	atBackground = false;
	neutral = isNeut;
	name = "";
	owner = NULL;
	
	gui = new InteractiveGUI(Global::permaGUI->getDim());
	gui->setParent(this);
}

InteractiveWorldObject::~InteractiveWorldObject() {
	delete gui;
}

void InteractiveWorldObject::refillStocks() {
	for (unsigned int i = 0; i < gui->getPartCount(); i++) {
		ItemMarket* possibleItemMarket = dynamic_cast<ItemMarket*>(gui->getPart(i));
		if (possibleItemMarket != NULL) {
			possibleItemMarket->getItemBuyingMenu()->refill();
		}
		
		Barracks* possibleBarracks = dynamic_cast<Barracks*>(gui->getPart(i));
		if (possibleBarracks != NULL) {
			possibleBarracks->getUnitBuyingMenu()->refill();
		}
	}
}

std::vector<Point> InteractiveWorldObject::getInteractiveTiles() {
	return interactiveTiles;
}

bool InteractiveWorldObject::isNeutral() {
	return neutral;
}

std::string InteractiveWorldObject::getName() {
	return name;
}

Character* InteractiveWorldObject::getOwner() {
	return owner;
}

InteractiveGUI* InteractiveWorldObject::getGUI() {
	return gui;
}

void InteractiveWorldObject::setName(std::string newName) {
	name = newName;
}

void InteractiveWorldObject::setOwner(Character* newOwner) {
	for (unsigned int i = 0; i < questTriggerCaptures.size(); i++) {
		questTriggerCaptures[i]->start();
	}
	
	//Setting ownership relations at npc side
	if (owner != NULL) {
		owner->removeOwned(this);
	}
	owner = newOwner;
	if (owner != NULL) {
		owner->addOwned(this);
	}
}

void InteractiveWorldObject::activate() {
	if (owner->getName() == "__takeable__") {
		setOwner(Global::player);
	}
	
	NPC* ownerNPC = dynamic_cast<NPC*>(owner);
	if (ownerNPC != NULL && ownerNPC->isEnemy()) {
		//Battle handles itself
		new Battle(this);
	} else {
		for (unsigned int i = 0; i < questObjectiveVisits.size(); i++) {
			questObjectiveVisits[i]->complete();
		}
		
		Global::guiHandler->setGUI(gui);
	}
}

void InteractiveWorldObject::activate(NPC* npc) {
	if (owner->getName() == "__takeable__") {
		setOwner(npc);
	}
	
	NPC* ownerNPC = dynamic_cast<NPC*>(owner);
	if (npc->isEnemy()) {
		//If current npc is an enemy
		if ((ownerNPC != NULL && !ownerNPC->isEnemy())
			|| (owner == Global::player)) {
			//If current structure's owner is hostile towards npc
			new Battle(npc, this);
			return;
		}
	} else {
		//If current npc is not an enemy
		if (ownerNPC->isEnemy()) {
			//If current structure's owner is hostile towards npc
			new Battle(npc, this);
			return;
		}
	}
	
	//NPC interaction for real
	//The NPC will be able to buy and sell items & units
	bool boughtAnything = false;
	for (unsigned int i = 0; i < gui->getPartCount(); i++) {
		//NOTE a bit slow due to the loop
		ItemMarket* possibleItemMarket = dynamic_cast<ItemMarket*>(gui->getPart(i));
		if (possibleItemMarket != NULL) {
			//Traversing items
			ItemBuyingMenu* ibm = possibleItemMarket->getItemBuyingMenu();
			for (unsigned int j = 0; j < ibm->getItemsToSellSize(); j++) {
				Item* currentItem = ibm->getItem(j);
				
				//The NPC does not hesitate, he simply buys all items he can
				if (currentItem->getPrice() <= npc->getGold()) {
					if (npc->getInventory()->addItem(currentItem)) {
						npc->takeGold(currentItem->getPrice());
						ibm->setSelectedItemPosition(j);
						ibm->removeCurrentItem();
						boughtAnything = true;
					}
				}
			}
		}
		
		Barracks* possibleBarracks = dynamic_cast<Barracks*>(gui->getPart(i));
		if (possibleBarracks != NULL) {
			//Traversing units
			UnitBuyingMenu* ubm = possibleBarracks->getUnitBuyingMenu();
			for (unsigned int j = 0; j < ubm->getUnitsToSellSize(); j++) {
				Unit* currentUnit = ubm->getUnit(j);
				
				//The NPC does not hesitate, he simply buys all units he can
				if (currentUnit->statsWithItems["price"] <= npc->getGold()) {
					if (npc->getArmy()->addUnit(currentUnit)) {
						npc->takeGold(currentUnit->statsWithItems["price"]);
						ubm->removeUnit(j);
						boughtAnything = true;
					}
				}
			}
		}
	}
	
	if (boughtAnything) {
		npc->rearrangeArmy();
	}
	
	//Adding few units to garrison
	Army* garrisonArmy = getGarrisonArmy();
	if (garrisonArmy != NULL && owner == npc) {
		int npcUnitCount = 0;
		int garrisonUnitCount = 0;
		for (int i = 0; i < npc->getArmy()->getWidth(); i++) {
			for (int j = 0; j < npc->getArmy()->getHeight(); j++) {
				Unit* currentNPCUnit = npc->getArmy()->getUnit(i, j);
				if (currentNPCUnit != NULL) {
					npcUnitCount++;
				}
				
				Unit* currentGarrUnit = garrisonArmy->getUnit(i, j);
				if (currentGarrUnit != NULL) {
					if (currentGarrUnit->isDead()) {
						delete currentGarrUnit;
						garrisonArmy->setUnit(i, j, NULL);
					} else {
						garrisonUnitCount++;
					}
				}
			}
		}
		
		int diff = npcUnitCount - garrisonUnitCount;
		
		for (int i = 0; i < npc->getArmy()->getWidth(); i++) {
			for (int j = 0; j < npc->getArmy()->getHeight(); j++) {
				if (diff > 0) {
					Unit* currentUnit = npc->getArmy()->getUnit(i, j);
					if (currentUnit != NULL) {
						garrisonArmy->addUnit(currentUnit);
						npc->getArmy()->setUnit(i, j, NULL);
						npcUnitCount--;
						garrisonUnitCount++;
						diff = npcUnitCount - garrisonUnitCount;
					}
				}
				if (diff < 0) {
					Unit* currentUnit = garrisonArmy->getUnit(i, j);
					if (currentUnit != NULL) {
						npc->getArmy()->addUnit(currentUnit);
						garrisonArmy->setUnit(i, j, NULL);
						npcUnitCount++;
						garrisonUnitCount--;
						diff = npcUnitCount - garrisonUnitCount;
					}
				}
			}
		}
	}
}

Army* InteractiveWorldObject::getGarrisonArmy() {
	for (unsigned int i = 0; i < gui->getPartCount(); i++) {
		GarrisonWrapper* possibleGarrison = dynamic_cast<GarrisonWrapper*>(gui->getPart(i));
		if (possibleGarrison != NULL) {
			return possibleGarrison->getGarrison()->getArmy();
		}
	}
	return NULL;
}

void InteractiveWorldObject::clearGarrisonArmy() {
	Army* garrisonArmy = getGarrisonArmy();
	if (garrisonArmy != NULL) {
		//Does NOT save the items belonging to the units
		for (int i = 0; i < garrisonArmy->getWidth(); i++) {
			for (int j = 0; j < garrisonArmy->getHeight(); j++) {
				Unit* currentUnit = garrisonArmy->getUnit(i, j);
				if (currentUnit != NULL) {
					delete currentUnit;
					garrisonArmy->setUnit(i, j, NULL);
				}
			}
		}
	}
}

void InteractiveWorldObject::addQuestTriggerCapture(Quest* questTriggerCaptureToAdd) {
	questTriggerCaptures.push_back(questTriggerCaptureToAdd);
}

void InteractiveWorldObject::addQuestObjectiveVisit(Quest* questObjectiveVisitToAdd) {
	questObjectiveVisits.push_back(questObjectiveVisitToAdd);
}
