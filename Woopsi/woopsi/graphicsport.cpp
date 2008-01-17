#include "graphicsport.h"

GraphicsPort::GraphicsPort(Gadget* const gadget, const s16 x, const s16 y, const u16 width, const u16 height, u16* const bitmap, const u16 bitmapWidth, const u16 bitmapHeight, const Gadget::Rect* clipRect) {
	_gadget = gadget;
	_rect.x = x;
	_rect.y = y;
	_rect.width = width;
	_rect.height = height;
	_bitmap = bitmap;
	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;

	// Set up clip rect
	if (clipRect != NULL) {
		_clipRect = new Gadget::Rect;
		_clipRect->height = clipRect->height;
		_clipRect->width = clipRect->width;
		_clipRect->x = clipRect->x;
		_clipRect->y = clipRect->y;
	} else {
		_clipRect = NULL;
	}
}

GraphicsPort::~GraphicsPort() {
	delete _clipRect;
}

// Clip filled rectangle
void GraphicsPort::clipFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour, const Gadget::Rect& clipRect) {

	// Get end point of rect to draw
	s16 x2 = x + width - 1;
	s16 y2 = y + height - 1;

	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {

		// Calculate new width/height
		width = x2 - x + 1;
		height = y2 - y + 1;

		// Adjust for top-screen offset
		if (y > TOP_SCREEN_Y_OFFSET) {
			y -= TOP_SCREEN_Y_OFFSET;
		}

		// Draw the rectangle
		drawClippedFilledRect(x, y, width, height, colour);
	}
}

// Draw pre-clipped rectangles
void GraphicsPort::drawClippedFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	// Draw top line
	drawClippedHorizLine(x, y, width, colour);

	// Calculate last line to draw
	u16 lastY = y + height;

	// Precalculate line values for loop
	u16* line0 = _bitmap + x + (y * SCREEN_WIDTH);
	u16* linei = line0 + SCREEN_WIDTH;
	u16 lineInc = SCREEN_WIDTH;

	// Loop through all lines
	for (u16 i = y + 1; i < lastY; i++) {
		while (DMA_Active());

		DMA_Copy(line0, linei, width, DMA_16NOW);

		// Move to next line
		linei += lineInc;
	}
}

// Print a single character in a specific colour
void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, char letter, u16 colour) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Store current font colour
	bool isMonochrome = font->isMonochrome();
	u16 oldColour = font->getColour();

	// Update font colour
	font->setColour(colour);

	// Output as normal
	drawText(x, y, font, letter);

	// Reset colour
	if (!isMonochrome) {
		font->clearColour();
	} else {
		font->setColour(oldColour);
	}
}

// Print a string in a specific colour
void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, char* string, u16 colour) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Store current font colour
	bool isMonochrome = font->isMonochrome();
	u16 oldColour = font->getColour();

	// Update font colour
	font->setColour(colour);

	// Output as normal
	drawText(x, y, font, string);

	// Reset colour
	if (!isMonochrome) {
		font->clearColour();
	} else {
		font->setColour(oldColour);
	}
}

// Print a single character
void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, char letter) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Convert char to char[]
	char text[2];
	sprintf(text, "%c", letter);

	// Output as normal
	drawText(x, y, font, text);
}

void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, char* string) {
	drawText(x, y, font, strlen(string), string);
}

void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, u16 length, char* string) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	if (_clipRect == NULL) {
		// Draw all visible rects
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipText(x, y, font, length, string, _gadget->getVisibleRectCache()->at(i));
		}
	} else {
		// Draw single rectangle
		clipText(x, y, font, length, string, *_clipRect);
	}
}

// Clip and draw text
void GraphicsPort::clipText(s16 x, s16 y, FontBase* font, u16 length, char* string, const Gadget::Rect& clipRect) {
	s16 clipX1 = clipRect.x;
	s16 clipY1 = clipRect.y;
	s16 clipX2 = clipRect.x + clipRect.width - 1;
	s16 clipY2 = clipRect.y + clipRect.height - 1;

	// Attempt to clip and draw
	if (clipCoordinates(&clipX1, &clipY1, &clipX2, &clipY2, clipRect)) {
		// Compensate for top screen offset
		if (y >= TOP_SCREEN_Y_OFFSET) {
			y -= TOP_SCREEN_Y_OFFSET;
		}
	
		TextWriter::drawString(_bitmap, _bitmapWidth, _bitmapHeight, font, string, length, x, y, clipX1, clipY1, clipX2, clipY2);
	}
}

