#ifndef _EQUALIZER_
#define _EQUALIZER_

//This is just a lot of biquad filters

#include "audio_func.h"

class Equalizer : public Audio_Func {

    public:
        
        Equalizer(int* values_db);
        int operator() (int* data);

    private:

        //int gain;

};

#endif