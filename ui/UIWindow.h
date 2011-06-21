/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _UI_WINDOW_H
#define _UI_WINDOW_H

#include <Window.h>

#include "FaustUI.h"

class UIView;

class UIWindow: public BWindow {
public:
					UIWindow(BRect frame, const char* title);
					~UIWindow();					

	virtual void	MessageReceived(BMessage *message);
	virtual bool	QuitRequested();
	
	FaustUI*		FaustUserInterface();

protected:
	UIView*		fUIView;
};

#endif /* _UI_WINDOW_H */
