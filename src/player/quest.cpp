#include "quest.h"

#include "../core/global.h"
#include "../core/questhandler.h"
#include "../map/npc.h"
#include "../map/worldobject.h"

/*!
 * @author kovlev
 */

Quest::Quest(std::string n, std::string d, QuestTrigger qt, QuestObjective qo, QuestState qs) {
	name = n;
	description = d;
	questTrigger = qt;
	questObjective = qo;
	questState = qs;
	
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
	if (questTrigger == QuestTrigger::QUEST_COMPLETION) {
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

void Quest::setRewardGold(int newRewardGold) {
	rewardGold = newRewardGold;
}

void Quest::addRewardItem(Item* rewardItemToAdd) {
	rewardItems.push_back(rewardItemToAdd);
}

void Quest::start() {
	if (questState == QuestState::NOT_YET_STARTED) {
		Popup* popup = new Popup(Popup::DEFAULT_DIM, PopupType::POPUP_OK);
		popup->setText(description);
		Global::guiHandler->addPopup(popup);
		questState = QuestState::IN_PROGRESS;
	}
}

void Quest::complete() {
	if (questState == QuestState::IN_PROGRESS) {
		Popup* popup = new Popup(Popup::DEFAULT_DIM, PopupType::POPUP_OK);
		popup->setItemList(rewardItems);
		Global::guiHandler->addPopup(popup);
		Global::player->giveGold(rewardGold);
		for (unsigned int i = 0; i < rewardItems.size(); i++) {
			Global::player->getInventory()->addItem(rewardItems[i]);
		}
		questState = QuestState::COMPLETED;
	}
}