// Draw a single pixel to the bitmap
void GraphicsPort::drawClippedPixel(u16 x, u16 y, u16 colour) {
	u32 pos = (y * _bitmapWidth) + x;
	_bitmap[pos] = colour;
}

// Draw a horizontal line - internal function
void GraphicsPort::drawClippedHorizLine(s16 x, s16 y, s16 width, u16 colour) {
	if (width-- > 0) {
		// Draw initial pixel
		u16* pos = _bitmap + (y * _bitmapWidth) + x;
		*pos = colour;

		if (width > 0) {
			// Duplicate pixel
			while(DMA_Active());
			DMA_Force(*pos, (pos + 1), width, DMA_16NOW);
		}
	}
}

/// Draw a vertical line - internal function
void GraphicsPort::drawClippedVertLine(s16 x, s16 y, s16 height, u16 colour) {
	for (u16 i = y; i < y + height; i++) {
		drawClippedPixel(x, i, colour);
	}
}

// XOR out a pixel
void GraphicsPort::drawClippedXORPixel(s16 x, s16 y) {
	u16 pixel = (_bitmap[x + (y * _bitmapWidth)] ^ 0xffff) | (1 << 15);
	drawClippedPixel(x, y, pixel);
}

// XOR out a horizontal line
void GraphicsPort::drawClippedXORHorizLine(s16 x, s16 y, u16 width) {
	for (u16 i = 0; i < width; i++) {
		drawClippedXORPixel(x + i, y);
	}
}

// XOR out a vertical line
void GraphicsPort::drawClippedXORVertLine(s16 x, s16 y, u16 height) {
	for (u16 i = 0; i < height; i++) {
		drawClippedXORPixel(x, y + i);
	}
}

// XOR out a rectangle
void GraphicsPort::drawClippedXORRect(s16 x, s16 y, u16 width, u16 height) {

	// Top
	if (((y >= 0) && (y < SCREEN_HEIGHT)) || ((y >= TOP_SCREEN_Y_OFFSET) && (y < TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT))) {
		drawClippedXORHorizLine(x, y, width);
	}

	// Bottom
	if ((y + height < SCREEN_HEIGHT) || ((y >= TOP_SCREEN_Y_OFFSET) && (y + height < TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT))) {
		drawClippedXORHorizLine(x, y + height - 1, width);
	}

	// Left
	if ((x >= 0) && (x < SCREEN_WIDTH)) {
		drawClippedXORVertLine(x, y + 1, height - 2);
	}

	// Right
	if (x + width < SCREEN_WIDTH) {
		drawClippedXORVertLine(x + width - 1, y + 1, height - 2);
	}
}

// Clip vertical line
void GraphicsPort::clipVertLine(s16 x, s16 y, s16 height, u16 colour, const Gadget::Rect& clipRect) {

	// Get end point of rect to draw
	s16 x2 = x;
	s16 y2 = y + height - 1;

	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {

		// Calculate new height
		height = y2 - y + 1;

		// Draw the line
		drawClippedVertLine(x, y, height, colour);
	}
}

// Clip horizontal line
void GraphicsPort::clipHorizLine(s16 x, s16 y, s16 width, u16 colour, const Gadget::Rect& clipRect) {

	// Get end point of rect to draw
	s16 x2 = x + width - 1;
	s16 y2 = y;

	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {

		// Calculate new width
		width = x2 - x + 1;

		// Draw the line
		drawClippedHorizLine(x, y, width, colour);
	}
}

// Clip XORed horizontal line
void GraphicsPort::clipXORHorizLine(s16 x, s16 y, s16 width, const Gadget::Rect& clipRect) {

	// Get end point of rect to draw
	s16 x2 = x + width - 1;
	s16 y2 = y;

	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {

		// Calculate new width
		width = x2 - x + 1;

		// Draw the line
		drawClippedXORHorizLine(x, y, width);
	}
}

