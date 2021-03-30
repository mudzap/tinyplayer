#ifndef _STREAM_
#define _STREAM_

#define RATE        44100
#define CHANNELS    2

#include <iostream>
#include <alsa/asoundlib.h>
#include "buffer.h"

//Use asynchronous notification transfer method from {ALSA}/tests/pcm.c

typedef struct {
    signed short *samples;
    snd_pcm_channel_area_t *areas; /*Area contains: Address, offset and step (addr, first, step)*/
} async_private_data;


class Stream {

    public:

        Stream();
        Stream(const char* name, snd_async_callback_t callback, void* data);

    private:

        snd_async_handler_t** async_handler = NULL;
        snd_pcm_t* pcm_handle = NULL;
        snd_async_callback_t async_callback = NULL;
        void* callback_data = NULL;

        snd_pcm_hw_params_t *hw_params;

        const char* pcm_name = "Default_Name";
        unsigned char out_buffer[16*1024];
        unsigned char in_buffer[16*1024];

        static snd_pcm_sframes_t buffer_size;
        static snd_pcm_sframes_t period_size;

        static void async_write_callback(snd_async_handler_t *async_handler);

    public:

        bool failure = false;
        snd_pcm_state_t get_state();
        void write_frame_i();

        void setup_pcm(const char* name,
            snd_async_callback_t callback,
            void* data);
        int setup_hwparams();

};

#endif