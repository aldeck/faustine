/*
 * Copyright 2009-2010, Alexandre Deckner (alex@zappotek.com)
 * Distributed under the terms of the MIT License.
 *
 */
#ifndef _UI_VIEW_H
#define _UI_VIEW_H


#include "FaustUI.h"

#include <View.h>


class BTwoDimensionalLayout;
class BTabView;


class UIView : public BView, public FaustUI {
public:
					UIView(BRect frame);
	virtual			~UIView();

	virtual void	Draw(BRect updateRect);
	virtual void	AttachedToWindow();
	virtual void	FrameResized(float width, float height);

	virtual void	MessageReceived(BMessage *message);

	// FaustUI

	virtual	void	addButton(const char* label, float* zone);
	virtual	void	addCheckButton(const char* label, float* zone);

	virtual	void	addVerticalSlider(const char* label, float* zone,
						float start, float min, float max, float step);

	virtual	void	addHorizontalSlider(const char* label, float* zone,
						float start, float min, float max, float step);

	virtual	void	addNumEntry(const char* label, float* zone,
						float start, float min, float max, float step);

	virtual	void	addHorizontalBargraph(const char*, float*, float, float);
	virtual	void	addVerticalBargraph(const char*, float*, float, float);

	virtual	void	openTabBox(const char*);
	virtual	void	openHorizontalBox(const char*);
	virtual	void	openVerticalBox(const char*);
	virtual	void	closeBox();

	virtual void	declare(float*, const char*, const char*);

	virtual void	run();

protected:
	void			_addSlider(const char* label, float* zone, float start,
						float min, float max, float step, orientation posture);
	void			_openBox(const char* label, enum orientation orientation);

	BTwoDimensionalLayout*			fCurrentLevelLayout;
	BTabView*						fCurrentTabView;
};


#endif /* _UI_VIEW_H */
