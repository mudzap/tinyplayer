#include "stream.h"

Stream::Stream() {
    //Do nothing
}

Stream::Stream(const char* name, snd_async_callback_t callback, void* data)
    : pcm_name(name), async_callback(callback), callback_data(data) {
    
#ifndef NDEBUG
    printf("Setting up stream with name: %s\n", name);
#endif

    int error;

#ifndef NDEBUG
    printf("Opening pcm channel\n");
#endif

    error = snd_pcm_open(
        &pcm_handle,
        name,
        SND_PCM_STREAM_PLAYBACK,
        SND_PCM_ASYNC
        );
    
    if(error != 0) {
        printf("Failed to open pcm channel\n");
        failure = true; 
    }

#ifndef NDEBUG
    printf("Setting up callback function\n");
#endif

    error = snd_async_add_pcm_handler(
        async_handler,
	    pcm_handle,
	    callback,
	    data 
	);

    if(error != 0) {
        printf("Failed to setup callback for asynchronous function\n");
        failure = true; 
    }

}

void Stream::write_frame_i() {

}