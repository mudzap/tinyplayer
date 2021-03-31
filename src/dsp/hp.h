#ifndef _HI_PASS_
#define _HI_PASS_

#include "audio_func.h"

//Implemented through a biquad filter

class HP : public Audio_Func {

    public:
        
        HP(int cutoff_freq, int sampling_freq);
        int operator() (int* data);

    private:

        int* filter_params;

};

#endif