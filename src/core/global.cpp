#include "global.h"

/*!
 * @author kovlev
 */

//Redundant stuff
int Global::screenWidth = 0;
int Global::screenHeight = 0; 
std::string Global::appName = "";
int Global::fps = 0;
int Global::ticks = 0;

int Global::gameBoardWidth = 0; 
int Global::gameBoardHeight = 0; 
int Global::tileSize = 0; 
int Global::defaultFontSize = 0; 

SDL_Renderer* Global::renderer = NULL;
SDL_Window* Global::window = NULL;

ResourceHandler* Global::resourceHandler = NULL;
AudioHandler* Global::audioHandler = NULL;
ItemHandler* Global::itemHandler = NULL;
UnitHandler* Global::unitHandler = NULL;
NPCHandler* Global::npcHandler = NULL;
WorldObjectHandler* Global::worldObjectHandler = NULL;
Map* Global::map = NULL;
Minimap* Global::minimap = NULL;
Cursor* Global::cursor = NULL;
Camera* Global::camera = NULL;
Player* Global::player = NULL;

PermanentGUI* Global::permaGUI = NULL;
GUIHandler* Global::guiHandler = NULL;

AnimationHandler* Global::animationHandler = NULL;
