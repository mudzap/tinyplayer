#ifndef _LR_BALANCE_
#define _LR_BALANCE_

#include "audio_func.h"

class LR_Balance : public Audio_Func {

    public:
        
        LR_Balance(double angle);
        int operator() (double* data);

    private:

        int angle;
        int l_multiplier;
        int r_multiplier;

};

#endif