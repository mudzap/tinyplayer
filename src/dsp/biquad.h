#ifndef _BIQUAD_
#define _BIQUAD_

#define PI 3.1415192

#include <math.h>
#include <memory>

#include "audio_func.h"

typedef enum Biquad_Type {
    BQ_LP,
    BQ_HP,
    BQ_BPF_0DB,
    BQ_NOTCH,
    BQ_AP,
    BQ_PEAKING_EQ,
    BQ_LOW_SHELF,
    BQ_HIGH_SHELF
} Biquad_Type;

class Biquad: public Audio_Func {

    public:
        
        //THESE ARE MOSTLY PRECOMPUTATIONS, DO NOT OPTIMIZE
        //https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

        Biquad(Biquad_Type type, double cutoff_freq, double sampling_freq, double Q, double db_gain = 0.0);

        int operator() (double* data_in, double* data_out);

    private:

        int compute_vars(double cutoff_freq, double sampling_freq, double Q, double db_gain);

        //b_0, b_1, b_2, a_0, a_1, a_2
        double coeffs[6];

        //params for precomputing
        double A;
        double f_0;
        double F_s;
        double w_0;
        double cos_w_0;
        double sin_w_0;
        double a;

};

#endif