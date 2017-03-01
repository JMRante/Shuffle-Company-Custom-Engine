/*

    SHUFFLE COMPANY SOURCE CODE

    sc_main.h
    ------------------------------------



*/

#ifndef SC_MAIN
#define SC_MAIN

#include <SDL.h>

//Defines
#define MS_PER_FRAME 16


extern SDL_Window *window;
extern SDL_GLContext glContext;

extern bool hasQuit;

extern Uint32 startTime;
extern Sint32 delay;

#endif