#ifndef _EQUALIZER_
#define _EQUALIZER_

class Equalizer {

    public:
        
        Equalizer(int* values_db);
        int operator() (int* data);

    private:

        //int gain;

};

#endif