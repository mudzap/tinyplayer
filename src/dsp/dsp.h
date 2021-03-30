#ifndef _DSP_
#define _DSP_

#include "crossfade.h"
#include "downmix.h"
#include "audio_func.h"
#include "fade.h"
#include "gain.h"
#include "hp.h"
#include "lp.h"
#include "lr_bal.h"
#include "eq.h"

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
*/

class DSP {


    private:
        
        unsigned char* dsp_buffer[16][1024]; //Contains data from input

};

#endif