// Clip XORed vertical line
void GraphicsPort::clipXORVertLine(s16 x, s16 y, s16 height, const Gadget::Rect& clipRect) {

	// Get end point of rect to draw
	s16 x2 = x;
	s16 y2 = y + height - 1;

	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {

		// Calculate new height
		height = y2 - y + 1;

		// Draw the line
		drawClippedXORVertLine(x, y, height);
	}
}

// Draw horizontal line - external function
void GraphicsPort::drawHorizLine(s16 x, s16 y, s16 width, u16 colour) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	if (_clipRect == NULL) {

		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipHorizLine(x, y, width, colour, _gadget->getVisibleRectCache()->at(i));
		}
	} else {
		// Draw single rectangle
		clipHorizLine(x, y, width, colour, *_clipRect);
	}
}

// Draw vertical line - external function
void GraphicsPort::drawVertLine(s16 x, s16 y, s16 height, u16 colour) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipVertLine(x, y, height, colour, _gadget->getVisibleRectCache()->at(i));
		}
	} else {
		// Draw single rectangle
		clipVertLine(x, y, height, colour, *_clipRect);
	}
}

// Draw filled rectangle - external function
void GraphicsPort::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipFilledRect(x, y, width, height, colour, _gadget->getVisibleRectCache()->at(i));
		}
	} else {
		// Draw single rectangle
		clipFilledRect(x, y, width, height, colour, *_clipRect);
	}
}

// Draw filled circle - external function
void GraphicsPort::drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x0, &y0);

	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipFilledCircle(x0, y0, radius, colour, _gadget->getVisibleRectCache()->at(i));
		}
	} else {
		// Draw single rectangle
		clipFilledCircle(x0, y0, radius, colour, *_clipRect);
	}
}

void GraphicsPort::clipFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour, const Gadget::Rect& clipRect) {
	s16 f = 1 - radius;
	s16 ddF_x = 0;
	s16 ddF_y = -2 * radius;
	s16 x = 0;
	s16 y = radius;

	// Draw central line
	clipHorizLine(x0 - radius, y0, (radius << 1) + 1, colour, clipRect);

	while(x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		clipHorizLine(x0 - x, y0 + y, (x << 1) + 1, colour, clipRect);
		clipHorizLine(x0 - x, y0 - y, (x << 1) + 1, colour, clipRect);

		clipHorizLine(x0 - y, y0 + x, (y << 1) + 1, colour, clipRect);
		clipHorizLine(x0 - y, y0 - x, (y << 1) + 1, colour, clipRect);
	}
}

// Draw rectangle - external function
void GraphicsPort::drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipHorizLine(x, y, width, colour, _gadget->getVisibleRectCache()->at(i));						// Top
			clipHorizLine(x, y + height - 1, width, colour, _gadget->getVisibleRectCache()->at(i));			// Bottom
			clipVertLine(x, y + 1, height - 2, colour, _gadget->getVisibleRectCache()->at(i));				// Left
			clipVertLine(x + width - 1, y + 1, height - 2, colour, _gadget->getVisibleRectCache()->at(i));	// Right
		}
	} else {
		// Draw single rectangle
		clipHorizLine(x, y, width, colour, *_clipRect);						// Top
		clipHorizLine(x, y + height - 1, width, colour, *_clipRect);		// Bottom
		clipVertLine(x, y + 1, height - 2, colour, *_clipRect);				// Left
		clipVertLine(x + width - 1, y + 1, height - 2, colour, *_clipRect);	// Right
	}
}

// Draw bevelled rectangle based on gadget border - external function
void GraphicsPort::drawBevelledRect(s16 x, s16 y, u16 width, u16 height) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Ignore command if gadget is borderless
	if (_gadget->isBorderless()) return;

	// Work out which colours to use
	u16 col1;
	u16 col2;

	// Bevel in or out?
	if ((_gadget->isClicked() && (_gadget->getOutlineType() == Gadget::OUTLINE_CLICK_DEPENDENT)) || (_gadget->getOutlineType() == Gadget::OUTLINE_IN)) {
		// Bevelled into the screen
		col1 = _gadget->getShadowColour();
		col2 = _gadget->getShineColour();
	} else {
		// Bevelled out of the screen
		col1 = _gadget->getShineColour();
		col2 = _gadget->getShadowColour();
	}

	drawBevelledRect(x, y, width, height, col1, col2);
}

