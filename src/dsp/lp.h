#ifndef _LOW_PASS_
#define _LOW_PASS_

#include "audio_func.h"

class LP : public Audio_Func {

    public:
        
        LP(int cutoff_freq, int sampling_freq);
        int operator() (int* data);

    private:

        int* filter_params;

};

#endif