#ifndef _DOWNMIX_
#define _DOWNMIX_

#include "audio_func.h"

class Downmix : public Audio_Func {

    public:
        
        Downmix();
        int operator() (int* data);

    private:

        //int gain;

};

#endif