/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include <Alert.h>
#include <MediaRoster.h>
#include <TimeSource.h>

#include "App.h"
#include "audio/GenericAudioFilterNode.h"
#include "audio/KarplusDSP.h"
#include "audio/FreeverbDSP.h"
#include "audio/PitchShifterDSP.h"
#include "audio/DbMeterDSP.h"
#include "audio/CaptureDSP.h"
#include "audio/MixerDSP.h"
#include "audio/TapiirDSP.h"

#include "UIWindow.h"

#include <stdio.h>
#include <string.h>


struct media_connection {
	media_output output;
	media_input input;
};


App::App()
	:
	BApplication("application/x-vnd.Haiku-Faustine"),
	/*fMainWindow(NULL),*/
	fMainMediaNode(NULL),
	fMediaRoster(NULL),
	fMixerConnection(NULL),
	fPlayerConnection(NULL)
{
}


App::~App()
{
	status_t err = fMediaRoster->Disconnect(fPlayerConnection->output, fPlayerConnection->input);

    if (err == B_OK) {
    	printf("Succesfully disconnected output '%s' (%ld,%ld) to input '%s' (%ld,%ld)\n",
    		fPlayerConnection->output.name, fPlayerConnection->output.node.node,
    		fPlayerConnection->output.source.id,
    		fPlayerConnection->input.name,fPlayerConnection->input.node.node,
    		fPlayerConnection->input.destination.id);
    } else {
		printf("Succesfully disconnected output '%s' (%ld,%ld) to input '%s' (%ld,%ld)\n",
    		fPlayerConnection->output.name, fPlayerConnection->output.node.node, fPlayerConnection->output.source.id,
    		fPlayerConnection->input.name, fPlayerConnection->input.node.node, fPlayerConnection->input.destination.id);
    }

    fMediaRoster->UnregisterNode(fMainMediaNode);
}


void
App::ReadyToRun()
{
	// faust ui
	UIWindow* uiWindow = new UIWindow(BRect(512, 512, 512 + 320, 512 + 240),
		"UIWindow");
	uiWindow->Show();

	// audio

	//fFaustDSP = new KarplusDSP();
	//fFaustDSP = new FreeverbDSP();
	fFaustDSP = new PitchShifterDSP();
	//fFaustDSP = new DbMeterDSP();
	//fFaustDSP = new MixerDSP();

	fFaustDSP->buildUserInterface(uiWindow->FaustUserInterface());

	// registering our media node
	status_t err;
	fMediaRoster = BMediaRoster::Roster(&err);
	if (fMediaRoster == NULL || err != B_OK) {
		/* the Media Server appears to be dead -- handle that here */
		printf("Oh! The Media Server appears to be dead :(\n");
	} else {
		fMainMediaNode = new GenericAudioFilterNode(fFaustDSP);
		err = fMediaRoster->RegisterNode(fMainMediaNode);
		if (err != B_OK) {
			printf("Error registering the audio node\n");
			return;
		} else
			printf("Registered the audio node successfully\n");
	}

	// connecting main output to the mixer
	media_node	mixerNode;
	err = fMediaRoster->GetAudioMixer(&mixerNode);
	if (err == B_OK) {
		_ConnectAudioNodes(fMainMediaNode->Node(), mixerNode);
	} else {
		printf("Error! Couldn't GetAudioMixer\n");
	}


	// connecting to mediaplayer
	int32 nodeInfoCount = 512;
	live_node_info* nodeInfos = new live_node_info[nodeInfoCount];
	err = fMediaRoster->GetLiveNodes(nodeInfos, &nodeInfoCount);//,
                      //const media_format* hasInput = NULL,
                     // const media_format* hasOutput = NULL,
                     // const char* name = NULL,
                     //uint64 nodeKinds = 0);

    media_node mediaPlayerNode;
    if (err == B_OK) {
    	for (int i = 0; i < nodeInfoCount; i++) {
    		printf("found node '%s' id=%ld\n", nodeInfos[i].name, nodeInfos[i].node.node);
    		if (strcmp(nodeInfos[i].name, "MediaPlayer audio out") == 0) {
    			printf("found 'MediaPlayer Audio Out' try connecting\n");
    			mediaPlayerNode = nodeInfos[i].node;
    			err = _ConnectAudioNodes(mediaPlayerNode, fMainMediaNode->Node());
    			if (err == B_OK)
    				break;
    			// todo stop the loop if succeed
    		}
    	}
    } else {
    	printf("Error! Couldn't get live nodes\n");
    }

}


