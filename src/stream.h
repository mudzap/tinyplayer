#ifndef _STREAM_
#define _STREAM_

#include <iostream>
#include <alsa/asoundlib.h>
#include "buffer.h"

class Stream {

    public:

        Stream();
        Stream(const char* name, snd_async_callback_t callback, void* data);

    private:

        snd_async_handler_t** async_handler = NULL;
        snd_pcm_t* pcm_handle = NULL;
        snd_async_callback_t async_callback = NULL;
        void* callback_data = NULL;

        const char* pcm_name = "Default_Name";
        unsigned char buffer[16*1024];

    public:

        bool failure = false;
        snd_pcm_state_t get_state();
        void write_frame_i();

};

#endif