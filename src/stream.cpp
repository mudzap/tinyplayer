#include "stream.h"

snd_pcm_sframes_t Stream::buffer_size = 10;
snd_pcm_sframes_t Stream::period_size = 10;

Stream::Stream() {
    //Do nothing
}

Stream::Stream(const char* name,
            snd_async_callback_t callback,
            void* data) {
    //setup_pcm(name, callback, data);
    setup_pcm(name, Stream::async_write_callback, data);
    setup_hwparams();
}

void Stream::setup_pcm(const char* name,
            snd_async_callback_t callback,
            void* data) {

    pcm_name = name;
    //async_callback = callback;
    async_callback = Stream::async_write_callback;
    callback_data = data;

#ifndef NDEBUG
    printf("Setting up stream with name: %s\n", name);
#endif

    int err;

    err = snd_pcm_open(
        &pcm_handle,
        name,
        SND_PCM_STREAM_PLAYBACK,
        SND_PCM_ASYNC
        );
    

    if(err < 0) {
        printf("Failed to open pcm channel, error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

#ifndef NDEBUG
    printf("Setting up callback function\n");
#endif

    err = snd_async_add_pcm_handler(
        async_handler,
	    pcm_handle,
	    callback,
	    data 
	);

    if(err < 0) {
        printf("Failed to setup callback for async handler, error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
}

int Stream::setup_hwparams() {

    unsigned int rrate;
    snd_pcm_uframes_t size;
    int err, dir;

    /* choose all parameters */
#ifndef NDEBUG
    printf("Getting hw_params\n");
#endif
    err = snd_pcm_hw_params_any(pcm_handle, hw_params);
    if (err < 0) {
        printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
        return err;
    }

#ifndef NDEBUG
    printf("Got: hw_params - %x\n", hw_params);
    //printf("Got: hw_params\n");
#endif


#ifndef NDEBUG
    printf("Setting hardware resampling\n");
#endif
    /* set hardware resampling */
    //err = snd_pcm_hw_params_set_rate_resample(pcm_handle, params, resample);
    if (err < 0) {
        printf("Resampling setup failed for playback: %s\n", snd_strerror(err));
        return err;
    }
    /* set the interleaved read/write format */
    //err = snd_pcm_hw_params_set_access(pcm_handle, params, access);
    if (err < 0) {
        printf("Access type not available for playback: %s\n", snd_strerror(err));
        return err;
    }
    /* set the sample format */
    //err = snd_pcm_hw_params_set_format(pcm_handle, params, format);
    if (err < 0) {
        printf("Sample format not available for playback: %s\n", snd_strerror(err));
        return err;
    }
    /* set the count of channels */
    //err = snd_pcm_hw_params_set_channels(pcm_handle, params, channels);
    if (err < 0) {
        //printf("Channels count (%u) not available for playbacks: %s\n", channels, snd_strerror(err));
        return err;
    }
    /* set the stream rate */
    //rrate = rate;
    //err = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rrate, 0);
    if (err < 0) {
        //printf("Rate %uHz not available for playback: %s\n", rate, snd_strerror(err));
        return err;
    }
    /*if (rrate != rate) {
        printf("Rate doesn't match (requested %uHz, get %iHz)\n", rate, err);
        return -EINVAL;
    }*/
    /* set the buffer time */
    //err = snd_pcm_hw_params_set_buffer_time_near(handle, params, &buffer_time, &dir);
    if (err < 0) {
        //printf("Unable to set buffer time %u for playback: %s\n", buffer_time, snd_strerror(err));
        return err;
    }
    err = snd_pcm_hw_params_get_buffer_size(hw_params, &size);
    if (err < 0) {
        printf("Unable to get buffer size for playback: %s\n", snd_strerror(err));
        return err;
    }
    buffer_size = size;
    /* set the period time */
    //err = snd_pcm_hw_params_set_period_time_near(handle, params, &period_time, &dir);
    if (err < 0) {
        //printf("Unable to set period time %u for playback: %s\n", period_time, snd_strerror(err));
        return err;
    }
    err = snd_pcm_hw_params_get_period_size(hw_params, &size, &dir);
    if (err < 0) {
        printf("Unable to get period size for playback: %s\n", snd_strerror(err));
        return err;
    }
    period_size = size;
    /* write the parameters to device */
    err = snd_pcm_hw_params(pcm_handle, hw_params);
    if (err < 0) {
        printf("Unable to set hw params for playback: %s\n", snd_strerror(err));
        return err;
    }

    return 0;

}
            

void Stream::write_frame_i() {

}

//Use asynchronous notification transfer method from {ALSA}/tests/pcm.c

void Stream::async_write_callback(snd_async_handler_t *async_handler) {
    //Needed, fuck you C++
    snd_pcm_t *pcm_handle = snd_async_handler_get_pcm(async_handler);

    async_private_data *data = static_cast<async_private_data*>(snd_async_handler_get_callback_private(async_handler));
    signed short *samples = data->samples;
    snd_pcm_channel_area_t *areas = data->areas;
    snd_pcm_sframes_t avail;
    int err;
    
    avail = snd_pcm_avail_update(pcm_handle);
    while (avail >= Stream::period_size) {
        //generate_sine(areas, 0, period_size, &data->phase);
        err = snd_pcm_writei(pcm_handle, samples, Stream::period_size);
        if (err < 0) {
            printf("Write error: %s\n", snd_strerror(err));
            exit(EXIT_FAILURE);
        }
        if (err != Stream::period_size) {
            printf("Write error: written %i expected %li\n", err, Stream::period_size);
            exit(EXIT_FAILURE);
        }
        avail = snd_pcm_avail_update(pcm_handle);
    }
}