#ifndef _HI_PASS_
#define _HI_PASS_

#include "audio_func.h"

//Implemented through a biquad filter

class HP : public Audio_Func {

    public:
        
        HP(double cutoff_freq, double sampling_freq);
        int operator() (double* data);

    private:

        double* filter_params;

};

#endif