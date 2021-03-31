#include "biquad.h"


Biquad::Biquad(Biquad_Type type, int cutoff_freq, int sampling_freq, double Q, double db_gain){

    compute_vars(cutoff_freq, sampling_freq, Q, db_gain);

    switch (type) {

        case BQ_LP:
            coeffs[0] = (1.0 - cos_w_0)*0.5;
            coeffs[1] = (1.0 - cos_w_0);
            coeffs[2] = (1.0 - cos_w_0)*0.5;
            coeffs[3] = 1.0 + a;
            coeffs[4] = -2 * cos_w_0;
            coeffs[5] = 1.0 - a;
            break;

        case BQ_HP:
            coeffs[0] = (1.0 + cos_w_0)*0.5;
            coeffs[1] = -(1.0 + cos_w_0);
            coeffs[2] = (1.0 + cos_w_0)*0.5;
            coeffs[3] = 1.0 + a;
            coeffs[4] = -2 * cos_w_0;
            coeffs[5] = 1.0 - a;
            break;

        case BQ_BPF_0DB:
            coeffs[0] = a;
            coeffs[1] = 0.0;
            coeffs[2] = -a;
            coeffs[3] = 1.0 + a;
            coeffs[4] = -2 * cos_w_0;
            coeffs[5] = 1.0 - a;
            break;

        case BQ_NOTCH:
            coeffs[0] = 1.0;
            coeffs[1] = -2 * cos_w_0;
            coeffs[2] = 1.0;
            coeffs[3] = 1.0 + a;
            coeffs[4] = -2 * cos_w_0;
            coeffs[5] = 1.0 - a;
            break;

        case BQ_AP:
            coeffs[0] = 1.0 - a;
            coeffs[1] = -2 * cos_w_0;
            coeffs[2] = 1.0 + a;
            coeffs[3] = 1.0 + a;
            coeffs[4] = -2 * cos_w_0;
            coeffs[5] = 1.0 - a;
            break;

        case BQ_PEAKING_EQ:
            coeffs[0] = 1.0 + a*A;
            coeffs[1] = -2 * cos_w_0;
            coeffs[2] = 1.0 - a*A;
            coeffs[3] = 1.0 + a/A;
            coeffs[4] = -2 * cos_w_0;
            coeffs[5] = 1.0 - a/A;
            break;

        case BQ_LOW_SHELF:
            coeffs[0] = A*( (A+1) - (A-1) * cos_w_0 + 2*sqrt(A)*a );
            coeffs[1] = 2*A*( (A-1) - (A+1) * cos_w_0);
            coeffs[2] = A*( (A+1) - (A-1) * cos_w_0 - 2*sqrt(A)*a );
            coeffs[3] = (A+1) + (A-1) * cos_w_0 + 2*sqrt(A)*a;
            coeffs[4] = -2*( (A-1) + (A+1) * cos_w_0);
            coeffs[5] = (A+1) + (A-1) * cos_w_0 - 2*sqrt(A)*a;
            break;

        case BQ_HIGH_SHELF:
            coeffs[0] = A*( (A+1) + (A-1) * cos_w_0 + 2*sqrt(A)*a );
            coeffs[1] = -2*A*( (A-1) + (A+1) * cos_w_0);
            coeffs[2] = A*( (A+1) + (A-1) * cos_w_0 - 2*sqrt(A)*a );
            coeffs[3] = (A+1) - (A-1) * cos_w_0 + 2*sqrt(A)*a;
            coeffs[4] = 2*( (A-1) - (A+1) * cos_w_0);
            coeffs[5] = (A+1) - (A-1) * cos_w_0 - 2*sqrt(A)*a;
            break;

        default: //H(w) = 1
            coeffs[0] = 1.0;
            coeffs[1] = 0.0;
            coeffs[2] = 0.0;
            coeffs[3] = 0.0;
            coeffs[4] = 0.0;
            coeffs[5] = 0.0;
            printf("Invalid filter type for given parameters\n");

    }
}

int Biquad::compute_vars(int cutoff_freq, int sampling_freq, double Q, double db_gain) {
    double A = pow(10.f, db_gain/40.f);
    double f_0 = cutoff_freq;
    double F_s = sampling_freq;
    double w_0 = 2*PI*f_0/F_s;
    double cos_w_0 = cos(w_0);
    double sin_w_0 = sin(w_0);
    double a = sin_w_0 / (2 * _Q_);
}