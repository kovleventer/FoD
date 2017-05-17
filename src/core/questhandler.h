#pragma once

#include <string>

/*!
 * @author kovlev
 */

/*!
 * @class QuestHandler handles quests, WIP
 */
class QuestHandler {
public:
	QuestHandler();
	~QuestHandler();
	
	void loadAll();
private:
	std::string questPath;
};
