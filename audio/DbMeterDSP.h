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
// name: "dbmeter"
//
// Code generated with Faust 0.9.9.6b11doc2 (http://faust.grame.fr)
//-----------------------------------------------------
#ifndef _DBMETER_DSP_H
#define _DBMETER_DSP_H


#include "FaustDSP.h"
#include "FaustUI.h"

#include <math.h>


class DbMeterDSP : public FaustDSP {
  private:
	float 	fConst0;
	float 	fRec0[2];
	FAUSTFLOAT 	fbargraph0;
	int 	iConst1;
  public:
	static void metadata(Meta* m) {
		m->declare("copyright", "(c)GRAME 2006");
		m->declare("license", "BSD");
		m->declare("author", "Grame");
		m->declare("version", "1.0");
		m->declare("name", "dbmeter");
	}

	virtual int getNumInputs() 	{ return 1; }
	virtual int getNumOutputs() 	{ return 1; }
	static void classInit(int samplingFreq) {
	}
	virtual void instanceInit(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		fConst0 = (80.0f / fSamplingFreq);
		for (int i=0; i<2; i++) fRec0[i] = 0;
		iConst1 = 0;
	}
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void buildUserInterface(FaustUI* interface) {
		interface->openVerticalBox("dbmeter");
		interface->declare(&fbargraph0, "unit", "dB");
		interface->addVerticalBargraph("", &fbargraph0, -70.0f, 10.0f);
		interface->closeBox();
	}
	virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
		FAUSTFLOAT* input0 = input[0];
		FAUSTFLOAT* output0 = output[0];
		for (int i=0; i<count; i++) {
			float fTemp0 = (float)input0[i];
			fRec0[0] = max((fRec0[1] - fConst0), min(10, (20 * log10f(max(3.162278e-04f, fabsf(fTemp0))))));
			fbargraph0 = fRec0[0];
			output0[i] = (FAUSTFLOAT)iConst1;
			// post processing
			fRec0[1] = fRec0[0];
		}
	}
};


#endif /* _DBMETER_DSP_H */
