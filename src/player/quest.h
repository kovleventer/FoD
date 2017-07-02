#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "item.h"
#include "../gui/popup.h"

//Forward declaration
class Quest;
class NPC;
class InteractiveWorldObject;

/*!
 * @author kovlev
 */


/*!
 * @enum QuestTrigger
 * Which way is the quest triggered (started)
 */
enum class QuestTrigger {
	TALK_WITH_NPC,
	STRUCTURE_CAPTURE,
	TIME,
	QUEST_COMPLETION
};


/*!
 * @enum QuestObjective
 * What needs to be done
 */
enum class QuestObjective {
	TALK_WITH_NPC,
	KILL_NPC,
	VISIT_STRUCTURE
};


/*!
 * @enum QuestState
 * The state of the quest
 */
enum class QuestState {
	NOT_YET_STARTED,
	IN_PROGRESS,
	COMPLETED
};


/*!
 * @class Quest
 * Represents a quest
 * A quest is triggerable, has different objectives and rewards
 * Normal quest lifetime:
 * NOT_YET_STARTED ---Trigger activated--> IN_PROGRESS ---Objective completed, reward added--> COMPLETED
 */
class Quest {
	friend class QuestHandler;
public:
	Quest(std::string n, std::string d, QuestTrigger qt, QuestObjective qo, QuestState qs);
	
	//Getters
	std::string getName();
	std::string getDescription();
	QuestTrigger getQuestTrigger();
	QuestObjective getQuestObjective();
	QuestState getQuestState();
	NPC* getQTTalkTarget();
	InteractiveWorldObject* getQTStructTarget();
	int getQTTimeInTicks();
	Quest* getQTPreviousQuest();
	NPC* getQOTalkTarget();
	NPC* getQOKillTarget();
	InteractiveWorldObject* getQOStructTarget();
	int getRewardGold();
	unsigned int getRewardItemsSize();
	Item* getRewardItem(unsigned int index);
	
	//Setters
	void setQuestState(QuestState newQuestState);
	void setRewardGold(int newRewardGold);
	void addRewardItem(Item* rewardItemToAdd);
	
	//Checks whether the questState is correct
	void start();
	void complete();
private:
	std::string name;
	std::string description;
	
	QuestTrigger questTrigger;
	QuestObjective questObjective;
	QuestState questState;
	
	//Different quest trigger informations
	NPC* qtTalkTarget;
	InteractiveWorldObject* qtStructTarget;
	int qtTimeInTicks;
	Quest* qtPreviousQuest;
	std::vector<Quest*> qtNextQuests;
	
	//Different quest objective informations
	NPC* qoTalkTarget;
	NPC* qoKillTarget;
	InteractiveWorldObject* qoStructTarget;
	
	int rewardGold;
	std::vector<Item*> rewardItems;
};
