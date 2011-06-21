/* 
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
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
#ifndef _MIXER_DSP_H
#define _MIXER_DSP_H

#include "FaustDSP.h"
#include "FaustUI.h"

#include <math.h>

class MixerDSP : public FaustDSP {
  private:
	float 	fConst0;
	FAUSTFLOAT 	fslider0;
	float 	fRec1[2];
	FAUSTFLOAT 	fcheckbox0;
	float 	fRec0[2];
	FAUSTFLOAT 	fbargraph0;
	FAUSTFLOAT 	fentry0;
	FAUSTFLOAT 	fslider1;
	float 	fRec3[2];
	FAUSTFLOAT 	fcheckbox1;
	float 	fRec2[2];
	FAUSTFLOAT 	fbargraph1;
	FAUSTFLOAT 	fentry1;
	FAUSTFLOAT 	fslider2;
	float 	fRec5[2];
	FAUSTFLOAT 	fcheckbox2;
	float 	fRec4[2];
	FAUSTFLOAT 	fbargraph2;
	FAUSTFLOAT 	fentry2;
	FAUSTFLOAT 	fslider3;
	float 	fRec7[2];
	FAUSTFLOAT 	fcheckbox3;
	float 	fRec6[2];
	FAUSTFLOAT 	fbargraph3;
	FAUSTFLOAT 	fentry3;
	FAUSTFLOAT 	fslider4;
	float 	fRec9[2];
	FAUSTFLOAT 	fcheckbox4;
	float 	fRec8[2];
	FAUSTFLOAT 	fbargraph4;
	FAUSTFLOAT 	fentry4;
	FAUSTFLOAT 	fslider5;
	float 	fRec11[2];
	FAUSTFLOAT 	fcheckbox5;
	float 	fRec10[2];
	FAUSTFLOAT 	fbargraph5;
	FAUSTFLOAT 	fentry5;
	FAUSTFLOAT 	fslider6;
	float 	fRec13[2];
	FAUSTFLOAT 	fcheckbox6;
	float 	fRec12[2];
	FAUSTFLOAT 	fbargraph6;
	FAUSTFLOAT 	fentry6;
	FAUSTFLOAT 	fslider7;
	float 	fRec15[2];
	FAUSTFLOAT 	fcheckbox7;
	float 	fRec14[2];
	FAUSTFLOAT 	fbargraph7;
	FAUSTFLOAT 	fentry7;
	FAUSTFLOAT 	fslider8;
	float 	fRec16[2];
  public:
	static void metadata(Meta* m) 	{ 
		m->declare("copyright", "(c) GRAME 2006");
		m->declare("license", "BSD");
		m->declare("author", "Grame");
		m->declare("version", "1.0");
		m->declare("name", "mixer");
		m->declare("volume.dsp/copyright", "(c)GRAME 2006");
		m->declare("volume.dsp/license", "BSD");
		m->declare("volume.dsp/author", "Grame");
		m->declare("volume.dsp/version", "1.0");
		m->declare("volume.dsp/name", "volume");
		m->declare("vumeter.dsp/version", "1.0");
		m->declare("vumeter.dsp/name", "vumeter");
		m->declare("panpot.dsp/copyright", "(c) GRAME 2006");
		m->declare("panpot.dsp/license", "BSD");
		m->declare("panpot.dsp/author", "Grame");
		m->declare("panpot.dsp/version", "1.0");
		m->declare("panpot.dsp/name", "panpot");
		m->declare("vumeter.dsp/copyright", "(c)GRAME 2006");
		m->declare("vumeter.dsp/license", "BSD");
		m->declare("vumeter.dsp/author", "Grame");
	}

	virtual int getNumInputs() 	{ return 8; }
	virtual int getNumOutputs() 	{ return 2; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = (1.0f / fSamplingFreq);
		fslider0 = 0.0f;
		for (int i=0; i<2; i++) fRec1[i] = 0;
		fcheckbox0 = 0.0;
		for (int i=0; i<2; i++) fRec0[i] = 0;
		fentry0 = 0.0f;
		fslider1 = 0.0f;
		for (int i=0; i<2; i++) fRec3[i] = 0;
		fcheckbox1 = 0.0;
		for (int i=0; i<2; i++) fRec2[i] = 0;
		fentry1 = 0.0f;
		fslider2 = 0.0f;
		for (int i=0; i<2; i++) fRec5[i] = 0;
		fcheckbox2 = 0.0;
		for (int i=0; i<2; i++) fRec4[i] = 0;
		fentry2 = 0.0f;
		fslider3 = 0.0f;
		for (int i=0; i<2; i++) fRec7[i] = 0;
		fcheckbox3 = 0.0;
		for (int i=0; i<2; i++) fRec6[i] = 0;
		fentry3 = 0.0f;
		fslider4 = 0.0f;
		for (int i=0; i<2; i++) fRec9[i] = 0;
		fcheckbox4 = 0.0;
		for (int i=0; i<2; i++) fRec8[i] = 0;
		fentry4 = 0.0f;
		fslider5 = 0.0f;
		for (int i=0; i<2; i++) fRec11[i] = 0;
		fcheckbox5 = 0.0;
		for (int i=0; i<2; i++) fRec10[i] = 0;
		fentry5 = 0.0f;
		fslider6 = 0.0f;
		for (int i=0; i<2; i++) fRec13[i] = 0;
		fcheckbox6 = 0.0;
		for (int i=0; i<2; i++) fRec12[i] = 0;
		fentry6 = 0.0f;
		fslider7 = 0.0f;
		for (int i=0; i<2; i++) fRec15[i] = 0;
		fcheckbox7 = 0.0;
		for (int i=0; i<2; i++) fRec14[i] = 0;
		fentry7 = 0.0f;
		fslider8 = 0.0f;
		for (int i=0; i<2; i++) fRec16[i] = 0;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void buildUserInterface(FaustUI* interface) {
		interface->openHorizontalBox("mixer");
		interface->openHorizontalBox("stereo out");
		interface->declare(&fslider8, "1", "");
		interface->addVerticalSlider("", &fslider8, 0.0f, -70.0f, 4.0f, 0.1f);
		interface->closeBox();
		interface->openVerticalBox("voice 0");
		interface->openHorizontalBox("[2]");
		interface->declare(&fslider7, "1", "");
		interface->addVerticalSlider("", &fslider7, 0.0f, -70.0f, 4.0f, 0.1f);
		interface->declare(&fbargraph7, "2", "");
		interface->declare(&fbargraph7, "unit", "dB");
		interface->addVerticalBargraph("", &fbargraph7, -70.0f, 5.0f);
		interface->closeBox();
		interface->addCheckButton("mute", &fcheckbox7);
		interface->openVerticalBox("panpot");
		interface->declare(&fentry7, "style", "knob");
		interface->addNumEntry("", &fentry7, 0.0f, -8.0f, 8.0f, 1.0f);
		interface->closeBox();
		interface->closeBox();
		interface->openVerticalBox("voice 1");
		interface->openHorizontalBox("[2]");
		interface->declare(&fslider6, "1", "");
		interface->addVerticalSlider("", &fslider6, 0.0f, -70.0f, 4.0f, 0.1f);
		interface->declare(&fbargraph6, "2", "");
		interface->declare(&fbargraph6, "unit", "dB");
		interface->addVerticalBargraph("", &fbargraph6, -70.0f, 5.0f);
		interface->closeBox();
		interface->addCheckButton("mute", &fcheckbox6);
		interface->openVerticalBox("panpot");
		interface->declare(&fentry6, "style", "knob");
		interface->addNumEntry("", &fentry6, 0.0f, -8.0f, 8.0f, 1.0f);
		interface->closeBox();
		interface->closeBox();
		interface->openVerticalBox("voice 2");
		interface->openHorizontalBox("[2]");
		interface->declare(&fslider5, "1", "");
		interface->addVerticalSlider("", &fslider5, 0.0f, -70.0f, 4.0f, 0.1f);
		interface->declare(&fbargraph5, "2", "");
		interface->declare(&fbargraph5, "unit", "dB");
		interface->addVerticalBargraph("", &fbargraph5, -70.0f, 5.0f);
		interface->closeBox();
		interface->addCheckButton("mute", &fcheckbox5);
		interface->openVerticalBox("panpot");
		interface->declare(&fentry5, "style", "knob");
		interface->addNumEntry("", &fentry5, 0.0f, -8.0f, 8.0f, 1.0f);
		interface->closeBox();
		interface->closeBox();
		interface->openVerticalBox("voice 3");
		interface->openHorizontalBox("[2]");
		interface->declare(&fslider4, "1", "");
		interface->addVerticalSlider("", &fslider4, 0.0f, -70.0f, 4.0f, 0.1f);
		interface->declare(&fbargraph4, "2", "");
		interface->declare(&fbargraph4, "unit", "dB");
		interface->addVerticalBargraph("", &fbargraph4, -70.0f, 5.0f);
		interface->closeBox();
		interface->addCheckButton("mute", &fcheckbox4);
		interface->openVerticalBox("panpot");
		interface->declare(&fentry4, "style", "knob");
		interface->addNumEntry("", &fentry4, 0.0f, -8.0f, 8.0f, 1.0f);
		interface->closeBox();
		interface->closeBox();
		interface->openVerticalBox("voice 4");
		interface->openHorizontalBox("[2]");
		interface->declare(&fslider3, "1", "");
		interface->addVerticalSlider("", &fslider3, 0.0f, -70.0f, 4.0f, 0.1f);
		interface->declare(&fbargraph3, "2", "");
		interface->declare(&fbargraph3, "unit", "dB");
		interface->addVerticalBargraph("", &fbargraph3, -70.0f, 5.0f);
		interface->closeBox();
		interface->addCheckButton("mute", &fcheckbox3);
		interface->openVerticalBox("panpot");
		interface->declare(&fentry3, "style", "knob");
		interface->addNumEntry("", &fentry3, 0.0f, -8.0f, 8.0f, 1.0f);
		interface->closeBox();
		interface->closeBox();
		interface->openVerticalBox("voice 5");
		interface->openHorizontalBox("[2]");
		interface->declare(&fslider2, "1", "");
		interface->addVerticalSlider("", &fslider2, 0.0f, -70.0f, 4.0f, 0.1f);
		interface->declare(&fbargraph2, "2", "");
		interface->declare(&fbargraph2, "unit", "dB");
		interface->addVerticalBargraph("", &fbargraph2, -70.0f, 5.0f);
		interface->closeBox();
		interface->addCheckButton("mute", &fcheckbox2);
		interface->openVerticalBox("panpot");
		interface->declare(&fentry2, "style", "knob");
		interface->addNumEntry("", &fentry2, 0.0f, -8.0f, 8.0f, 1.0f);
		interface->closeBox();
		interface->closeBox();
		interface->openVerticalBox("voice 6");
		interface->openHorizontalBox("[2]");
		interface->declare(&fslider1, "1", "");
		interface->addVerticalSlider("", &fslider1, 0.0f, -70.0f, 4.0f, 0.1f);
		interface->declare(&fbargraph1, "2", "");
		interface->declare(&fbargraph1, "unit", "dB");
		interface->addVerticalBargraph("", &fbargraph1, -70.0f, 5.0f);
		interface->closeBox();
		interface->addCheckButton("mute", &fcheckbox1);
		interface->openVerticalBox("panpot");
		interface->declare(&fentry1, "style", "knob");
		interface->addNumEntry("", &fentry1, 0.0f, -8.0f, 8.0f, 1.0f);
		interface->closeBox();
		interface->closeBox();
		interface->openVerticalBox("voice 7");
		interface->openHorizontalBox("[2]");
		interface->declare(&fslider0, "1", "");
		interface->addVerticalSlider("", &fslider0, 0.0f, -70.0f, 4.0f, 0.1f);
		interface->declare(&fbargraph0, "2", "");
		interface->declare(&fbargraph0, "unit", "dB");
		interface->addVerticalBargraph("", &fbargraph0, -70.0f, 5.0f);
		interface->closeBox();
		interface->addCheckButton("mute", &fcheckbox0);
		interface->openVerticalBox("panpot");
		interface->declare(&fentry0, "style", "knob");
		interface->addNumEntry("", &fentry0, 0.0f, -8.0f, 8.0f, 1.0f);
		interface->closeBox();
		interface->closeBox();
		interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		float 	fSlow0 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider0)));
		float 	fSlow1 = (1 - fcheckbox0);
		float 	fSlow2 = (6.250000e-02f * (fentry0 - 8));
		float 	fSlow3 = (1 + fSlow2);
		float 	fSlow4 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider1)));
		float 	fSlow5 = (1 - fcheckbox1);
		float 	fSlow6 = (6.250000e-02f * (fentry1 - 8));
		float 	fSlow7 = (1 + fSlow6);
		float 	fSlow8 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider2)));
		float 	fSlow9 = (1 - fcheckbox2);
		float 	fSlow10 = (6.250000e-02f * (fentry2 - 8));
		float 	fSlow11 = (1 + fSlow10);
		float 	fSlow12 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider3)));
		float 	fSlow13 = (1 - fcheckbox3);
		float 	fSlow14 = (6.250000e-02f * (fentry3 - 8));
		float 	fSlow15 = (1 + fSlow14);
		float 	fSlow16 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider4)));
		float 	fSlow17 = (1 - fcheckbox4);
		float 	fSlow18 = (6.250000e-02f * (fentry4 - 8));
		float 	fSlow19 = (1 + fSlow18);
		float 	fSlow20 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider5)));
		float 	fSlow21 = (1 - fcheckbox5);
		float 	fSlow22 = (6.250000e-02f * (fentry5 - 8));
		float 	fSlow23 = (1 + fSlow22);
		float 	fSlow24 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider6)));
		float 	fSlow25 = (1 - fcheckbox6);
		float 	fSlow26 = (6.250000e-02f * (fentry6 - 8));
		float 	fSlow27 = (1 + fSlow26);
		float 	fSlow28 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider7)));
		float 	fSlow29 = (1 - fcheckbox7);
		float 	fSlow30 = (6.250000e-02f * (fentry7 - 8));
		float 	fSlow31 = (1 + fSlow30);
		float 	fSlow32 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider8)));
		float 	fSlow33 = (0 - fSlow26);
		float 	fSlow34 = (0 - fSlow30);
		float 	fSlow35 = (0 - fSlow2);
		float 	fSlow36 = (0 - fSlow6);
		float 	fSlow37 = (0 - fSlow22);
		float 	fSlow38 = (0 - fSlow18);
		float 	fSlow39 = (0 - fSlow14);
		float 	fSlow40 = (0 - fSlow10);
		FAUSTFLOAT* input0 = input[0];
		FAUSTFLOAT* input1 = input[1];
		FAUSTFLOAT* input2 = input[2];
		FAUSTFLOAT* input3 = input[3];
		FAUSTFLOAT* input4 = input[4];
		FAUSTFLOAT* input5 = input[5];
		FAUSTFLOAT* input6 = input[6];
		FAUSTFLOAT* input7 = input[7];
		FAUSTFLOAT* output0 = output[0];
		FAUSTFLOAT* output1 = output[1];
		for (int i=0; i<count; i++) {
			fRec1[0] = ((0.999f * fRec1[1]) + fSlow0);
			float fTemp0 = (fSlow1 * ((float)input7[i] * fRec1[0]));
			fRec0[0] = max((fRec0[1] - fConst0), fabsf(fTemp0));
			fbargraph0 = (20 * log10f(max(3.162278e-04f, fRec0[0])));
			float fTemp1 = fTemp0;
			fRec3[0] = ((0.999f * fRec3[1]) + fSlow4);
			float fTemp2 = (fSlow5 * ((float)input6[i] * fRec3[0]));
			fRec2[0] = max((fRec2[1] - fConst0), fabsf(fTemp2));
			fbargraph1 = (20 * log10f(max(3.162278e-04f, fRec2[0])));
			float fTemp3 = fTemp2;
			fRec5[0] = ((0.999f * fRec5[1]) + fSlow8);
			float fTemp4 = (fSlow9 * ((float)input5[i] * fRec5[0]));
			fRec4[0] = max((fRec4[1] - fConst0), fabsf(fTemp4));
			fbargraph2 = (20 * log10f(max(3.162278e-04f, fRec4[0])));
			float fTemp5 = fTemp4;
			fRec7[0] = ((0.999f * fRec7[1]) + fSlow12);
			float fTemp6 = (fSlow13 * ((float)input4[i] * fRec7[0]));
			fRec6[0] = max((fRec6[1] - fConst0), fabsf(fTemp6));
			fbargraph3 = (20 * log10f(max(3.162278e-04f, fRec6[0])));
			float fTemp7 = fTemp6;
			fRec9[0] = ((0.999f * fRec9[1]) + fSlow16);
			float fTemp8 = (fSlow17 * ((float)input3[i] * fRec9[0]));
			fRec8[0] = max((fRec8[1] - fConst0), fabsf(fTemp8));
			fbargraph4 = (20 * log10f(max(3.162278e-04f, fRec8[0])));
			float fTemp9 = fTemp8;
			fRec11[0] = ((0.999f * fRec11[1]) + fSlow20);
			float fTemp10 = (fSlow21 * ((float)input2[i] * fRec11[0]));
			fRec10[0] = max((fRec10[1] - fConst0), fabsf(fTemp10));
			fbargraph5 = (20 * log10f(max(3.162278e-04f, fRec10[0])));
			float fTemp11 = fTemp10;
			fRec13[0] = ((0.999f * fRec13[1]) + fSlow24);
			float fTemp12 = (fSlow25 * ((float)input1[i] * fRec13[0]));
			fRec12[0] = max((fRec12[1] - fConst0), fabsf(fTemp12));
			fbargraph6 = (20 * log10f(max(3.162278e-04f, fRec12[0])));
			float fTemp13 = fTemp12;
			fRec15[0] = ((0.999f * fRec15[1]) + fSlow28);
			float fTemp14 = (fSlow29 * ((float)input0[i] * fRec15[0]));
			fRec14[0] = max((fRec14[1] - fConst0), fabsf(fTemp14));
			fbargraph7 = (20 * log10f(max(3.162278e-04f, fRec14[0])));
			float fTemp15 = fTemp14;
			fRec16[0] = ((0.999f * fRec16[1]) + fSlow32);
			output0[i] = (FAUSTFLOAT)(fRec16[0] * ((((((((fSlow31 * fTemp15) + (fSlow27 * fTemp13)) + (fSlow23 * fTemp11)) + (fSlow19 * fTemp9)) + (fSlow15 * fTemp7)) + (fSlow11 * fTemp5)) + (fSlow7 * fTemp3)) + (fSlow3 * fTemp1)));
			output1[i] = (FAUSTFLOAT)(fRec16[0] * ((((fSlow40 * fTemp5) + ((fSlow39 * fTemp7) + ((fSlow38 * fTemp9) + ((fSlow37 * fTemp11) + ((fSlow36 * fTemp3) + (fSlow35 * fTemp1)))))) + (fSlow34 * fTemp15)) + (fSlow33 * fTemp13)));
			// post processing
			fRec16[1] = fRec16[0];
			fRec14[1] = fRec14[0];
			fRec15[1] = fRec15[0];
			fRec12[1] = fRec12[0];
			fRec13[1] = fRec13[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec2[1] = fRec2[0];
			fRec3[1] = fRec3[0];
			fRec0[1] = fRec0[0];
			fRec1[1] = fRec1[0];
		}
	}
};


#endif	//_MIXER_DSP_H
