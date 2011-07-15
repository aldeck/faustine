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
// name: "pitch-shifter"
//
// Code generated with Faust 0.9.9.6b11doc2 (http://faust.grame.fr)
//-----------------------------------------------------
#ifndef _PITCH_SHIFTER_DSP_H
#define _PITCH_SHIFTER_DSP_H


#include "FaustDSP.h"
#include "FaustUI.h"

#include <math.h>


class PitchShifterDSP : public FaustDSP {
  private:
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT 	fslider1;
	float 	fRec0[2];
	int 	IOTA;
	float 	fVec0[65536];
	FAUSTFLOAT 	fslider2;
  public:
	static void metadata(Meta* m) {
		m->declare("copyright", "(c)GRAME 2006");
		m->declare("license", "BSD");
		m->declare("author", "Grame");
		m->declare("version", "1.0");
		m->declare("name", "pitch-shifter");
	}

	virtual int getNumInputs() 	{ return 1; }
	virtual int getNumOutputs() 	{ return 1; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fslider0 = 0.0f;
		fslider1 = 1000.0f;
		for (int i=0; i<2; i++) fRec0[i] = 0;
		IOTA = 0;
		for (int i=0; i<65536; i++) fVec0[i] = 0;
		fslider2 = 10.0f;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void buildUserInterface(FaustUI* interface) {
		interface->openVerticalBox("Pitch Shifter");
		interface->addHorizontalSlider("shift (semitones)", &fslider0, 0.0f, -12.0f, 12.0f, 0.1f);
		interface->addHorizontalSlider("window (samples)", &fslider1, 1000.0f, 50.0f, 10000.0f, 1.0f);
		interface->addHorizontalSlider("xfade (samples)", &fslider2, 10.0f, 1.0f, 10000.0f, 1.0f);
		interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		float 	fSlow0 = fslider1;
		float 	fSlow1 = ((1 + fSlow0) - powf(2,(8.333333e-02f * fslider0)));
		float 	fSlow2 = (fSlow0 - 1);
		float 	fSlow3 = (1.0f / fslider2);
		FAUSTFLOAT* input0 = input[0];
		FAUSTFLOAT* output0 = output[0];
		for (int i=0; i<count; i++) {
			fRec0[0] = fmodf((fSlow1 + fRec0[1]),fSlow0);
			float fTemp0 = (fSlow0 + fRec0[0]);
			int iTemp1 = int(fTemp0);
			float fTemp2 = (float)input0[i];
			fVec0[IOTA&65535] = fTemp2;
			float fTemp3 = min((fSlow3 * fRec0[0]), 1);
			int iTemp4 = int(fRec0[0]);
			int iTemp5 = (1 + iTemp4);
			output0[i] = (FAUSTFLOAT)((fTemp3 * ((fVec0[(IOTA-int((iTemp4 & 65535)))&65535] * (iTemp5 - fRec0[0])) + ((fRec0[0] - iTemp4) * fVec0[(IOTA-int((int(iTemp5) & 65535)))&65535]))) + ((1 - fTemp3) * ((fVec0[(IOTA-int((int((1 + iTemp1)) & 65535)))&65535] * (fTemp0 - iTemp1)) + (fVec0[(IOTA-int((iTemp1 & 65535)))&65535] * (0 - ((fSlow2 + fRec0[0]) - iTemp1))))));
			// post processing
			IOTA = IOTA+1;
			fRec0[1] = fRec0[0];
		}
	}
};


#endif /* _PITCH_SHIFTER_DSP_H */
