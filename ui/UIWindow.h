/*
 * Copyright 2009-2010, Alexandre Deckner (alex@zappotek.com)
 * Distributed under the terms of the MIT License.
 *
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
