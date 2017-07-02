#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <map>

#include "../util/filesystemhandler.h"
#include "../player/quest.h"
#include "../util/stdextension.h"

/*!
 * @author kovlev
 */

/*!
 * @class QuestTimeComparator
 * Compares two quests by their trigger time value
 * Cannod reside in quest.h
 */
class QuestTimeComparator {
public:
	bool operator()(Quest* a, Quest* b);
};

/*!
 * @class QuestHandler 
 * Handles and loads quests
 */
class QuestHandler {
public:
	QuestHandler();
	~QuestHandler();
	
	void loadAll();
	
	//Varios storage methods
	std::priority_queue<Quest*, std::vector<Quest*>, QuestTimeComparator> qtTimePQ;
	
	Quest* getQuestByName(std::string name);
	
	//Translater
	QuestTrigger translateQT(std::string questTriggerString);
	std::string translateQT(QuestTrigger questTrigger);
	
	QuestObjective translateQO(std::string questObjectiveString);
	std::string translateQO(QuestObjective questObjective);
	
	QuestState translateQS(std::string questStateString);
	std::string translateQS(QuestState questState);
private:
	std::string questPath;
	
	//Not accessable
	//Used only at deletion
	std::vector<Quest*> backgroundStorage;
	
	std::map<std::string, Quest*> questsByName;
	
	//Translater stuff
	std::map<std::string, QuestTrigger> translaterSQT;
	std::map<QuestTrigger, std::string> translaterQTS;
	
	std::map<std::string, QuestObjective> translaterSQO;
	std::map<QuestObjective, std::string> translaterQOS;
	
	std::map<std::string, QuestState> translaterSQS;
	std::map<QuestState, std::string> translaterQSS;
	
	void translaterInit();
};
