#ifndef _STREAM_
#define _STREAM_

#include <iostream>
#include <alsa/asoundlib.h>

#include <poll.h>
#include "buffer.h"

typedef struct pollfd pollfd;

class Stream {

    public:

        Stream();
        Stream(const char* name);
        ~Stream();

    private:

        snd_pcm_t* pcm_handle;

        snd_pcm_hw_params_t *hw_params;

        const char* pcm_name = NULL;
        unsigned char out_buffer[16*1024];
        unsigned char in_buffer[16*1024];

        snd_pcm_sframes_t buffer_size = 1024;
        snd_pcm_sframes_t period_size = 64;
        int rate = 44100;
        int channels = 2;


    public:

        void setup_all();
        void setup_pcm(const char* name);
        int setup_hwparams();

        int wait_for_poll(snd_pcm_t *handle, pollfd *ufds, unsigned int count);
        int write_and_poll_loop(snd_pcm_t *handle,
                   signed short *samples,
                   snd_pcm_channel_area_t *areas);
        int xrun_recovery(snd_pcm_t *handle, int err);
        

};

#endif