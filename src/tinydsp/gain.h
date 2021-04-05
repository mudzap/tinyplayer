#ifndef _GAIN_
#define _GAIN_

#include "audio_func.h"

class Gain : public Audio_Func {

    public:
        
        Gain(double gain_db);
        int operator() (double* data);

    private:

        double gain;

};

#endif