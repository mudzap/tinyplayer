#ifndef _GAIN_
#define _GAIN_

class Gain {

    public:
        
        Gain(int gain_db);
        int operator() (int* data);

    private:

        int gain;

};

#endif