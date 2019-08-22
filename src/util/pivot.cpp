#include "pivot.h"

void PivotRender::render(ATexture* text, Pivot pivot, Point point) {
	Dimension d = text->getDimensions();
	SDL_Rect destinationRect = { point.getX(), point.getY(), d.W(), d.H() };
	switch (pivot) {
		case Pivot::LEFT_TOP:
			// Do nothing
			break;
		case Pivot::TOP:
			destinationRect.x -= d.W() / 2;
			break;
		case Pivot::RIGHT_TOP:
			destinationRect.x -= d.W();
		case Pivot::LEFT:
			destinationRect.y -= d.H() / 2;
			break;
		case Pivot::CENTER:
			destinationRect.x -= d.W() / 2;
			destinationRect.y -= d.H() / 2;
			break;
		case Pivot::RIGHT:
			destinationRect.x -= d.W();
			destinationRect.y -= d.H() / 2;
			break;
		case Pivot::LEFT_BOT:
			destinationRect.y -= d.H();
			break;
		case Pivot::BOT:
			destinationRect.x -= d.W() / 2;
			destinationRect.y -= d.H();
			break;
		case Pivot::RIGHT_BOT:
			destinationRect.x -= d.W();
			destinationRect.y -= d.H();
			break;
	}
	text->render(destinationRect);
}
