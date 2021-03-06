#include "questhandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

bool QuestTimeComparator::operator()(Quest* a, Quest* b) {
	return (a->getQTTimeInTicks() < b->getQTTimeInTicks());
}

QuestHandler::QuestHandler() {
	questPath = "data/quest/";
	translaterInit();
}

QuestHandler::~QuestHandler() {
	//Deletes quest
	stdex::clear_ptr_vec(backgroundStorage);
}

void QuestHandler::loadAll() {
	std::vector<std::string> questNames = FilesystemHandler::getFilesInDir(questPath);
	
	//Key depends on value
	std::map<std::string, std::string> tempQuestRelationStorage;
	
	//NOTE uses file IO
	std::fstream file;
	for (unsigned int i = 0; i < questNames.size(); i++) {
		file.open(questPath + questNames[i], std::ios::in);
		//File pattern:
		//Name
		//Description on one line
		//Quest trigger
		//Quest objective
		//Quest state
		//Quest trigger information
		//Quest objective information
		//Gold Reward
		//Itemcount [Itemname] itemcount times
		std::string name;
		std::getline(file, name);
		
		std::string description;
		std::getline(file, description);
		
		std::string questTriggerString;
		std::getline(file, questTriggerString);
		QuestTrigger questTrigger = translateQT(questTriggerString);
		
		std::string questObjectiveString;
		std::getline(file, questObjectiveString);
		QuestObjective questObjective = translateQO(questObjectiveString);
		
		std::string questStateString;
		std::getline(file, questStateString);
		QuestState questState = translateQS(questStateString);
		
		if (!file) {
			//If the file pattern is wrong
			std::clog << "Error! Skipping " << questNames[i] << std::endl;
			continue;
		}
		
		Quest* loaded = new Quest(name, description, questTrigger, questObjective, questState);
		backgroundStorage.push_back(loaded);
		
		switch(questTrigger) {
			case QuestTrigger::TALK_WITH_NPC: {
				std::string triggerTargetName;
				std::getline(file, triggerTargetName);
				NPC* triggerTarget = dynamic_cast<NPC*>(Global::npcHandler->getCharacterByName(triggerTargetName));
				if (triggerTarget == NULL) {
					std::clog << "Error! Skipping " << questNames[i] << std::endl;
					continue;
				}
				loaded->qtTalkTarget = triggerTarget;
				break; }
			case QuestTrigger::STRUCTURE_CAPTURE: {
				std::string triggerTargetName;
				std::getline(file, triggerTargetName);
				InteractiveWorldObject* triggerTarget = Global::worldObjectHandler->getInteractiveByName(triggerTargetName);
				if (triggerTarget == NULL) {
					std::clog << "Error! Skipping " << questNames[i] << std::endl;
					continue;
				}
				loaded->qtStructTarget = triggerTarget;
				break; }
			case QuestTrigger::TIME: {
				std::string triggerTimeString;
				std::getline(file, triggerTimeString);
				//FIXME the exception this might throw somewhy not cached
				loaded->qtTimeInTicks = std::stoi(triggerTimeString);
				break; }
			case QuestTrigger::QUEST_COMPLETION: {
				std::string triggerQuestString;
				std::getline(file, triggerQuestString);
				tempQuestRelationStorage[name] = triggerQuestString;
				break; }
		}
		
		switch(questObjective) {
			case QuestObjective::TALK_WITH_NPC: {
				std::string objectiveTargetName;
				std::getline(file, objectiveTargetName);
				NPC* objectiveTarget = dynamic_cast<NPC*>(Global::npcHandler->getCharacterByName(objectiveTargetName));
				if (objectiveTarget == NULL) {
					std::clog << "Error! Skipping " << questNames[i] << std::endl;
					continue;
				}
				loaded->qoTalkTarget = objectiveTarget;
				break; }
			case QuestObjective::KILL_NPC: {
				std::string objectiveTargetName;
				std::getline(file, objectiveTargetName);
				NPC* objectiveTarget = dynamic_cast<NPC*>(Global::npcHandler->getCharacterByName(objectiveTargetName));
				if (objectiveTarget == NULL) {
					std::clog << "Error! Skipping " << questNames[i] << std::endl;
					continue;
				}
				loaded->qoKillTarget = objectiveTarget;
				break; }
			case QuestObjective::VISIT_STRUCTURE: {
				std::string objectiveTargetName;
				std::getline(file, objectiveTargetName);
				InteractiveWorldObject* objectiveTarget = Global::worldObjectHandler->getInteractiveByName(objectiveTargetName);
				if (objectiveTarget == NULL) {
					std::clog << "Error! Skipping " << questNames[i] << std::endl;
					continue;
				}
				loaded->qoStructTarget = objectiveTarget;
				break; }
		}
		
		file >> loaded->rewardGold;
		
		int itemCount;
		file >> itemCount;
		for (int i = 0; i < itemCount; i++) {
			std::string itemName;
			file >> std::quoted(itemName);
			loaded->addRewardItem(Global::itemHandler->getItem(itemName));
		}
		
		file.close();
		
		if (!file) {
			//If the file pattern is wrong (checking again)
			std::clog << "Error! Skipping " << questNames[i] << std::endl;
			continue;
		}
		
		//NOTE repeating switches are ugly
		//Setting proper targeting
		switch(questTrigger) {
			case QuestTrigger::TALK_WITH_NPC:
				loaded->getQTTalkTarget()->addQuestTriggerTalk(loaded);
				break;
			case QuestTrigger::STRUCTURE_CAPTURE:
				loaded->getQTStructTarget()->addQuestTriggerCapture(loaded);
				break;
			case QuestTrigger::TIME:
				if (loaded->getQTTimeInTicks() >= Global::tickHandler->getGameTicks()) {
					qtTimePQ.push(loaded);
				}
				break;
			case QuestTrigger::QUEST_COMPLETION:
				//Targeting added below
				break;
		}
		
		switch(questObjective) {
			case QuestObjective::TALK_WITH_NPC:
				loaded->getQOTalkTarget()->addQuestObjectiveTalk(loaded);
				break;
			case QuestObjective::KILL_NPC:
				loaded->getQOKillTarget()->addQuestObjectiveKill(loaded);
				break;
			case QuestObjective::VISIT_STRUCTURE:
				loaded->getQOStructTarget()->addQuestObjectiveVisit(loaded);
				break;
		}
		
		Global::player->getQuestLister()->addQuest(loaded);
		questsByName[loaded->getName()] = loaded;
	}
	
	for (auto it = tempQuestRelationStorage.begin(); it != tempQuestRelationStorage.end(); it++) {
		Quest* current = getQuestByName(it->first);
		Quest* previous = getQuestByName(it->second);
		if (current == NULL || previous == NULL) {
			std::clog << "Warning: Invalid quest names: [" << it->first << "] [" << it->second << "]" << std::endl;
			continue;
		}
		
		current->qtPreviousQuest = previous;
		previous->qtNextQuests.push_back(current);
	}
}

