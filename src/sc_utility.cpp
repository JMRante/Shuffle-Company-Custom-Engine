/*

    SHUFFLE COMPANY SOURCE CODE

    sc_utility.cpp
    ------------------------------------

    This file contains various utility functions and classes that have general purpose use
    throughout the Shuffle Company code base.

*/

#include "sc_utility.h"

namespace sc
{
    //Stolen shamelessly from the SDL Wiki article on SDL_RWread
    char* fileRead(const char* filename)
    {
        SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
        if (rw == NULL) 
        {
            return NULL;
        }

        Sint64 res_size = SDL_RWsize(rw);
        char* res = (char*)malloc(res_size + 1);

        Sint64 nb_read_total = 0, nb_read = 1;
        char* buf = res;
        while (nb_read_total < res_size && nb_read != 0) 
        {
            nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
            nb_read_total += nb_read;
            buf += nb_read;
        }

        SDL_RWclose(rw);
        if (nb_read_total != res_size) 
        {
            free(res);
            return NULL;
        }

        res[nb_read_total] = '\0';
        return res;
    }
}