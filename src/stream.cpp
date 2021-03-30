#include "stream.h"

Stream::Stream() {
    //Do nothing
}

Stream::Stream(const char* name) :
    pcm_name(name) {

    setup_pcm(name);
    setup_hwparams();
}   

Stream::~Stream() {
    snd_pcm_close (pcm_handle);
}

void Stream::setup_pcm(const char* name) {

#ifndef NDEBUG
    printf("Setting up stream with name: %s\n", name);
#endif

    int err;
    err = snd_pcm_open(&pcm_handle, name, SND_PCM_STREAM_PLAYBACK, 0);

    if(err < 0) {
        printf("Failed to open pcm channel, error: %s\n", snd_strerror(err));
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

    err = snd_pcm_hw_params_malloc(&hw_params);

    if (err < 0) {
        printf("Failed to allocate memory for he_params error: %s\n", snd_strerror(err));
        return err;
    }

    err = snd_pcm_hw_params_any(pcm_handle, hw_params);

    if (err < 0) {
        printf("Broken configuration for playback: no configurations available: %s\n", snd_strerror(err));
        return err;
    }

#ifndef NDEBUG
    //Add afterwards to a function
    printf("Got device name: %s\n", pcm_name);
    unsigned int num, den;
    snd_pcm_hw_params_get_rate_numden(hw_params, &num, &den); 
    printf("\tRatio:\t\t%u/%u\n",num, den);
    printf("\tBit Depth:\t%d\n",snd_pcm_hw_params_get_sbits(hw_params));
    printf("\tFIFO Size:\t%u\n",snd_pcm_hw_params_get_fifo_size(hw_params));
    unsigned int min_ch, max_ch, n_ch;
    snd_pcm_hw_params_get_channels(hw_params, &n_ch);
    snd_pcm_hw_params_get_channels_max(hw_params, &max_ch);
    snd_pcm_hw_params_get_channels_min(hw_params, &min_ch);
    printf("\tMinMax Ch.:\t%u\t%u\t%u\n",min_ch, n_ch, max_ch);
    unsigned int min_r, max_r, n_r;
    int min_d, max_d, n_d;
    snd_pcm_hw_params_get_rate(hw_params, &n_r, &n_d);
    snd_pcm_hw_params_get_rate_max(hw_params, &max_r, &max_d);
    snd_pcm_hw_params_get_rate_min(hw_params, &min_r, &min_d);
    printf("\tMinMax Rate:\t%u\t%u\t%u\n",min_r, n_r, max_r);

#endif

#ifndef NDEBUG
    printf("Setting hardware resampling\n");
#endif
    err = snd_pcm_hw_params_set_rate_resample(pcm_handle, hw_params, rate);
    if (err < 0) {
        printf("Resampling setup failed for playback: %s\n", snd_strerror(err));
        return err;
    }

#ifndef NDEBUG
    printf("Setting interleaved read/write format\n");
#endif
    err = snd_pcm_hw_params_set_access(pcm_handle, hw_params, access);
    if (err < 0) {
        printf("Access type not available for playback: %s\n", snd_strerror(err));
        return err;
    }

#ifndef NDEBUG
    printf("Setting sample format\n");
#endif
    err = snd_pcm_hw_params_set_format(pcm_handle, hw_params, format);
    if (err < 0) {
        printf("Sample format not available for playback: %s\n", snd_strerror(err));
        return err;
    }

#ifndef NDEBUG
    printf("Setting number of channels\n");
#endif
    err = snd_pcm_hw_params_set_channels(pcm_handle, hw_params, channels);
    if (err < 0) {
        printf("Channels count (%u) not available for playbacks: %s\n", channels, snd_strerror(err));
        return err;
    }

#ifndef NDEBUG
    printf("Setting stream rate\n");
#endif
    rrate = rate;
    err = snd_pcm_hw_params_set_rate_near(pcm_handle, hw_params, &rrate, 0);
    if (err < 0) {
        printf("Rate %uHz not available for playback: %s\n", rate, snd_strerror(err));
        return err;
    }
    if (rrate != rate) {
        printf("Rate doesn't match (requested %uHz, get %iHz)\n", rate, err);
        return -EINVAL;
    }

#ifndef NDEBUG
    printf("Setting buffer time\n");
#endif  
    err = snd_pcm_hw_params_set_buffer_time_near(pcm_handle, hw_params, &buffer_time, &dir);
    if (err < 0) {
        printf("Unable to set buffer time %u for playback: %s\n", buffer_time, snd_strerror(err));
        return err;
    }
    err = snd_pcm_hw_params_get_buffer_size(hw_params, &size);
    if (err < 0) {
        printf("Unable to get buffer size for playback: %s\n", snd_strerror(err));
        return err;
    }
    buffer_size = size;

#ifndef NDEBUG
    printf("Setting period time\n");
#endif  
    err = snd_pcm_hw_params_set_period_time_near(pcm_handle, hw_params, &period_time, &dir);
    if (err < 0) {
        printf("Unable to set period time %u for playback: %s\n", period_time, snd_strerror(err));
        return err;
    }
    err = snd_pcm_hw_params_get_period_size(hw_params, &size, &dir);
    if (err < 0) {
        printf("Unable to get period size for playback: %s\n", snd_strerror(err));
        return err;
    }
    period_size = size;

#ifndef NDEBUG
    printf("Writing hardware parameters\n");
#endif 
    err = snd_pcm_hw_params(pcm_handle, hw_params);
    if (err < 0) {
        printf("Unable to set hw params for playback: %s\n", snd_strerror(err));
        return err;
    }

    return 0;

}


//Underrun and suspend recovery

int Stream::xrun_recovery(snd_pcm_t *handle, int err)
{
    
#ifndef NDEBUG
    printf("stream recovery\n");
#endif

    if (err == -EPIPE) {    // under-run
        err = snd_pcm_prepare(handle);
        if (err < 0)
            printf("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
        return 0;
    } else if (err == -ESTRPIPE) {
        while ((err = snd_pcm_resume(handle)) == -EAGAIN)
            sleep(1);   // wait until the suspend flag is released 
        if (err < 0) {
            err = snd_pcm_prepare(handle);
            if (err < 0)
                printf("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
        }
        return 0;
    }
    return err;
}

//Use write and wait for room in buffer using poll transfer method from {ALSA}/tests/pcm.c

int Stream::wait_for_poll(snd_pcm_t *handle, pollfd *ufds, unsigned int count) {
    unsigned short revents;
    while (1) {
        poll(ufds, count, -1);
        snd_pcm_poll_descriptors_revents(handle, ufds, count, &revents);
        if (revents & POLLERR)
            return -EIO;
        if (revents & POLLOUT)
            return 0;
    }
}

int Stream::write_and_poll_loop(snd_pcm_t *handle,
                   signed short *samples,
                   snd_pcm_channel_area_t *areas) {

    pollfd *ufds;
    signed short *ptr;

    int err, count, cptr, init;
    count = snd_pcm_poll_descriptors_count (handle);
    if (count <= 0) {
        printf("Invalid poll descriptors count\n");
        return count;
    }

    ufds = new pollfd[count];
    if (ufds == NULL) {
        printf("No enough memory\n");
        return -ENOMEM;
    }

    if ((err = snd_pcm_poll_descriptors(handle, ufds, count)) < 0) {
        printf("Unable to obtain poll descriptors for playback: %s\n", snd_strerror(err));
        return err;
    }

    init = 1;
    while (1) {

        if (!init) {
            
            err = wait_for_poll(handle, ufds, count);
            if (err < 0) {

                if (snd_pcm_state(handle) == SND_PCM_STATE_XRUN ||
                    snd_pcm_state(handle) == SND_PCM_STATE_SUSPENDED) {

                    err = snd_pcm_state(handle) == SND_PCM_STATE_XRUN ? -EPIPE : -ESTRPIPE;
                    if (xrun_recovery(handle, err) < 0) {
                        printf("Write error: %s\n", snd_strerror(err));
                        exit(EXIT_FAILURE);
                    }
                    init = 1;

                } else {
                    printf("Wait for poll failed\n");
                    return err;
                }
            }
        }

        //generate_sine(areas, 0, period_size, &phase);
        ptr = samples;
        cptr = period_size;

        while (cptr > 0) {

            err = snd_pcm_writei(handle, ptr, cptr);
            if (err < 0) {

                if (xrun_recovery(handle, err) < 0) {
                    printf("Write error: %s\n", snd_strerror(err));
                    exit(EXIT_FAILURE);
                }
                init = 1;
                break;  // skip one period 

            }

            if (snd_pcm_state(handle) == SND_PCM_STATE_RUNNING)
                init = 0;

            ptr += err * channels;
            cptr -= err;

            if (cptr == 0)
                break;
            
            // it is possible, that the initial buffer cannot store
            // all data from the last period, so wait awhile 
            err = wait_for_poll(handle, ufds, count);
            if (err < 0) {
                if (snd_pcm_state(handle) == SND_PCM_STATE_XRUN ||
                    snd_pcm_state(handle) == SND_PCM_STATE_SUSPENDED) {

                    err = snd_pcm_state(handle) == SND_PCM_STATE_XRUN ? -EPIPE : -ESTRPIPE;

                    if (xrun_recovery(handle, err) < 0) {
                        printf("Write error: %s\n", snd_strerror(err));
                        exit(EXIT_FAILURE);
                    }

                    init = 1;

                } else {
                    printf("Wait for poll failed\n");
                    return err;
                }
            }

        }

    }
}
