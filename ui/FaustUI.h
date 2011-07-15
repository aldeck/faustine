/*
 * Copyright 2009-2010, Alexandre Deckner (alex@zappotek.com)
 * Distributed under the terms of the MIT License.
 *
 */
#ifndef _FAUST_UI_H
#define _FAUST_UI_H


class FaustUI {
public:
	virtual void addButton(const char*, float*) = 0;
	virtual void addCheckButton(const char*, float*) = 0;
	virtual void addVerticalSlider(const char*, float*, float, float, float, float) = 0;
	virtual void addHorizontalSlider(const char* label, float* zone, float start, float min, float max, float step) = 0;
	virtual void addNumEntry(const char*, float*, float start, float min, float max, float step) = 0;

	virtual void addHorizontalBargraph(const char*, float*, float, float) = 0;
	virtual void addVerticalBargraph(const char*, float*, float, float) = 0;

	virtual void openTabBox(const char*) = 0;
	virtual void openHorizontalBox(const char*) = 0;
	virtual void openVerticalBox(const char*) = 0;
	virtual void closeBox() = 0;

	virtual void declare(float*, const char*, const char*) = 0;

	virtual void run() = 0;
};


#endif /* _FAUST_UI_H */
