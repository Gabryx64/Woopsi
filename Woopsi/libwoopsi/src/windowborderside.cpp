#include "windowborderside.h"
#include "graphicsport.h"

using namespace WoopsiUI;

WindowBorderSide::WindowBorderSide(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_BORDERLESS) {

	_flags.hasFocus = true;
	_flags.decoration = true;
}

void WindowBorderSide::draw(Rect clipRect) {

	// Choose a colour depending on parent's active state
	u16 colour = getFillColour();
	if (_parent != NULL) {
		colour = _parent->hasFocus() ? getHighlightColour() : getFillColour();
	}

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Left bevel background
	port->drawFilledRect(1, 0, _width - 1, _height, colour);

	// Left
	port->drawVertLine(0, 0, _height, getShineColour());

	// Right
	port->drawVertLine(_width - 1, 0, _height, getShadowColour());

	delete port;
}

bool WindowBorderSide::focus() {
	return true;
}

bool WindowBorderSide::blur() {
	return true;
}
