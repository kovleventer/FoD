#pragma once

#include "npc.h"
#include "wholescreengui.h"

class Battle {
public:
	//Quick Battle
	Battle(NPC* npc1, NPC* npc2);
	
	//Interactive Battle with player
	Battle(NPC* enemy);
	
	~Battle();
private:
	WholeScreenGUI* gui;
};
