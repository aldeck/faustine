/*
 * Copyright 2009-2010, Alexandre Deckner (alex@zappotek.com)
 * Distributed under the terms of the MIT License.
 *
 */


#include "UIWindow.h"
#include "UIView.h"

#include <Application.h>
#include <Message.h>

#include <stdio.h>


UIWindow::UIWindow(BRect frame, const char* title)
	:
	BWindow(frame, title, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_AUTO_UPDATE_SIZE_LIMITS)
{
	fUIView = new UIView(Bounds());
	AddChild(fUIView);
	Show();
}


UIWindow::~UIWindow()
{
}


FaustUI*
UIWindow::FaustUserInterface()
{
	return fUIView;
}


bool
UIWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
UIWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		default:
			BWindow::MessageReceived(message);
	}
}
