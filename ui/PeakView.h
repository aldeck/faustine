/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _PEAK_VIEW_H
#define _PEAK_VIEW_H

#include <View.h>

class BMessageRunner;
class BRect;

class PeakView : public BView {
public:
								PeakView(const BRect& frame, float* zone, float min, float max);
	virtual						~PeakView();

	// BView interface
	virtual	void				MessageReceived(BMessage* message);
	virtual	void				AttachedToWindow();
	virtual	void				DetachedFromWindow();
	
	virtual	void				Draw(BRect updateRect);
	virtual	void				FrameResized(float width, float height);
	
	//virtual	void				GetPreferredSize(float* _width,
	//								float* _height);

 private:
 			void				_Refresh();
 
			bigtime_t			fRefreshPeriod;
			BMessageRunner*		fRefreshRunner;

			float*				fZone;
			float				fZoneMin;
			float				fZoneMax;
};

#endif // PEAK_VIEW_H
