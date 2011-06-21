/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "UIView.h"

#include <Button.h>
#include <Box.h>
#include <GroupLayout.h>
#include <Message.h>
#include <Slider.h>
#include <TabView.h>
#include <Window.h>

#include <stdio.h>


//test
#include "KarplusDSP.h"

#include "PeakView.h"


UIView::UIView(BRect frame)
	:
	BView(frame, "UIView", B_FOLLOW_ALL, B_WILL_DRAW),
	fCurrentLevelLayout(NULL),
	fCurrentTabView(NULL)
{
}


UIView::~UIView()
{
}


void
UIView::AttachedToWindow()
{
	BView::AttachedToWindow();
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fCurrentLevelLayout = new BGroupLayout(B_VERTICAL);
	fCurrentLevelLayout->SetInsets(8, 8, 8, 8);
	SetLayout(fCurrentLevelLayout);
}


void
UIView::Draw(BRect updateRect)
{	
}


void
UIView::FrameResized(float width, float height)
{
}


void
UIView::MessageReceived(BMessage *message)
{
	message->PrintToStream();
	switch (message->what) {
		case 'butt':
		{
			message->PrintToStream();
			float *zone = NULL;
			message->FindPointer("faust:zone_ptr", (void**)&zone);
			if (zone != NULL) {
				if (*zone == 1.0)
					*zone = 0.0;
				else
					*zone = 1.0;
				//snooze(50000);
				//*zone = 0.0;
			}			
			break;
		}
		case 'slid':
		{
			message->PrintToStream();
			float *zone = NULL;
			message->FindPointer("faust:zone_ptr", (void**)&zone);
			int32 value = 0;
			message->FindInt32("be:value", &value);
			if (zone != NULL)
				*zone = (float)value / 1000.0f;
			break;
		}
		default:
			BView::MessageReceived(message);
	}
}

//////////////

void
UIView::addButton(const char* label, float* zone)
{
	if (Window()->Lock()) {
		BMessage* buttonMessage = new BMessage('butt');	
		buttonMessage->AddPointer("faust:zone_ptr", zone);
		BButton* button = new BButton(BRect(0, 1, 80, 1), label, label, buttonMessage);
		button->SetTarget(this);

		fCurrentLevelLayout->AddView(button);
		DoLayout();	
		Window()->Unlock();
	}
}


void
UIView::addCheckButton(const char* label, float* zone)
{
}


void
UIView::addVerticalSlider(const char* label, float* zone,
	float start, float min, float max, float step)
{
	_addSlider(label, zone, start, min, max, step, B_VERTICAL);
}


void
UIView::addHorizontalSlider(const char* label, float* zone,
	float start, float min, float max, float step)
{
	_addSlider(label, zone, start, min, max, step, B_HORIZONTAL);
}


void
UIView::_addSlider(const char* label, float* zone, float start, float min,
	float max, float step, orientation posture)
{
	if (Window()->Lock()) {
		BMessage* sliderMessage = new BMessage('slid');
		sliderMessage->AddPointer("faust:zone_ptr", zone);
		BSlider* slider = new BSlider(BRect(0, 0, 1, 1), label,	label,
			sliderMessage, (int32)(min * 1000.0f), (int32)(max * 1000.0f),
			posture);
		slider->SetKeyIncrementValue((int32)(step * 1000.0f));
			// multiply by 1000 to simulate a float slider
		slider->SetValue((int32)(start * 1000.0f));
		slider->SetTarget(this);

		fCurrentLevelLayout->AddView(slider);
		DoLayout();
		//printf("UIView::_addSlider('%s') view %p layout %p\n", label, fCurrentLevelLayout->View(), fCurrentLevelLayout);		
		Window()->Unlock();
	}
}


void
UIView::addNumEntry(const char* label, float* zone,
	float start, float min, float max, float step)
{
	_addSlider(label, zone, start, min, max, step, B_HORIZONTAL);
}


