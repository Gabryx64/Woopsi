#ifndef _SCROLLABLE_BASE_H_
#define _SCROLLABLE_BASE_H_

#include <nds.h>

/**
 * Abstract class providing some basic functionality required by scrolling gadgets.
 * Scrolling regions are modelled as a virtual "canvas", or rectangular region, with
 * height/width dimensions and x/y co-ordinates.
 */
class ScrollableBase {
public:
	/**
	 * Constructor.
	 */
	ScrollableBase() { }
	
	/**
	 * Destructor.
	 */
	virtual ~ScrollableBase() { }

	/**
	 * Gets the x co-ordinate of the virtual canvas.
	 * @return The x co-ordinate of the virtual canvas.
	 */
	virtual const s32 getCanvasX() const { return _canvasX; };
	
	/**
	 * Gets the y co-ordinate of the virtual canvas.
	 * @return The y co-ordinate of the virtual canvas.
	 */
	virtual const s32 getCanvasY() const { return _canvasY; };

	/**
	 * Gets the width of the virtual canvas.
	 * @return The width of the virtual canvas.
	 */
	virtual const s32 getCanvasWidth() const { return _canvasWidth; };
	
	/**
	 * Gets the height of the virtual canvas.
	 * @return The height of the virtual canvas.
	 */
	virtual const s32 getCanvasHeight() const { return _canvasHeight; };

	/**
	 * Scrolls the virtual canvas by the specified amounts.
	 * @param dx Distance to scroll horizontally.
	 * @param dy Distance to scroll vertically.
	 */
	virtual void scroll(s32 dx, s32 dy) = 0;
	
	/**
	 * Repositions the virtual canvas to the specified co-ordinates.
	 * @param x New x co-ordinate of the virtual canvas.
	 * @param y New y co-ordinate of the virtual canvas.
	 */
	virtual void jump(s32 x, s32 y) = 0;

protected:
	s32 _canvasX;
	s32 _canvasY;
	s32 _canvasWidth;
	s32 _canvasHeight;

	/**
	 * Sets the width of the virtual canvas.
	 * @param The width of the virtual canvas.
	 */
	virtual void setCanvasWidth(const s32 width) { _canvasWidth = width; };
	
	/**
	 * Sets the height of the virtual canvas.
	 * @param The height of the virtual canvas.
	 */
	virtual void setCanvasHeight(const s32 height) { _canvasHeight = height; };
};

#endif
