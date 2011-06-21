/*
 * Copyright (C) 1998-1999 Be Incorporated. All rights reseved.
 * Distributed under the terms of the Be Sample Code license.
 *
 * Copyright (C) 2001-2008 Stephan Aßmus. All rights reserved.
 * Distributed under the terms of the MIT license.
 */
#include "PeakView.h"

#include <new>
#include <stdio.h>
#include <string.h>

#include <Bitmap.h>
#include <MenuItem.h>
#include <Message.h>
#include <MessageRunner.h>
#include <Messenger.h>
#include <PopUpMenu.h>
#include <Window.h>


const uint32 kMsgRefresh = 'frsh';

// constructor
PeakView::PeakView(const BRect& frame, float* zone, float min, float max)
	:
	BView(frame, "noname", B_FOLLOW_LEFT | B_FOLLOW_TOP,
		B_WILL_DRAW),
	
	fRefreshPeriod(20000),
	fRefreshRunner(NULL),
	fZone(zone),
	fZoneMin(min),
	fZoneMax(max)
{
	SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetViewColor(B_TRANSPARENT_COLOR);
}


PeakView::~PeakView()
{
	delete fRefreshRunner;
}


void
PeakView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kMsgRefresh:
			_Refresh();
			break;

		default:
			BView::MessageReceived(message);
			break;
	}
}


void
PeakView::AttachedToWindow()
{
	delete fRefreshRunner;
	BMessage message(kMsgRefresh);
	fRefreshRunner = new BMessageRunner(BMessenger(this), &message,
		fRefreshPeriod);

}


void
PeakView::DetachedFromWindow()
{
	delete fRefreshRunner;
	fRefreshRunner = NULL;
}


void
PeakView::Draw(BRect updateRect)
{
	SetHighColor(255, 250, 250);
	FillRect(updateRect);
	BRect rect(Bounds());
	rect.top = rect.Height() * (1.0 - (*fZone - fZoneMin) / (fZoneMax - fZoneMin));
	SetHighColor(0, 0, 0);
	StrokeRect(rect);
}


void
PeakView::FrameResized(float width, float height)
{
}


void
PeakView::_Refresh()
{	
	Invalidate();
}


/*void
PeakView::GetPreferredSize(float* _width, float* _height)
{
	if (_width)
		*_width = Bounds().Width();
	if (_height)
		*_height = Bounds().Height();
}*/
