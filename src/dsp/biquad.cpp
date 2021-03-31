#include "biquad.h"

Biquad::Biquad() {
    coeffs[0] = 1.0;
    coeffs[1] = 0.0;
    coeffs[2] = 0.0;
    coeffs[3] = 0.0;
    coeffs[4] = 0.0;
    coeffs[5] = 0.0;
}

Biquad::Biquad(Biquad_Type type, double cutoff_freq, double sampling_freq, double Q, double db_gain){

    compute_vars(cutoff_freq, sampling_freq, Q, db_gain);

    float a_0 = 0;

    switch (type) {

        case BQ_LP:
            coeffs[0] = (1.0 - cos_w_0)*0.5;    // b_0
            coeffs[1] = (1.0 - cos_w_0);        // b_1
            coeffs[2] = (1.0 - cos_w_0)*0.5;    // b_2
            a_0 = 1.0 + a;                      // a_0
            coeffs[3] = -2 * cos_w_0;           // a_1
            coeffs[4] = 1.0 - a;                // a_2
            break;

        case BQ_HP:
            coeffs[0] = (1.0 + cos_w_0)*0.5;
            coeffs[1] = -(1.0 + cos_w_0);
            coeffs[2] = (1.0 + cos_w_0)*0.5;
            a_0 = 1.0 + a;
            coeffs[3] = -2 * cos_w_0;
            coeffs[4] = 1.0 - a;
            break;

        case BQ_BPF_0DB: //Band pass with maximum gain of 0dB
            coeffs[0] = a;
            coeffs[1] = 0.0;
            coeffs[2] = -a;
            a_0 = 1.0 + a;
            coeffs[3] = -2 * cos_w_0;
            coeffs[4] = 1.0 - a;
            break;

        case BQ_NOTCH:
            coeffs[0] = 1.0;
            coeffs[1] = -2 * cos_w_0;
            coeffs[2] = 1.0;
            a_0 = 1.0 + a;
            coeffs[3] = -2 * cos_w_0;
            coeffs[4] = 1.0 - a;
            break;

        case BQ_AP:
            coeffs[0] = 1.0 - a;
            coeffs[1] = -2 * cos_w_0;
            coeffs[2] = 1.0 + a;
            a_0 = 1.0 + a;
            coeffs[3] = -2 * cos_w_0;
            coeffs[4] = 1.0 - a;
            break;

        case BQ_PEAKING_EQ:
            coeffs[0] = 1.0 + a*A;
            coeffs[1] = -2 * cos_w_0;
            coeffs[2] = 1.0 - a*A;
            a_0 = 1.0 + a/A;
            coeffs[3] = -2 * cos_w_0;
            coeffs[4] = 1.0 - a/A;
            break;

        case BQ_LOW_SHELF:
            coeffs[0] = A*( (A+1) - (A-1) * cos_w_0 + 2*sqrt(A)*a );
            coeffs[1] = 2*A*( (A-1) - (A+1) * cos_w_0);
            coeffs[2] = A*( (A+1) - (A-1) * cos_w_0 - 2*sqrt(A)*a );
            a_0 = (A+1) + (A-1) * cos_w_0 + 2*sqrt(A)*a;
            coeffs[3] = -2*( (A-1) + (A+1) * cos_w_0);
            coeffs[4] = (A+1) + (A-1) * cos_w_0 - 2*sqrt(A)*a;
            break;

        case BQ_HIGH_SHELF:
            coeffs[0] = A*( (A+1) + (A-1) * cos_w_0 + 2*sqrt(A)*a );
            coeffs[1] = -2*A*( (A-1) + (A+1) * cos_w_0);
            coeffs[2] = A*( (A+1) + (A-1) * cos_w_0 - 2*sqrt(A)*a );
            a_0 = (A+1) - (A-1) * cos_w_0 + 2*sqrt(A)*a;
            coeffs[3] = 2*( (A-1) - (A+1) * cos_w_0);
            coeffs[4] = (A+1) - (A-1) * cos_w_0 - 2*sqrt(A)*a;
            break;

        default: //H(w) = 1
            coeffs[0] = 1.0;
            coeffs[1] = 0.0;
            coeffs[2] = 0.0;
            a_0 = 1.0;
            coeffs[3] = 0.0;
            coeffs[4] = 0.0;
            printf("Invalid filter type for given parameters\n");

    }

    //Normalize all coefficients
    for(double &coeff : coeffs) {
        coeff = coeff / a_0;
    }

}

int Biquad::compute_vars(double cutoff_freq, double sampling_freq, double Q, double db_gain) {
    double A = pow(10.f, db_gain/40.f);
    double f_0 = cutoff_freq;
    double F_s = sampling_freq;
    double w_0 = 2*PI*f_0/F_s;
    double cos_w_0 = cos(w_0);
    double sin_w_0 = sin(w_0);
    double a = sin_w_0 / (2 * Q);
}

int Biquad::operator() (double* data_in, double* data_out){

    /*
    As to prevent too many pointer dereferencing, delayed signals
    will be stored in a variable within the same biquad object
    */

    for(int i = 0; i < BATCH_SIZE; i++) {
        buf_out[i] = data_in[i]*coeffs[0] + x_1*coeffs[1] + x_2*coeffs[2] - y_1*coeffs[3] - y_2*coeffs[4];
        x_2 = x_1;
        x_1 = data_in[i];
        y_2 = y_1;
        y_1 = buf_out[i];
    }

}