Quest* QuestHandler::getQuestByName(std::string name) {
	auto it = questsByName.find(name);
	if(it == questsByName.end()) {
		return NULL;
	} else {
		return questsByName[name];
	}
}

QuestTrigger QuestHandler::translateQT(std::string questTriggerString) {
	return translaterSQT[questTriggerString];
}

std::string QuestHandler::translateQT(QuestTrigger questTrigger) {
	return translaterQTS[questTrigger];
}
	
QuestObjective QuestHandler::translateQO(std::string questObjectiveString) {
	return translaterSQO[questObjectiveString];
}

std::string QuestHandler::translateQO(QuestObjective questObjective) {
	return translaterQOS[questObjective];
}
	
QuestState QuestHandler::translateQS(std::string questStateString) {
	return translaterSQS[questStateString];
}

std::string QuestHandler::translateQS(QuestState questState) {
	return translaterQSS[questState];
}

void QuestHandler::translaterInit() {
	translaterSQT["TALK_WITH_NPC"] = QuestTrigger::TALK_WITH_NPC;
	translaterSQT["STRUCTURE_CAPTURE"] = QuestTrigger::STRUCTURE_CAPTURE;
	translaterSQT["TIME"] = QuestTrigger::TIME;
	translaterSQT["QUEST_COMPLETION"] = QuestTrigger::QUEST_COMPLETION;
	
	translaterQTS[QuestTrigger::TALK_WITH_NPC] = "TALK_WITH_NPC";
	translaterQTS[QuestTrigger::STRUCTURE_CAPTURE] = "STRUCTURE_CAPTURE";
	translaterQTS[QuestTrigger::TIME] = "TIME";
	translaterQTS[QuestTrigger::QUEST_COMPLETION] = "QUEST_COMPLETION";
	
	
	translaterSQO["TALK_WITH_NPC"] = QuestObjective::TALK_WITH_NPC;
	translaterSQO["KILL_NPC"] = QuestObjective::KILL_NPC;
	translaterSQO["VISIT_STRUCTURE"] = QuestObjective::VISIT_STRUCTURE;
	
	translaterQOS[QuestObjective::TALK_WITH_NPC] = "TALK_WITH_NPC";
	translaterQOS[QuestObjective::KILL_NPC] = "KILL_NPC";
	translaterQOS[QuestObjective::VISIT_STRUCTURE] = "VISIT_STRUCTURE";
	
	
	translaterSQS["NOT_YET_STARTED"] = QuestState::NOT_YET_STARTED;
	translaterSQS["IN_PROGRESS"] = QuestState::IN_PROGRESS;
	translaterSQS["COMPLETED"] = QuestState::COMPLETED;
	
	translaterQSS[QuestState::NOT_YET_STARTED] = "NOT_YET_STARTED";
	translaterQSS[QuestState::IN_PROGRESS] = "IN_PROGRESS";
	translaterQSS[QuestState::COMPLETED] = "COMPLETED";
}
