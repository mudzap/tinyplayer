#ifndef _LOW_PASS_
#define _LOW_PASS_

class LP {

    public:
        
        LP(int cutoff_freq, int sampling_freq);
        int operator() (int* data);

    private:

        int* filter_params;

};

#endif