void
UIView::addHorizontalBargraph(const char* label, float* zone, float min, float max)
{
	if (Window()->Lock()) {
		PeakView* bar = new PeakView(BRect(0, 0, 80, 32), zone, min, max);
		fCurrentLevelLayout->AddView(bar);
		DoLayout();
		Window()->Unlock();
	}
}


void
UIView::addVerticalBargraph(const char* label, float* zone, float min, float max)
{
	if (Window()->Lock()) {
		PeakView* bar = new PeakView(BRect(0, 0, 32, 80), zone, min, max);
		fCurrentLevelLayout->AddView(bar);
		DoLayout();
		Window()->Unlock();
	}
}


void
UIView::openTabBox(const char* label)
{
	
	if (Window()->Lock()) {
		if (fCurrentTabView == NULL) {
			fCurrentTabView = new BTabView(label);			
			//fCurrentTabView->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));
			fCurrentLevelLayout->AddView(fCurrentTabView);						
			//printf("UIView::openTabBox('%s') tabview %p layout %p\n", label, fCurrentTabView, fCurrentLevelLayout);
			DoLayout();
		} else 
			printf("UIView::openTabBox('%s') unimplemented, can't add a tabview in a tabview\n", label);
		
		Window()->Unlock();
	}
}


void
UIView::openHorizontalBox(const char* label)
{
	_openBox(label, B_HORIZONTAL);
}


void
UIView::openVerticalBox(const char* label)
{	
	_openBox(label, B_VERTICAL);
}


void
UIView::_openBox(const char* label, enum orientation orientation)
{
	if (Window()->Lock()) {
		if (fCurrentTabView != NULL) {
			BView* view = new BView(label, B_WILL_DRAW);
			view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
			view->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
			view->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNLIMITED));
			
			fCurrentLevelLayout = new BGroupLayout(orientation);
			fCurrentLevelLayout->SetInsets(8, 8, 8, 8);	// 16 to give space to the box label (unimplemented?)
			view->SetLayout(fCurrentLevelLayout);
			
			fCurrentTabView->AddTab(view);	
			//printf("UIView::_openBox('%s') tabmode tabview %p view %p layout %p\n", label, fCurrentTabView, view, fCurrentLevelLayout);		
			
		} else {
		
			BBox* box = new BBox(label);
			box->SetLabel(label);			
			
			BLayout* upperLevelLayout = fCurrentLevelLayout;
			
			fCurrentLevelLayout = new BGroupLayout(orientation);
			fCurrentLevelLayout->SetInsets(8, 16, 8, 8);	// 16 to give space to the box label (unimplemented?)
			box->SetLayout(fCurrentLevelLayout);
			
			upperLevelLayout->AddView(box);
		
		}
		
		DoLayout();
		Window()->Unlock();
	}
}


void
UIView::closeBox()
{
	if (Window()->Lock()) {
		if (fCurrentTabView != NULL) {			
			if (fCurrentLevelLayout != static_cast<BTwoDimensionalLayout*>(fCurrentTabView->Parent()->GetLayout())) {
				fCurrentLevelLayout = static_cast<BTwoDimensionalLayout*>(fCurrentTabView->Parent()->GetLayout());
				//printf("UIView::closeBox() tabmode fCurrentLevelLayout %p\n", fCurrentLevelLayout);
			} else {
				// on est deja remonte une fois, on ferme le tabview
				fCurrentTabView = NULL;
				//printf("UIView::closeBox() closing tabmode fCurrentLevelLayout %p\n", fCurrentLevelLayout);
			}
			
			//
		} else {
			fCurrentLevelLayout = static_cast<BTwoDimensionalLayout*>(fCurrentLevelLayout->View()->Parent()->GetLayout());	
			//printf("UIView::closeBox() fCurrentLevelLayout %p\n", fCurrentLevelLayout);	
		}
		DoLayout();
		Window()->Unlock();
	}
}


void
UIView::declare(float* zone, const char* s1, const char* s2)
{
	// TODO
}


void
UIView::run()
{
}
