#ifndef _FADE_
#define _FADE_

class Fade {

    public:
        
        Fade(int fade_in_ms, int fade_out_ms, int fade_db);
        int operator() (int* data);

    private:

        //int gain;

};

#endif