status_t
App::_ConnectAudioNodes(const media_node& outputNode, const media_node& inputNode)
{

	//fFlags |= F_MUST_RELEASE_MIXER;


	// set the producer's time source to be the "default" time source,
	// which the system audio mixer uses too.
	media_node timeSource;
	status_t err = B_OK;

	err = fMediaRoster->GetTimeSource(&timeSource);
	if (err != B_OK) {
		printf("Couldn't GetTimeSource\n");
		return err;
	}
	err = fMediaRoster->SetTimeSourceFor(outputNode.node, timeSource.node);
	if (err != B_OK) {
		printf("Couldn't SetTimeSourceFor\n");
		return err;
	}
	printf("output node %ld has timesource %ld\n", outputNode.node, timeSource.node/*fMainMediaNode->TimeSource()->Node().node*/);

	// find a free input in the mixer
	media_input	mixerInput;
	int32 mixerInputCount;
	err = fMediaRoster->GetFreeInputsFor(inputNode, &mixerInput, 1, &mixerInputCount, B_MEDIA_RAW_AUDIO);
	if (err != B_OK) {
		printf("Couldn't GetFreeInputsFor\n");
		return err;
	}
	if (mixerInputCount < 1) {
		printf("Couldn't find a free input\n");
		return B_ERROR;
	}

	// find a free output from our node
	media_output mainOutput;
	int32 mainOutputCount;
	err = fMediaRoster->GetFreeOutputsFor(outputNode, &mainOutput, 1, &mainOutputCount, B_MEDIA_RAW_AUDIO);
	if (err != B_OK) {
		printf("Couldn't GetFreeOutputsFor\n");
		return err;
	}
	if (mainOutputCount < 1) {
		printf("Couldn't find a free output\n");
		return B_ERROR;
	}

	// Set an appropriate run mode for the producer
	err = fMediaRoster->SetRunModeNode(outputNode, BMediaNode::B_INCREASE_LATENCY);
	if (err != B_OK) {
		printf("Couldn't SetRunModeNode\n");
		return err;
	}

	// setup our requested format (can still have many wildcards)
	media_format format;
	format.type = B_MEDIA_RAW_AUDIO;
	//format.u.raw_audio = *format;

	char buf[512];
	string_for_format(format, buf, sizeof(buf));
	printf("Trying to connect with format %s\n", buf);

	// and connect the nodes
	err = fMediaRoster->Connect(mainOutput.source, mixerInput.destination, &format, &mainOutput, &mixerInput);
	if (err != B_OK) {
		printf("Couldn't Connect\n");
		return err;
	}

	//move that out
	printf("Succesfully connected outputNode %ld to inputNode %ld\n", outputNode.node, inputNode.node);
	fPlayerConnection = new media_connection();// leaky test, todo list of connections
	fPlayerConnection->output = mainOutput;
	fPlayerConnection->input = mixerInput;

	//todo rename mainOutput en output ou outNodeOutput
	//				mixerInput input ou inNodeInput

	//fFlags |= F_NODES_CONNECTED;

	//get_volume_slider();


	//TRACE("BSoundPlayer::Init: %s\n", strerror(err));
	return err;
}


void
App::AboutRequested()
{
	BAlert* alert;
	alert = new BAlert("About", "A little audio demo using Faust", "ok");
	alert->Go(NULL);
}


bool
App::QuitRequested()
{
	return true;
}


int
main(int argc, char** argv)
{
	App app;
	app.Run();
	return 0;
}
