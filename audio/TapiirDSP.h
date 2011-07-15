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
#ifndef _TAPIIR_DSP_H
#define _TAPIIR_DSP_H


#include "FaustDSP.h"
#include "FaustUI.h"

#include <math.h>


class TapiirDSP : public FaustDSP {
  private:
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT 	fslider1;
	FAUSTFLOAT 	fslider2;
	FAUSTFLOAT 	fslider3;
	FAUSTFLOAT 	fslider4;
	FAUSTFLOAT 	fslider5;
	FAUSTFLOAT 	fslider6;
	FAUSTFLOAT 	fslider7;
	FAUSTFLOAT 	fslider8;
	int 	IOTA;
	float 	fVec0[524288];
	FAUSTFLOAT 	fslider9;
	float 	fRec0[2];
	FAUSTFLOAT 	fslider10;
	FAUSTFLOAT 	fslider11;
	FAUSTFLOAT 	fslider12;
	FAUSTFLOAT 	fslider13;
	FAUSTFLOAT 	fslider14;
	FAUSTFLOAT 	fslider15;
	FAUSTFLOAT 	fslider16;
	FAUSTFLOAT 	fslider17;
	FAUSTFLOAT 	fslider18;
	float 	fVec1[524288];
	FAUSTFLOAT 	fslider19;
	float 	fRec1[2];
	FAUSTFLOAT 	fslider20;
	FAUSTFLOAT 	fslider21;
	FAUSTFLOAT 	fslider22;
	FAUSTFLOAT 	fslider23;
	FAUSTFLOAT 	fslider24;
	FAUSTFLOAT 	fslider25;
	FAUSTFLOAT 	fslider26;
	FAUSTFLOAT 	fslider27;
	FAUSTFLOAT 	fslider28;
	float 	fVec2[524288];
	FAUSTFLOAT 	fslider29;
	float 	fRec2[2];
	FAUSTFLOAT 	fslider30;
	FAUSTFLOAT 	fslider31;
	FAUSTFLOAT 	fslider32;
	FAUSTFLOAT 	fslider33;
	FAUSTFLOAT 	fslider34;
	FAUSTFLOAT 	fslider35;
	FAUSTFLOAT 	fslider36;
	FAUSTFLOAT 	fslider37;
	FAUSTFLOAT 	fslider38;
	float 	fVec3[524288];
	FAUSTFLOAT 	fslider39;
	float 	fRec3[2];
	FAUSTFLOAT 	fslider40;
	FAUSTFLOAT 	fslider41;
	FAUSTFLOAT 	fslider42;
	FAUSTFLOAT 	fslider43;
	FAUSTFLOAT 	fslider44;
	FAUSTFLOAT 	fslider45;
	FAUSTFLOAT 	fslider46;
	FAUSTFLOAT 	fslider47;
	FAUSTFLOAT 	fslider48;
	float 	fVec4[524288];
	FAUSTFLOAT 	fslider49;
	float 	fRec4[2];
	FAUSTFLOAT 	fslider50;
	FAUSTFLOAT 	fslider51;
	FAUSTFLOAT 	fslider52;
	FAUSTFLOAT 	fslider53;
	FAUSTFLOAT 	fslider54;
	FAUSTFLOAT 	fslider55;
	FAUSTFLOAT 	fslider56;
	FAUSTFLOAT 	fslider57;
	FAUSTFLOAT 	fslider58;
	float 	fVec5[524288];
	FAUSTFLOAT 	fslider59;
	float 	fRec5[2];
	FAUSTFLOAT 	fslider60;
	FAUSTFLOAT 	fslider61;
	FAUSTFLOAT 	fslider62;
	FAUSTFLOAT 	fslider63;
	FAUSTFLOAT 	fslider64;
	FAUSTFLOAT 	fslider65;
	FAUSTFLOAT 	fslider66;
	FAUSTFLOAT 	fslider67;
	FAUSTFLOAT 	fslider68;
	FAUSTFLOAT 	fslider69;
	FAUSTFLOAT 	fslider70;
	FAUSTFLOAT 	fslider71;
	FAUSTFLOAT 	fslider72;
	FAUSTFLOAT 	fslider73;
	FAUSTFLOAT 	fslider74;
	FAUSTFLOAT 	fslider75;
	FAUSTFLOAT 	fslider76;
	FAUSTFLOAT 	fslider77;
  public:
	/*static void metadata(Meta* m) 	{
		m->declare("copyright", "(c) GRAME 2006");
		m->declare("license", "BSD");
		m->declare("author", "Grame");
		m->declare("version", "1.0");
		m->declare("name", "tapiir");
	}*/

