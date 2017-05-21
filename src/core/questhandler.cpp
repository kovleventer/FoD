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
				loaded->qtTalkTarget = (NPC*)Global::npcHandler->getCharacterByName(triggerTargetName);
				break; }
			case QuestTrigger::STRUCTURE_CAPTURE: {
				std::string triggerTargetName;
				std::getline(file, triggerTargetName);
				loaded->qtStructTarget = Global::worldObjectHandler->getInteractiveByName(triggerTargetName);
				break; }
			case QuestTrigger::TIME: {
				std::string triggerTimeString;
				std::getline(file, triggerTimeString);
				loaded->qtTimeInTicks = std::stoi(triggerTimeString);
				break; }
			case QuestTrigger::QUEST_COMPLETION: {
				//TODO implementation
				//That is going to be a hard one
				break; }
		}
		
		switch(questObjective) {
			case QuestObjective::TALK_WITH_NPC: {
				std::string objectiveTargetName;
				std::getline(file, objectiveTargetName);
				loaded->qoTalkTarget = (NPC*)Global::npcHandler->getCharacterByName(objectiveTargetName);
				break; }
			case QuestObjective::KILL_NPC: {
				std::string objectiveTargetName;
				std::getline(file, objectiveTargetName);
				loaded->qoKillTarget = (NPC*)Global::npcHandler->getCharacterByName(objectiveTargetName);
				break; }
			case QuestObjective::VISIT_STRUCTURE: {
				std::string objectiveTargetName;
				std::getline(file, objectiveTargetName);
				loaded->qoStructTarget = Global::worldObjectHandler->getInteractiveByName(objectiveTargetName);
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
				//TODO implementation
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
