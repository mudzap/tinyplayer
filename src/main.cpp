#include <stdio.h>
#include <math.h>
#include <alsa/asoundlib.h>
#include <curses.h>

#include "playlist.h"
#include "dsp/dsp.h"
#include "stream.h"
#include "buffer.h"
#include "interface.h"
#include "file_system.h"

static char *DEVICE = "default";            /* playback device */
unsigned char buffer[16*1024];              /* some random data */

class Interface;
class File_System;
class DSP;
class Stream;
class Playlist;

typedef struct{
    Playlist plist;
    Interface intf;
    DSP dsp;
    Stream str;
} Player;

int demo();
int init_player(Player &p);
int main_loop(Player &p);
int clear_all(Player &p);

typedef struct{
    int eq_values[12];
    // Primero estructurar como realizar el DSP en si, luego proponer configuración
    // Possibly use ALSA's config module itself 
} DSP_Config;

int main(int argc, const char* argv[]) {

#ifndef NDEBUG
	printf("Starting tinyplayer\n");
#endif

    //INIT
    //Read cached stuff
    //No cached stuff? Make it
    //Must init the following:

#ifndef NDEBUG
	printf("Initializing\n");
#endif
    File_System fs;
    Player player;
    player.str = Stream(DEVICE);

    init_player(player);


    //Proposed pipeline:
    /*
    Phase:              Handled by:     Comments:

    INPUT               Audio_Read      Read & Decode from file/playlist/etc...
    PRE-PROCCESSING     Audio_Read      Transform into processable data (separate channels, give parameters)
    DSP                 DSP             Process
    OUTPUT              Stream          Output to speakers/etc...
    */


    main_loop(player);


#ifndef NDEBUG
	printf("Terminating\n");
#endif

    //EXIT
    clear_all(player);

	return 0;


}

int init_player(Player &p) {

    //curses init
    //player.intf->init();

    return 0;

}

int main_loop(Player &p) {

    //player.buf
    //if(player.dsp->isActive)

    return 0;

}

int clear_all(Player &p) {

    p.str.free_resources();
    //player.buf
    //if(player.dsp->isActive)

    return 0;

}