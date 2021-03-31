#ifndef _FADE_
#define _FADE_

#include "audio_func.h"

class Fade : public Audio_Func {

    public:
        
        Fade(int fade_in_ms, int fade_out_ms, double fade_db);
        int operator() (double* data);

    private:

        //int gain;

};

#endif