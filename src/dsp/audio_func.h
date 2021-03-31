#ifndef _AUDIO_FUNC_
#define _AUDIO_FUNC_

#include <math.h>
#include <array>

/*
Implements a inheritable function which implements:
    A bi-quad filter (For IIR filters, direct form I)
        Direct form I is utilized since no additional buffer is required.

        (maybe FIR?)
    A symmetric/antisymmetric form FIR filter
        For similar reasons as above, and as to reduce computational time.

Considerations:
    What if cascading filters is desired?
        Then the function object itself must contain a buffer
    What is the required buffer size?
        The number of taps
    What if there are discrepancies between buffer sizes?
        Inherit from the next filter OR
        Use two buffers per filter
    
Many buffers are necessary for it to work as a pipeline

IN      F1          F2          OUT
b_out   b_in b_out  b_in b_out  b_in
{   1      } {     2   } {    3    }

What if instead, output and input buffers share the same memory space while keeping
track of the number of required elements?

Using two buffers per filter however, would result in a more efficient parallel pipeline
but for single core purposes, sharing the buffers is better

For this reason, a number of buffers will be implemented in the DSP class itself,
which represents the DSP pipeline
*/

#define BATCH_SIZE 64



class Audio_Func {

    public:

        Audio_Func();
        
        /* Not needed, being overloaded anyways
        int operator() (double* data_in);
        int operator() (double* data_in, double* data_out);
        */

        std::array<double, BATCH_SIZE> buf_in;
        std::array<double, BATCH_SIZE> buf_out;

        int in_coeffs = 1;
        int out_coeffs = 0;

        /*
        double* get_data_in_ptr() {
            return data_in;
        }

        double* get_data_out_ptr() {
            return data_out;
        }
        */

};

#endif