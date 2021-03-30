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

    private:

        snd_pcm_t* pcm_handle;

        snd_pcm_hw_params_t *hw_params;
        snd_pcm_sw_params_t *sw_params;

        const char* pcm_name = NULL;
        unsigned char out_buffer[16*1024];
        unsigned char in_buffer[16*1024];

        snd_pcm_sframes_t buffer_size = 2048;
        snd_pcm_sframes_t period_size = 128;
        int rate = 44100;
        int channels = 2;
        snd_pcm_format_t format = SND_PCM_FORMAT_S16;
        unsigned int buffer_time = 500000; //in us
        unsigned int period_time = 100000;  
        snd_pcm_access_t access = SND_PCM_ACCESS_RW_INTERLEAVED;


    public:

        void setup_all();
        void setup_pcm(const char* name);
        int setup_hwparams();
        int set_swparams();

        void free_resources();

        int wait_for_poll(snd_pcm_t *handle, pollfd *ufds, unsigned int count);
        int write_and_poll_loop(snd_pcm_t *handle,
                   signed short *samples,
                   snd_pcm_channel_area_t *areas);
        int xrun_recovery(snd_pcm_t *handle, int err);
        

};

#endif