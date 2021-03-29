#ifndef _DSP_
#define _DSP_

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

*/

class DSP {

    //TEST COMMIT
    unsigned char* buffer[16][1024];

};

#endif