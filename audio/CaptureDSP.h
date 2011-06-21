//-----------------------------------------------------
// copyright: "(c)GRAME 2006"
// license: "BSD"
// author: "Grame"
// version: "1.0"
// name: "dbmeter"
//
// Code generated with Faust 0.9.9.6b11doc2 (http://faust.grame.fr)
//-----------------------------------------------------
#ifndef _CAPTURE_DSP_H
#define _CAPTURE_DSP_H

#include "FaustDSP.h"
#include "FaustUI.h"

#include <math.h>


class CaptureDSP : public FaustDSP {
  private:
        FAUSTFLOAT      fbutton0;
        int     IOTA;
        float   fVec0[524288];
        int     iVec1[2];
        int     iRec1[2];
        float   fRec0[2];
        FAUSTFLOAT      fslider0;
        float   fRec2[2];
  public:
        static void metadata(Meta* m) { 
                m->declare("copyright", "(c)GRAME 2006");
                m->declare("license", "BSD");
                m->declare("author", "Grame");
                m->declare("version", "1.0");
                m->declare("name", "capture");
        }

        virtual int getNumInputs()      { return 1; }
        virtual int getNumOutputs()     { return 1; }
        static void classInit(int samplingFreq) {
        }
        virtual void instanceInit(int samplingFreq) {
                fSamplingFreq = samplingFreq;
                fbutton0 = 0.0;
                IOTA = 0;
                for (int i=0; i<524288; i++) fVec0[i] = 0;
                for (int i=0; i<2; i++) iVec1[i] = 0;
                for (int i=0; i<2; i++) iRec1[i] = 0;
                for (int i=0; i<2; i++) fRec0[i] = 0;
                fslider0 = 0.0f;
                for (int i=0; i<2; i++) fRec2[i] = 0;
        }
        virtual void init(int samplingFreq) {
                classInit(samplingFreq);
                instanceInit(samplingFreq);
        }
        virtual void buildUserInterface(FaustUI* interface) {
                interface->openVerticalBox("Audio Capture");
                interface->addButton("Capture", &fbutton0);
                interface->addHorizontalSlider("level (db)", &fslider0, 0.0f, -96.0f, 4.0f, 0.1f);
                interface->closeBox();
        }
        virtual void compute (int count, FAUSTFLOAT** input, FAUSTFLOAT** output) {
                float   fSlow0 = fbutton0;
                float   fSlow1 = (1.0f - fSlow0);
                int     iSlow2 = int(fSlow0);
                float   fSlow3 = (1.000000e-03f * powf(10,(5.000000e-02f * fslider0)));
                FAUSTFLOAT* input0 = input[0];
                FAUSTFLOAT* output0 = output[0];
                for (int i=0; i<count; i++) {
                        fVec0[IOTA&524287] = ((fSlow1 * fRec0[1]) + (fSlow0 * (float)input0[i]));
                        iVec1[0] = iSlow2;
                        iRec1[0] = ((iRec1[1] + iSlow2) * ((iSlow2 - iVec1[1]) <= 0));
                        fRec0[0] = fVec0[(IOTA-int((int((iRec1[0] - 1)) & 524287)))&524287];
                        fRec2[0] = ((0.999f * fRec2[1]) + fSlow3);
                        output0[i] = (FAUSTFLOAT)(fRec2[0] * fRec0[0]);
                        // post processing
                        fRec2[1] = fRec2[0];
                        fRec0[1] = fRec0[0];
                        iRec1[1] = iRec1[0];
                        iVec1[1] = iVec1[0];
                        IOTA = IOTA+1;
                }
        }
};



#endif /* _CAPTURE_DSP_H */

