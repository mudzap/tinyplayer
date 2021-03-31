#ifndef _AUDIO_FUNC_
#define _AUDIO_FUNC_

#include <math.h>
#include <array>

/*
Implements a inheritable function which implements:
    A bi-quad filter (For IIR filters, direct form I)
        Direct form I is utilized since no additional buffer is required.
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

Decimation should also be implemented for FIR (maybe?)
*/

#define BATCH_SIZE 64


typedef std::array<double, BATCH_SIZE> pcm_array;

class Audio_Func {

    public:

        Audio_Func();
        
        int operator() (double* data_in, int batch_size) {


        }

        int operator() (double* data_in, double* data_out, int batch_size) {


        }

        pcm_array data_in[BATCH_SIZE];
        pcm_array data_out[BATCH_SIZE];

        int in_coeffs = 1;
        int out_coeffs = 1;

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