#include "quest.h"

/*!
 * @author kovlev
 */

Quest::Quest(std::string n, std::string d, QuestTrigger qt, QuestObjective qo) {
	name = n;
	description = d;
	questTrigger = qt;
	questObjective = qo;
	questState = QuestState::NOT_YET_STARTED;
	
	qtTalkTarget = NULL;
	qtStructTarget = NULL;
	qtTimeInTicks = 0;
	qtPreviousQuest = NULL;
	qoTalkTarget = NULL;
	qoKillTarget = NULL;
	qoStructTarget = NULL;
	rewardGold = 0;
}

std::string Quest::getName() {
	return name;
}

std::string Quest::getDescription() {
	return description;
}

QuestTrigger Quest::getQuestTrigger() {
	return questTrigger;
}

QuestObjective Quest::getQuestObjective() {
	return questObjective;
}

QuestState Quest::getQuestState() {
	return questState;
}

NPC* Quest::getQTTalkTarget() {
	if (questTrigger == QuestTrigger::TALK_WITH_NPC) {
		return qtTalkTarget;
	}
	std::clog << "Warning: Tried to access invalid trigger at " << name << std::endl;
	return NULL;
}

InteractiveWorldObject* Quest::getQTStructTarget() {
	if (questTrigger == QuestTrigger::STRUCTURE_CAPTURE) {
		return qtStructTarget;
	}
	std::clog << "Warning: Tried to access invalid trigger at " << name << std::endl;
	return NULL;
}

int Quest::getQTTimeInTicks() {
	if (questTrigger == QuestTrigger::TIME) {
		return qtTimeInTicks;
	}
	std::clog << "Warning: Tried to access invalid trigger at " << name << std::endl;
	return 0;
}

Quest* Quest::getQTPreviousQuest() {
	if (questTrigger == QuestTrigger::QUEST_COMLETION) {
		return qtPreviousQuest;
	}
	std::clog << "Warning: Tried to access invalid trigger at " << name << std::endl;
	return NULL;
}

NPC* Quest::getQOTalkTarget() {
	if (questObjective == QuestObjective::TALK_WITH_NPC) {
		return qoTalkTarget;
	}
	std::clog << "Warning: Tried to access invalid objective at " << name << std::endl;
	return NULL;
}

NPC* Quest::getQOKillTarget() {
	if (questObjective == QuestObjective::KILL_NPC) {
		return qoKillTarget;
	}
	std::clog << "Warning: Tried to access invalid objective at " << name << std::endl;
	return NULL;
}

InteractiveWorldObject* Quest::getQOStructTarget() {
	if (questObjective == QuestObjective::VISIT_STRUCTURE) {
		return qoStructTarget;
	}
	std::clog << "Warning: Tried to access invalid objective at " << name << std::endl;
	return NULL;
}

int Quest::getRewardGold() {
	return rewardGold;
}

unsigned int Quest::getRewardItemsSize() {
	return rewardItems.size();
}

Item* Quest::getRewardItem(unsigned int index) {
	if (index < rewardItems.size()) {
		return rewardItems[index];
	}
	return NULL;
}

void Quest::setQuestState(QuestState newQuestState) {
	questState = newQuestState;
}

void Quest::setQTTalkTarget(NPC* newQTTalkTarget) {
	if (questTrigger == QuestTrigger::TALK_WITH_NPC) {
		qtTalkTarget = newQTTalkTarget;
	} else {
		std::clog << "Warning: Tried to modify invalid trigger at " << name << std::endl;
	}
}

void Quest::setQTStructTarget(InteractiveWorldObject* newQTStructTarget) {
	if (questTrigger == QuestTrigger::STRUCTURE_CAPTURE) {
		qtStructTarget = newQTStructTarget;
	} else {
		std::clog << "Warning: Tried to modify invalid trigger at " << name << std::endl;
	}
}

void Quest::setQTTimeInTicks(int newQTTimeInTicks) {
	if (questTrigger == QuestTrigger::TIME) {
		qtTimeInTicks = newQTTimeInTicks;
	} else {
		std::clog << "Warning: Tried to modify invalid trigger at " << name << std::endl;
	}
}

void Quest::setQTPreviousQuest(Quest* newQTPreviousQuest) {
	if (questTrigger == QuestTrigger::QUEST_COMLETION) {
		qtPreviousQuest = newQTPreviousQuest;
	} else {
		std::clog << "Warning: Tried to modify invalid trigger at " << name << std::endl;
	}
}

void Quest::setQOTalkTarget(NPC* newQOTalkTarget) {
	if (questObjective == QuestObjective::TALK_WITH_NPC) {
		qoTalkTarget = newQOTalkTarget;
	} else {
		std::clog << "Warning: Tried to modify invalid objective at " << name << std::endl;
	}
}

void Quest::setQOKillTarget(NPC* newQOKillTarget) {
	if (questObjective == QuestObjective::KILL_NPC) {
		qoKillTarget = newQOKillTarget;
	} else {
		std::clog << "Warning: Tried to modify invalid objective at " << name << std::endl;
	}
}

void Quest::setQOStructTarget(InteractiveWorldObject* newQOStructTarget) {
	if (questObjective == QuestObjective::VISIT_STRUCTURE) {
		qoStructTarget = newQOStructTarget;
	} else {
		std::clog << "Warning: Tried to modify invalid objective at " << name << std::endl;
	}
}

void Quest::setRewardGold(int newRewardGold) {
	rewardGold = newRewardGold;
}

void Quest::addRewardItem(Item* rewardItemToAdd) {
	rewardItems.push_back(rewardItemToAdd);
}
