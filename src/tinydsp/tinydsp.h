#ifndef _TINYDSP_
#define _TINYDSP_

//EDICION DE PRUEBA

/* Proposal:

    DSP should be configurable in multiple steps:
    Many common DSP operations are lineal, so order MOSTLY doesn't matter

    Being just a music player, complex effects are not necessary at all.
    Proposed effects:
        LP, HP, BP filters (Shelving, 1st order, 2nd order, Butterworth, etc...)
        Equalizer
        Fade In/Out
        Crossfeed (bs2b?)
        Gain (+ ReplayGain?)
        Phase compensation maybe?
        Stereo to Mono
        L/R Balance

    This is not meant to be a professional solution, but some of these would be nice

    Some effects require a larger buffer than others. Some are temporal, consider how these play along

    For different sample rates, precomputing the filters for all common sample rates could be ideal.
    Otherwise, resampling could work for sample rates multiples of the most common ones (44.1k and 48k)
    Support:
    44.1k,  88.2k,  176.4k
    48k,    96k,    192k

    Maybe support:
    11.025k,22.05k
    8k,     16k

    As for sampling resolution, similarly, upsampling could be ideal followed by downsampling.
    I don't see any gain from it, but it can be done implicitelly by using float types.
    Since the most common bit-depths are of 8, 16 and 24 bits, it could be easily considered.
    Of course, batching would be the best.

    As for convolution, since these filters and operations are precomputed, it could be done
    on the frequency domain itself. The size of the fft (and consequential memory requirements)
    could be worrying, though. Maybe try this method with filters which require many poles.

    As for the operations themselves, the objective would be for a customizable DSP pipeline.

    In -> DSP -> Stream

    Example:
    In ---> LP --> Crossfade --> Stream

    A vector of function pointers could be ideal. Filters must be precomputed on runtime.
    Function objects are a possiblity, however they can't be stored in the same array, however
    an array of pointers to function objects and their type can be stored. This is useful because
    it provides the possibility for initialization (i.e: precomputation) of the filter.

    As for data conversion, consider:
    Int -> float -> Int
    This is not ideal

    ALSA, however, supports a float(both 32 and 64 bit) format, so only an initial conversion is required
    Filters will utilize double float precision, the output can be casted easily to a cast at the output.

    32768

    Lastly, all this should be done in a batched form, as to avoid excessive memory transfers
*/

#define TINYDSP_FILTERS_USES_STACK
#define TINYDSP_BUFFERS_USES_STACK

#ifdef TINYDSP_FILTERS_USES_STACK
#define TINYDSP_MAX_FILTERS 16
#endif

#ifdef TINYDSP_BUFFERS_USES_STACK
#define TINYDSP_MAX_BUFFER_SIZE 64
#endif


#include "crossfade.h"
#include "downmix.h"
#include "fade.h"
#include "gain.h"
#include "hp.h"
#include "lp.h"
#include "lr_bal.h"
#include "eq.h"

#include <vector>
#include <array>
#include <memory>
#include <stdio.h>


typedef enum Filter_Type{
    TINYDSP_LOW_PASS,
    TINYDSP_HIGH_PASS,
    TINYDSP_EQ,
    TINYDSP_DOWNMIX,
    TINYDSP_GAIN,
    TINYDSP_LR_BALANCE,
    TINYDSP_XFADE,
    TINYDSP_NO_FILTER
} Filter_Type;

typedef enum Error_Type{
    TINYDSP_INVALID_FILTER,
    TINYDSP_ARRAY_FILLED
} Filter_Type;

#ifdef TINYDSP_FILTERS_USES_STACK
union Filter_Func {
    LP low_pass;
    HP high_pass;
    Equalizer eq;
    XFade xfade;
    Gain gain;
    LR_Balance lr_bal;
};
#endif

typedef struct Filter_Op {
    Filter_Type type = TINYDSP_NO_FILTER; //????
    
#ifdef TINYDSP_FILTERS_USES_STACK
    Filter_Func filter_func;
    int buffer[TINYDSP_MAX_BUFFER_SIZE];
#else
    std::unique_ptr<void> filter_ptr;
    std::vector<int> buffer;
#endif

    Filter_Op(Filter_Type init_type) :
        type(init_type) {
            for (int& data : buffer)
                data = 0;
    }

#ifdef TINYDSP_FILTERS_USES_STACK
    Filter_Op(Filter_Type init_type,  Filter_Func init_func) :
        type(init_type), filter_func(init_func) {
            for (int& data : buffer)
                data = 0;
    }
#else
    Filter_Op(Filter_Type init_type, void* init_ptr) :
        type(init_type), filter_ptr(init_ptr) {
            for (int& data : buffer)
                data = 0;
    }
#endif

} Filter_Op;


class DSP {

    public:

        DSP();

        int add_filter(Filter_Type type, double data[]);

    private:
        
        //std::vector<int*> dsp_buffers[MAX_FILTERS]; //Contains data from input

#ifdef TINYDSP_FILTERS_USES_STACK
        std::array<Filter_Op, TINYDSP_MAX_FILTERS> filters;
        int filter_index;
#else
        std::vector<Filter_Op> filters; //why is stl so ugly
#endif

};

#endif
