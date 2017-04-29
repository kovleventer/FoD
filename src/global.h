#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <map>

#include "point.h"
#include "resourcehandler.h"
#include "map.h"
#include "cursor.h"
#include "camera.h"
#include "player.h"
#include "permanentgui.h"
#include "guihandler.h"
#include "itemhandler.h"
#include "unithandler.h"
#include "minimap.h"
#include "npchandler.h"
#include "worldobjecthandler.h"

//NOTE a better way would be using namespaces
//https://stackoverflow.com/questions/2483978/best-way-to-implement-globally-scoped-data
class Global {
public:
	//NOTE we are not using private variables with getters/setters
	static int screenWidth;
	static int screenHeight;
	static std::string appName;
	static int fps;
	
	static int gameBoardWidth;
	static int gameBoardHeight;
	static int tileSize;
	static int defaultFontSize;
	
	static SDL_Renderer* renderer;
	static SDL_Window* window;
	
	static ResourceHandler* resourceHandler;
	static ItemHandler* itemHandler;
	static UnitHandler* unitHandler;
	static NPCHandler* npcHandler;
	static WorldObjectHandler* worldObjectHandler;
	static Map* map;
	static Minimap* minimap;
	static Cursor* cursor;
	static Camera* camera;
	static Player* player;
	
	static PermanentGUI* permaGUI;
	static GUIHandler* guiHandler;
};
