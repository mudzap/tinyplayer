#ifndef _LR_BALANCE_
#define _LR_BALANCE_

class LR_Balance {

    public:
        
        LR_Balance(int angle);
        int operator() (int* data);

    private:

        int angle;
        int l_multiplier;
        int r_multiplier;

};

#endif