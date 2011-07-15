/*
 * Copyright 2009-2010, Alexandre Deckner (alex@zappotek.com)
 * Distributed under the terms of the MIT License.
 *
 */
#ifndef _FAUST_DSP_H
#define _FAUST_DSP_H


#include <map>


struct Meta : std::map<const char*, const char*>
{
    void declare (const char* key, const char* value) { (*this)[key]=value; }
};


#define FAUSTFLOAT float
#define max(x,y) (((x)>(y)) ? (x) : (y))
#define min(x,y) (((x)<(y)) ? (x) : (y))


typedef long double quad;

class FaustUI;


class FaustDSP {
 protected:
	int fSamplingFreq;
 public:
	virtual ~FaustDSP() {}

	virtual int getNumInputs() 										= 0;
	virtual int getNumOutputs() 									= 0;
	virtual void buildUserInterface(FaustUI* interface) 			= 0;
	virtual void init(int samplingRate) 							= 0;
 	virtual void compute(int len, float** inputs, float** outputs) 	= 0;
};


#endif /* _FAUST_DSP_H */