// Draw bevelled rectangle - external function
void GraphicsPort::drawBevelledRect(s16 x, s16 y, u16 width, u16 height, u16 shineColour, u16 shadowColour) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipHorizLine(x, y, width, shineColour, _gadget->getVisibleRectCache()->at(i));							// Top
			clipHorizLine(x, y + height - 1, width, shadowColour, _gadget->getVisibleRectCache()->at(i));			// Bottom
			clipVertLine(x, y + 1, height - 2, shineColour, _gadget->getVisibleRectCache()->at(i));					// Left
			clipVertLine(x + width - 1, y + 1, height - 2, shadowColour, _gadget->getVisibleRectCache()->at(i));	// Right
		}
	} else {
		// Draw single rectangle
		clipHorizLine(x, y, width, shineColour, *_clipRect);						// Top
		clipHorizLine(x, y + height - 1, width, shadowColour, *_clipRect);			// Bottom
		clipVertLine(x, y + 1, height - 2, shineColour, *_clipRect);				// Left
		clipVertLine(x + width - 1, y + 1, height - 2, shadowColour, *_clipRect);	// Right
	}
}

// Draw XORed rectangle - external function
void GraphicsPort::drawXORRect(s16 x, s16 y, u16 width, u16 height) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipXORHorizLine(x, y, width, _gadget->getVisibleRectCache()->at(i));						// Top
			clipXORHorizLine(x, y + height - 1, width, _gadget->getVisibleRectCache()->at(i));			// Bottom
			clipXORVertLine(x, y + 1, height - 2, _gadget->getVisibleRectCache()->at(i));				// Left
			clipXORVertLine(x + width - 1, y + 1, height - 2, _gadget->getVisibleRectCache()->at(i));	// Right
		}
	} else {
		// Draw single rectangle
		clipXORHorizLine(x, y, width, *_clipRect);						// Top
		clipXORHorizLine(x, y + height - 1, width, *_clipRect);			// Bottom
		clipXORVertLine(x, y + 1, height - 2, *_clipRect);				// Left
		clipXORVertLine(x + width - 1, y + 1, height - 2, *_clipRect);	// Right
	}
}

//Draw bitmap - external function
void GraphicsPort::drawBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight) {
	
	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	// Ensure width of rect being drawn into does not exceed size of bitmap
	if (bitmapWidth - bitmapX < width) {
		width = bitmapWidth - bitmapX;
	}

	if (bitmapHeight - bitmapY < height) {
		height = bitmapHeight - bitmapY;
	}

	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipBitmap(x, y, width, height, bitmap, bitmapX, bitmapY, bitmapWidth, bitmapHeight, _gadget->getVisibleRectCache()->at(i));
		}
	} else {
		// Draw single rectangle
		clipBitmap(x, y, width, height, bitmap, bitmapX, bitmapY, bitmapWidth, bitmapHeight, *_clipRect);
	}
}

// Draw XORed horizontal line - external function
void GraphicsPort::drawXORHorizLine(s16 x, s16 y, s16 width) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	if (_clipRect == NULL) {

		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipXORHorizLine(x, y, width, _gadget->getVisibleRectCache()->at(i));
		}
	} else {

		// Draw single rectangle
		clipXORHorizLine(x, y, width, *_clipRect);
	}
}

// Draw XORed vertical line - external function
void GraphicsPort::drawXORVertLine(s16 x, s16 y, s16 height) {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	if (_clipRect == NULL) {

		// Draw all visible rectangles
		for (u8 i = 0; i < _gadget->getVisibleRectCache()->size(); i++) {
			clipXORVertLine(x, y, height, _gadget->getVisibleRectCache()->at(i));
		}
	} else {

		// Draw single rectangle
		clipXORVertLine(x, y, height, *_clipRect);
	}
}

