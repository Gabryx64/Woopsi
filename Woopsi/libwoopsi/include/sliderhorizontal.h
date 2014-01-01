#ifndef _SLIDER_HORIZONTAL_H_
#define _SLIDER_HORIZONTAL_H_

#include <nds.h>
#include "gadget.h"
#include "gadgeteventhandler.h"
#include "sliderbase.h"

namespace WoopsiUI {

	class SliderHorizontalGrip;

	/**
	 * Gadget providing a sliding "grip" that can be moved left and
	 * right in the "gutter".  Essentially a scrollbar, but more
	 * generic.
	 */
	class SliderHorizontal : public SliderBase, public Gadget, public GadgetEventHandler {

	public:

		/**
		 * Constructor.
		 * @param x The x co-ord of the slider, relative to its parent.
		 * @param y The y co-ord of the slider, relative to its parent.
		 * @param width The width of the slider.
		 * @param height The height of the slider.
		 */
		SliderHorizontal(s16 x, s16 y, u16 width, u16 height);

		/**
		 * Get the smallest value that the slider can represent.
		 * @return The smallest value.
		 */
		inline const s16 getMinimumValue() const { return _minimumValue; };

		/**
		 * Get the largest value that the slider can represent.
		 * @return The largest value.
		 */
		inline const s16 getMaximumValue() const { return _maximumValue; };

		/**
		 * Get the current value of the slider.
		 * return The current slider value.
		 */
		const s32 getValue() const;

		/**
		 * Get the value represented by the height of the grip.
		 * For sliders, this would typically be 1 (so each new
		 * grip position is worth 1).  For scrollbars, this
		 * would be the height of the scrolling gadget.
		 * @return The page size.
		 */
		inline const s16 getPageSize() const { return _pageSize; };

		/**
		 * Set the smallest value that the slider can represent.
		 * @param value The smallest value.
		 */
		inline void setMinimumValue(const s16 value) {
			_minimumValue = value;
			resizeGrip();
		};

		/**
		 * Set the largest value that the slider can represent.
		 * @param value The largest value.
		 */
		inline void setMaximumValue(const s16 value) {
			_maximumValue = value;
			resizeGrip();
		};

		/**
		 * Set the value that of the slider.  This will reposition
		 * and mark the grip for redraw.
		 * @param value The new value.
		 */
		void setValue(const s32 value);

		/**
		 * Set the page size represented by the grip.
		 * @param pageSize The page size.
		 * @see getPageSize().
		 */
		inline void setPageSize(const s16 pageSize) {
			_pageSize = pageSize;
			resizeGrip();
		};

		/**
		 * Process events fired by the grip.
		 * @param e The event details.
		 */
		virtual void handleDragEvent(Gadget& source, const WoopsiPoint& point, const WoopsiPoint& delta);

		/**
		 * Get the smallest value that the slider can move through when
		 * dragged.
		 * @return The smallest value that the slider can move through when
		 * dragged.
		 */
		s16 getMinimumStep() const;

	protected:
		SliderHorizontalGrip* _grip;				/**< Pointer to the grip. */
		s16 _minimumValue;							/**< Minimum value that the grip can represent. */
		s16 _maximumValue;							/**< Maximum value that the grip can represent. */
		s32 _value;									/**< Current value of the slider. */
		s16 _minimumGripWidth;						/**< Smallest width that the grip can become */
		s16 _pageSize;								/**< Value of a page of data, used when clicking the gutter. */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawBorder(GraphicsPort* port);

		/**
		 * Resize the slider to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 */
		virtual void onResize(u16 width, u16 height);
		
		/**
		 * Moves the grip towards the stylus.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);

		/**
		 * Destructor.
		 */
		virtual inline ~SliderHorizontal() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SliderHorizontal(const SliderHorizontal& sliderHorizontal) : Gadget(sliderHorizontal) { };

		/**
		 * Resize and mark the grip for redraw.
		 */
		void resizeGrip();
	};
}

#endif
