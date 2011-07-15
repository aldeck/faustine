/*
 * Copyright 2009-2010, Alexandre Deckner (alex@zappotek.com)
 * Distributed under the terms of the MIT License.
 *
 */
#ifndef _APP_H
#define _APP_H


#include <Application.h>


class BMediaNode;
class BMediaRoster;
class media_node;
struct media_connection;
class FaustDSP;


class App: public BApplication {
public:
					App();
	virtual			~App();

	virtual	void	ReadyToRun();
	virtual void	AboutRequested();
	virtual	bool	QuitRequested();

protected:
	status_t		_ConnectAudioNodes(const media_node& outputNode,
						const media_node& inputNode);

	//BWindow*		fMainWindow;
	BMediaNode*		fMainMediaNode;
	BMediaRoster*	fMediaRoster;

	media_connection* fMixerConnection;
	media_connection* fPlayerConnection;

	FaustDSP*		fFaustDSP;
};


#endif /* _APP_H */