	virtual int getNumInputs() 	{ return 2; }
	virtual int getNumOutputs() 	{ return 2; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fslider0 = 1.0f;
		fslider1 = 1.0f;
		fslider2 = 0.0f;
		fslider3 = 0.0f;
		fslider4 = 0.0f;
		fslider5 = 0.0f;
		fslider6 = 0.0f;
		fslider7 = 0.0f;
		fslider8 = 1.0f;
		IOTA = 0;
		for (int i=0; i<524288; i++) fVec0[i] = 0;
		fslider9 = 0.0f;
		for (int i=0; i<2; i++) fRec0[i] = 0;
		fslider10 = 0.0f;
		fslider11 = 0.0f;
		fslider12 = 0.0f;
		fslider13 = 0.0f;
		fslider14 = 0.0f;
		fslider15 = 1.0f;
		fslider16 = 0.0f;
		fslider17 = 1.0f;
		fslider18 = 1.0f;
		for (int i=0; i<524288; i++) fVec1[i] = 0;
		fslider19 = 0.0f;
		for (int i=0; i<2; i++) fRec1[i] = 0;
		fslider20 = 1.0f;
		fslider21 = 1.0f;
		fslider22 = 0.0f;
		fslider23 = 0.0f;
		fslider24 = 0.0f;
		fslider25 = 0.0f;
		fslider26 = 0.0f;
		fslider27 = 0.0f;
		fslider28 = 1.0f;
		for (int i=0; i<524288; i++) fVec2[i] = 0;
		fslider29 = 0.0f;
		for (int i=0; i<2; i++) fRec2[i] = 0;
		fslider30 = 1.0f;
		fslider31 = 1.0f;
		fslider32 = 0.0f;
		fslider33 = 0.0f;
		fslider34 = 0.0f;
		fslider35 = 0.0f;
		fslider36 = 0.0f;
		fslider37 = 0.0f;
		fslider38 = 1.0f;
		for (int i=0; i<524288; i++) fVec3[i] = 0;
		fslider39 = 0.0f;
		for (int i=0; i<2; i++) fRec3[i] = 0;
		fslider40 = 0.0f;
		fslider41 = 0.0f;
		fslider42 = 0.0f;
		fslider43 = 0.0f;
		fslider44 = 0.0f;
		fslider45 = 0.0f;
		fslider46 = 1.0f;
		fslider47 = 1.0f;
		fslider48 = 1.0f;
		for (int i=0; i<524288; i++) fVec4[i] = 0;
		fslider49 = 0.0f;
		for (int i=0; i<2; i++) fRec4[i] = 0;
		fslider50 = 1.0f;
		fslider51 = 1.0f;
		fslider52 = 0.0f;
		fslider53 = 0.0f;
		fslider54 = 0.0f;
		fslider55 = 0.0f;
		fslider56 = 0.0f;
		fslider57 = 0.0f;
		fslider58 = 1.0f;
		for (int i=0; i<524288; i++) fVec5[i] = 0;
		fslider59 = 0.0f;
		for (int i=0; i<2; i++) fRec5[i] = 0;
		fslider60 = 0.0f;
		fslider61 = 0.0f;
		fslider62 = 0.0f;
		fslider63 = 0.0f;
		fslider64 = 0.0f;
		fslider65 = 0.0f;
		fslider66 = 1.0f;
		fslider67 = 1.0f;
		fslider68 = 1.0f;
		fslider69 = 0.0f;
		fslider70 = 0.0f;
		fslider71 = 0.0f;
		fslider72 = 0.0f;
		fslider73 = 0.0f;
		fslider74 = 0.0f;
		fslider75 = 1.0f;
		fslider76 = 1.0f;
		fslider77 = 1.0f;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void buildUserInterface(FaustUI* interface) {
		interface->openVerticalBox("Tapiir");
		interface->openTabBox("");
		interface->openHorizontalBox("Tap 0");
		interface->addVerticalSlider("delay (sec)", &fslider9, 0.0f, 0.0f, 5.0f, 1.000000e-02f);
		interface->addVerticalSlider("gain", &fslider8, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 0", &fslider1, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 1", &fslider0, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 0", &fslider7, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 1", &fslider6, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 2", &fslider5, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 3", &fslider4, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 4", &fslider3, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 5", &fslider2, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->closeBox();
		interface->openHorizontalBox("Tap 1");
		interface->addVerticalSlider("delay (sec)", &fslider19, 0.0f, 0.0f, 5.0f, 1.000000e-02f);
		interface->addVerticalSlider("gain", &fslider18, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 0", &fslider15, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 1", &fslider17, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 0", &fslider14, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 1", &fslider13, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 2", &fslider12, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 3", &fslider11, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 4", &fslider10, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 5", &fslider16, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->closeBox();
		interface->openHorizontalBox("Tap 2");
		interface->addVerticalSlider("delay (sec)", &fslider29, 0.0f, 0.0f, 5.0f, 1.000000e-02f);
		interface->addVerticalSlider("gain", &fslider28, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 0", &fslider21, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 1", &fslider20, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 0", &fslider27, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 1", &fslider26, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 2", &fslider25, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 3", &fslider24, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 4", &fslider23, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 5", &fslider22, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->closeBox();
		interface->openHorizontalBox("Tap 3");
		interface->addVerticalSlider("delay (sec)", &fslider39, 0.0f, 0.0f, 5.0f, 1.000000e-02f);
		interface->addVerticalSlider("gain", &fslider38, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 0", &fslider31, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 1", &fslider30, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 0", &fslider37, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 1", &fslider36, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 2", &fslider35, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 3", &fslider34, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 4", &fslider33, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 5", &fslider32, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->closeBox();
		interface->openHorizontalBox("Tap 4");
		interface->addVerticalSlider("delay (sec)", &fslider49, 0.0f, 0.0f, 5.0f, 1.000000e-02f);
		interface->addVerticalSlider("gain", &fslider48, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 0", &fslider47, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 1", &fslider46, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 0", &fslider45, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 1", &fslider44, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 2", &fslider43, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 3", &fslider42, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 4", &fslider41, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 5", &fslider40, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->closeBox();
		interface->openHorizontalBox("Tap 5");
		interface->addVerticalSlider("delay (sec)", &fslider59, 0.0f, 0.0f, 5.0f, 1.000000e-02f);
		interface->addVerticalSlider("gain", &fslider58, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 0", &fslider51, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 1", &fslider50, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 0", &fslider57, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 1", &fslider56, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 2", &fslider55, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 3", &fslider54, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 4", &fslider53, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 5", &fslider52, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->closeBox();
		interface->closeBox();
		interface->openVerticalBox("outputs");
		interface->openHorizontalBox("output 0");
		interface->addVerticalSlider("gain", &fslider68, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 0", &fslider66, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 1", &fslider67, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 0", &fslider62, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 1", &fslider61, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 2", &fslider60, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 3", &fslider64, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 4", &fslider63, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 5", &fslider65, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->closeBox();
		interface->openHorizontalBox("output 1");
		interface->addVerticalSlider("gain", &fslider77, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 0", &fslider76, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("input 1", &fslider75, 1.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 0", &fslider74, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 1", &fslider73, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 2", &fslider72, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 3", &fslider71, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 4", &fslider70, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->addVerticalSlider("tap 5", &fslider69, 0.0f, 0.0f, 1.0f, 0.1f);
		interface->closeBox();
		interface->closeBox();
		interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		float 	fSlow0 = fslider0;
		float 	fSlow1 = fslider1;
		float 	fSlow2 = fslider2;
		float 	fSlow3 = fslider3;
		float 	fSlow4 = fslider4;
		float 	fSlow5 = fslider5;
		float 	fSlow6 = fslider6;
		float 	fSlow7 = fslider7;
		float 	fSlow8 = fslider8;
		int 	iSlow9 = int((int((fSamplingFreq * fslider9)) & 524287));
		float 	fSlow10 = fslider10;
		float 	fSlow11 = fslider11;
		float 	fSlow12 = fslider12;
		float 	fSlow13 = fslider13;
		float 	fSlow14 = fslider14;
		float 	fSlow15 = fslider15;
		float 	fSlow16 = fslider16;
		float 	fSlow17 = fslider17;
		float 	fSlow18 = fslider18;
		int 	iSlow19 = int((int((fSamplingFreq * fslider19)) & 524287));
		float 	fSlow20 = fslider20;
		float 	fSlow21 = fslider21;
		float 	fSlow22 = fslider22;
		float 	fSlow23 = fslider23;
		float 	fSlow24 = fslider24;
		float 	fSlow25 = fslider25;
		float 	fSlow26 = fslider26;
		float 	fSlow27 = fslider27;
		float 	fSlow28 = fslider28;
		int 	iSlow29 = int((int((fSamplingFreq * fslider29)) & 524287));
		float 	fSlow30 = fslider30;
		float 	fSlow31 = fslider31;
		float 	fSlow32 = fslider32;
		float 	fSlow33 = fslider33;
		float 	fSlow34 = fslider34;
		float 	fSlow35 = fslider35;
		float 	fSlow36 = fslider36;
		float 	fSlow37 = fslider37;
		float 	fSlow38 = fslider38;
		int 	iSlow39 = int((int((fSamplingFreq * fslider39)) & 524287));
		float 	fSlow40 = fslider40;
		float 	fSlow41 = fslider41;
		float 	fSlow42 = fslider42;
		float 	fSlow43 = fslider43;
		float 	fSlow44 = fslider44;
		float 	fSlow45 = fslider45;
		float 	fSlow46 = fslider46;
		float 	fSlow47 = fslider47;
		float 	fSlow48 = fslider48;
		int 	iSlow49 = int((int((fSamplingFreq * fslider49)) & 524287));
		float 	fSlow50 = fslider50;
		float 	fSlow51 = fslider51;
		float 	fSlow52 = fslider52;
		float 	fSlow53 = fslider53;
		float 	fSlow54 = fslider54;
		float 	fSlow55 = fslider55;
		float 	fSlow56 = fslider56;
		float 	fSlow57 = fslider57;
		float 	fSlow58 = fslider58;
		int 	iSlow59 = int((int((fSamplingFreq * fslider59)) & 524287));
		float 	fSlow60 = fslider60;
		float 	fSlow61 = fslider61;
		float 	fSlow62 = fslider62;
		float 	fSlow63 = fslider63;
		float 	fSlow64 = fslider64;
		float 	fSlow65 = fslider65;
		float 	fSlow66 = fslider66;
		float 	fSlow67 = fslider67;
		float 	fSlow68 = fslider68;
		float 	fSlow69 = fslider69;
		float 	fSlow70 = fslider70;
		float 	fSlow71 = fslider71;
		float 	fSlow72 = fslider72;
		float 	fSlow73 = fslider73;
		float 	fSlow74 = fslider74;
		float 	fSlow75 = fslider75;
		float 	fSlow76 = fslider76;
		float 	fSlow77 = fslider77;
		FAUSTFLOAT* input0 = input[0];
		FAUSTFLOAT* input1 = input[1];
		FAUSTFLOAT* output0 = output[0];
		FAUSTFLOAT* output1 = output[1];
		for (int i=0; i<count; i++) {
			float fTemp0 = (float)input1[i];
			float fTemp1 = (float)input0[i];
			fVec0[IOTA&524287] = (fSlow8 * ((((((((fSlow7 * fRec0[1]) + (fSlow6 * fRec1[1])) + (fSlow5 * fRec2[1])) + (fSlow4 * fRec3[1])) + (fSlow3 * fRec4[1])) + (fSlow2 * fRec5[1])) + (fSlow1 * fTemp1)) + (fSlow0 * fTemp0)));
			fRec0[0] = fVec0[(IOTA-iSlow9)&524287];
			fVec1[IOTA&524287] = (fSlow18 * (((((((fSlow17 * fTemp0) + ((fSlow16 * fRec5[1]) + (fSlow15 * fTemp1))) + (fSlow14 * fRec0[1])) + (fSlow13 * fRec1[1])) + (fSlow12 * fRec2[1])) + (fSlow11 * fRec3[1])) + (fSlow10 * fRec4[1])));
			fRec1[0] = fVec1[(IOTA-iSlow19)&524287];
			fVec2[IOTA&524287] = (fSlow28 * ((((((((fSlow27 * fRec0[1]) + (fSlow26 * fRec1[1])) + (fSlow25 * fRec2[1])) + (fSlow24 * fRec3[1])) + (fSlow23 * fRec4[1])) + (fSlow22 * fRec5[1])) + (fSlow21 * fTemp1)) + (fSlow20 * fTemp0)));
			fRec2[0] = fVec2[(IOTA-iSlow29)&524287];
			fVec3[IOTA&524287] = (fSlow38 * ((((((((fSlow37 * fRec0[1]) + (fSlow36 * fRec1[1])) + (fSlow35 * fRec2[1])) + (fSlow34 * fRec3[1])) + (fSlow33 * fRec4[1])) + (fSlow32 * fRec5[1])) + (fSlow31 * fTemp1)) + (fSlow30 * fTemp0)));
			fRec3[0] = fVec3[(IOTA-iSlow39)&524287];
			fVec4[IOTA&524287] = (fSlow48 * ((((((((fSlow47 * fTemp1) + (fSlow46 * fTemp0)) + (fSlow45 * fRec0[1])) + (fSlow44 * fRec1[1])) + (fSlow43 * fRec2[1])) + (fSlow42 * fRec3[1])) + (fSlow41 * fRec4[1])) + (fSlow40 * fRec5[1])));
			fRec4[0] = fVec4[(IOTA-iSlow49)&524287];
			fVec5[IOTA&524287] = (fSlow58 * ((((((((fSlow57 * fRec0[1]) + (fSlow56 * fRec1[1])) + (fSlow55 * fRec2[1])) + (fSlow54 * fRec3[1])) + (fSlow53 * fRec4[1])) + (fSlow52 * fRec5[1])) + (fSlow51 * fTemp1)) + (fSlow50 * fTemp0)));
			fRec5[0] = fVec5[(IOTA-iSlow59)&524287];
			output0[i] = (FAUSTFLOAT)(fSlow68 * (((((fSlow67 * fTemp0) + ((fSlow66 * fTemp1) + ((fSlow65 * fRec5[0]) + ((fSlow64 * fRec3[0]) + (fSlow63 * fRec4[0]))))) + (fSlow62 * fRec0[0])) + (fSlow61 * fRec1[0])) + (fSlow60 * fRec2[0])));
			output1[i] = (FAUSTFLOAT)(fSlow77 * ((((((((fSlow76 * fTemp1) + (fSlow75 * fTemp0)) + (fSlow74 * fRec0[0])) + (fSlow73 * fRec1[0])) + (fSlow72 * fRec2[0])) + (fSlow71 * fRec3[0])) + (fSlow70 * fRec4[0])) + (fSlow69 * fRec5[0])));
			// post processing
			fRec5[1] = fRec5[0];
			fRec4[1] = fRec4[0];
			fRec3[1] = fRec3[0];
			fRec2[1] = fRec2[0];
			fRec1[1] = fRec1[0];
			fRec0[1] = fRec0[0];
			IOTA = IOTA+1;
		}
	}
};


#endif	//_TAPIIR_DSP_H