void GraphicsPort::clipBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16 bitmapY, u16 bitmapWidth, u16 bitmapHeight, const Gadget::Rect& clipRect) {

	// Get co-ords of screen section we're drawing to
	s16 minX = x;
	s16 minY = y;
	s16 maxX = x + width - 1;
	s16 maxY = y + height - 1;

	// Attempt to clip
	if (clipCoordinates(&minX, &minY, &maxX, &maxY, clipRect)) {

		// Calculate new width and height
		width = maxX - minX + 1;
		height = maxY - minY + 1;

		//Adjust bitmap co-ordinates to allow for clipping changes to visible section
		if (minX > x) {
			bitmapX += minX - x;
		}
		if (y < TOP_SCREEN_Y_OFFSET) {
			if (minY > y) {
				bitmapY += minY - y;
			}
		} else {
			if (minY + TOP_SCREEN_Y_OFFSET > y) {
				bitmapY += (minY + TOP_SCREEN_Y_OFFSET) - y;
			}
		}

		// Draw the bitmap
		drawClippedBitmap(minX, minY, width, height, bitmap, bitmapX, bitmapY, bitmapWidth, bitmapHeight);
	}
}

void GraphicsPort::drawClippedBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight) {

	// Precalculate line values for loop
	u16* srcLine0 = (u16*)bitmap + bitmapX + (bitmapY * bitmapWidth);

	u16* srcLinei = srcLine0;

	u16* destLine0 = _bitmap + x + (y * _bitmapWidth);
	u16* destLinei = destLine0;

	u16 lastLine = y + height;

	for (u16 i = y; i < lastLine; i++) {
		while (DMA_Active());
		DMA_Copy(srcLinei, destLinei, width, DMA_16NOW);

		srcLinei += bitmapWidth;
		destLinei += _bitmapWidth;
	}
}

// Erase by redrawing gadget
void GraphicsPort::clear() {

	// Ignore command if gadget deleted or invisible
	if ((_gadget->isDeleted()) || (!_gadget->isVisible())) return;

	_gadget->draw();
}

// Adjust co-ords from port-space to screen-space
void GraphicsPort::convertPortToScreenSpace(s16* x, s16* y) {
	*x += _rect.x + _gadget->getX();
	*y += _rect.y + _gadget->getY();
}

// Clip co-ordinates
bool GraphicsPort::clipCoordinates(s16* x1, s16* y1, s16* x2, s16* y2, const Gadget::Rect& clipRect) {
	// Get co-ords of clipping rect
	s16 minX = clipRect.x;
	s16 minY = clipRect.y;
	s16 maxX = clipRect.x + clipRect.width - 1;
	s16 maxY = clipRect.y + clipRect.height - 1;

	// Get start and point co-ords of graphics port
	s16 portX1 = getX();
	s16 portY1 = getY();
	s16 portX2 = getX() + _rect.width - 1;
	s16 portY2 = getY() + _rect.height - 1;

	// Choose larger start point values
	minX = minX > portX1 ? minX : portX1;
	minY = minY > portY1 ? minY : portY1;

	// Choose smaller end point values
	maxX = maxX < portX2 ? maxX : portX2;
	maxY = maxY < portY2 ? maxY : portY2;

	// Ensure values don't exceed clipping rectangle
	*x1 = *x1 > minX ? *x1 : minX;
	*y1 = *y1 > minY ? *y1 : minY;
	*x2 = *x2 < maxX ? *x2 : maxX;
	*y2 = *y2 < maxY ? *y2 : maxY;

	// Compensate for top screen offset
	if (*y1 >= TOP_SCREEN_Y_OFFSET) {
		*y1 -= TOP_SCREEN_Y_OFFSET;
		*y2 -= TOP_SCREEN_Y_OFFSET;
	}

	// Ensure Y values don't exceed screen dimensions
	if (*y1 > SCREEN_HEIGHT) {
		return false;
	}
	if (*y2 >= SCREEN_HEIGHT) {
		*y2 = SCREEN_HEIGHT - 1;
	}

	// Return false if no box to draw
	if ((*x2 < *x1) || (*y2 < *y1)) {
		return false;
	}

	// Return true as box can be drawn
	return true;
}

s16 GraphicsPort::getX() {
	return _rect.x + _gadget->getX();
}

s16 GraphicsPort::getY() {
	return _rect.y + _gadget->getY();
}
