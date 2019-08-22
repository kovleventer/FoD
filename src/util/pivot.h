#pragma once

#include <SDL.h>

#include "../core/animatabletexture.h"
#include "point.h"

enum class Pivot {
	LEFT_TOP,
	TOP,
	RIGHT_TOP,
	LEFT,
	CENTER,
	RIGHT,
	LEFT_BOT,
	BOT,
	RIGHT_BOT
};

class PivotRender {
public:
	static void render(ATexture* text, Pivot pivot, Point point);
};
