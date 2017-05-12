#include <iostream>
#include <string>

#include "core/game.h"
#include "util/version.h"

/*!
 * @author kovlev
 * @date 2016-11-28
 */

const std::string APPNAME = "Fall of Dyrangor";
const Version VERSION = Version("1.1.4.0");

int main (int argc, char* args[]) {
	Game* game;
	
	//Init
	game = new Game(APPNAME, VERSION);
	
	//Not needed possibly
	delete game;
	
	return 0;
}
