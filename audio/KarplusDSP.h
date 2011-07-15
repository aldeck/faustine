/*
 * Copyright 2009-2010, Alexandre Deckner (alex@zappotek.com)
 * Distributed under the terms of the MIT License.
 *
 */

//-----------------------------------------------------
// copyright: "(c)GRAME 2006"
// license: "BSD"
// author: "Grame"
// version: "1.0"
// name: "karplus"
//
// Code generated with Faust 0.9.9.6b11doc2 (http://faust.grame.fr)
//-----------------------------------------------------
#ifndef _KARPLUS_DSP_H
#define _KARPLUS_DSP_H


#include "FaustDSP.h"
#include "FaustUI.h"

#include <math.h>


class KarplusDSP : public FaustDSP {
//public:
//	static FAUSTFLOAT 	fbutton0;
//	static FAUSTFLOAT 	fslider3;
  private:
	int 	iRec1[2];
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT 	fbutton0;
	float 	fVec0[2];
	float 	fRec2[2];
	FAUSTFLOAT 	fslider1;
	FAUSTFLOAT 	fslider2;
	int 	IOTA;
	float 	fVec1[512];
	FAUSTFLOAT 	fslider3;
	float 	fRec0[3];
  public:
	/*static void metadata(Meta* m) 	{
		m->declare("copyright", "(c)GRAME 2006");
		m->declare("license", "BSD");
		m->declare("author", "Grame");
		m->declare("version", "1.0");
		m->declare("name", "karplus");
	}*/

	virtual int getNumInputs() 	{ return 0; }
	virtual int getNumOutputs() 	{ return 1; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		for (int i=0; i<2; i++) iRec1[i] = 0;
		fslider0 = 128.0f;
		fbutton0 = 0.0;
		for (int i=0; i<2; i++) fVec0[i] = 0;
		for (int i=0; i<2; i++) fRec2[i] = 0;
		fslider1 = 0.5f;
		fslider2 = 0.1f;
		IOTA = 0;
		for (int i=0; i<512; i++) fVec1[i] = 0;
		fslider3 = 128.0f;
		for (int i=0; i<3; i++) fRec0[i] = 0;
	}


	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}


	virtual void buildUserInterface(FaustUI* interface) {
		interface->openVerticalBox("karplus");
		interface->openVerticalBox("excitator");
		//interface->declare(&fslider0, "unit", "f");
		interface->addHorizontalSlider("excitation", &fslider0, 128.0f, 2.0f, 512.0f, 1.0f);
		interface->addButton("play", &fbutton0);
		interface->closeBox();
		interface->addHorizontalSlider("level", &fslider1, 0.5f, 0.0f, 1.0f, 0.1f);
		interface->openVerticalBox("resonator");
		interface->addHorizontalSlider("attenuation", &fslider2, 0.1f, 0.0f, 1.0f, 1.000000e-02f);
		//interface->declare(&fslider3, "unit", "f");
		interface->addHorizontalSlider("duration", &fslider3, 128.0f, 2.0f, 512.0f, 1.0f);
		interface->closeBox();
		interface->closeBox();
	}


	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output){
		float 	fSlow0 = (1.0f / fslider0);
		float 	fSlow1 = fbutton0;
		float 	fSlow2 = (4.656613e-10f * fslider1);
		float 	fSlow3 = (0.5f * (1.0f - fslider2));
		int 	iSlow4 = int((int((fslider3 - 1.5f)) & 4095));
		FAUSTFLOAT* output0 = output[0];
		for (int i=0; i<count; i++) {
			iRec1[0] = (12345 + (1103515245 * iRec1[1]));
			fVec0[0] = fSlow1;
			fRec2[0] = ((fRec2[1] + ((fSlow1 - fVec0[1]) > 0.0f)) - (fSlow0 * (fRec2[1] > 0.0f)));
			fVec1[IOTA&511] = ((fSlow3 * (fRec0[2] + fRec0[1])) + (fSlow2 * ((fRec2[0] > 0.0f) * iRec1[0])));
			fRec0[0] = fVec1[(IOTA-iSlow4)&511];
			output0[i] = (FAUSTFLOAT)fRec0[0];
			// post processing
			fRec0[2] = fRec0[1]; fRec0[1] = fRec0[0];
			IOTA = IOTA+1;
			fRec2[1] = fRec2[0];
			fVec0[1] = fVec0[0];
			iRec1[1] = iRec1[0];
		}
	}

};


#endif /* _KARPLUS_DSP_H */
