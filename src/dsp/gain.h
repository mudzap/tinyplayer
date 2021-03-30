#ifndef _GAIN_
#define _GAIN_

#include "audio_func.h"

class Gain : public Audio_Func {

    public:
        
        Gain(int gain_db);
        int operator() (int* data);

    private:

        int gain;

};

#endif