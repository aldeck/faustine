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
// name: "fir"
//
// Code generated with Faust 0.9.9.6b11doc2 (http://faust.grame.fr)
//-----------------------------------------------------
#ifndef _FIR_DSP_H
#define _FIR_DSP_H


#include "FaustDSP.h"
#include "FaustUI.h"

#include <math.h>


class FirDSP : public FaustDSP {
  private:
        float   fVec0[3];
        float   fConst0;
        float   fConst1;
        float   fConst2;
        float   fRec0[3];
  public:
        static void metadata(Meta* m) {
        }

        virtual int getNumInputs()      { return 1; }
        virtual int getNumOutputs()     { return 1; }

        static void classInit(int samplingFreq) {
        }


        virtual void instanceInit(int samplingFreq) {
                fSamplingFreq = samplingFreq;
                for (int i=0; i<3; i++) fVec0[i] = 0;
                fConst0 = expf((0 - (314.159265f / fSamplingFreq)));
                fConst1 = (0.5f * (1 - (fConst0 * fConst0)));
                fConst2 = (2 * cosf((2764.601535f / fSamplingFreq)));
                for (int i=0; i<3; i++) fRec0[i] = 0;
        }


        virtual void init(int samplingFreq) {
                classInit(samplingFreq);
                instanceInit(samplingFreq);
        }


        virtual void buildUserInterface(FaustUI* interface) {
                interface->openVerticalBox("cpgr");
                interface->closeBox();
        }


        virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
                FAUSTFLOAT* input0 = input[0];
                FAUSTFLOAT* output0 = output[0];
                for (int i=0; i<count; i++) {
                        float fTemp0 = (float)input0[i];
                        fVec0[0] = fTemp0;
                        fRec0[0] = ((fConst0 * ((fConst2 * fRec0[1]) - (fConst0 * fRec0[2]))) + (fConst1 * (fVec0[0] - fVec0[2])));
                        output0[i] = (FAUSTFLOAT)fRec0[0];
                        // post processing
                        fRec0[2] = fRec0[1]; fRec0[1] = fRec0[0];
                        fVec0[2] = fVec0[1]; fVec0[1] = fVec0[0];
                }
        }
};


#endif /* _FIR_DSP_H */
