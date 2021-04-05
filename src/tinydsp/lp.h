#ifndef _LOW_PASS_
#define _LOW_PASS_

#include "audio_func.h"

class LP : public Audio_Func {

    public:
        
        LP(double cutoff_freq, double sampling_freq);
        int operator() (double* data);

    private:

        double* filter_params;

};

#endif