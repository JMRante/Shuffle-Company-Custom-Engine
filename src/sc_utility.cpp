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

    std::string iToS(int i)
    {
      std::ostringstream oss;
      oss << i;
      return oss.str();
    }

    std::string fToS(float f)
    {
      std::ostringstream oss;
      oss << f;
      return oss.str();
    }

    //Function for quick power of two padding taken from Lazy Foo Production's OpenGL tutorial
    //Fancy bitwise magic
    GLuint powerOfTwo(GLuint num) 
    { 
        if (num != 0) 
        { 
            num--; 
            num |= (num >> 1);
            num |= (num >> 2);
            num |= (num >> 4); 
            num |= (num >> 8);
            num |= (num >> 16);
            num++; 
        }

        return num;
    }

    /*
        ID
            */
    ID::ID()
    {
        id[0] = '\0';
    }

    ID::ID(const char* id)
    {
        int i = 0;
        bool end = false;

        while (!end && i < ID_SIZE)
        {
            this->id[i] = id[i];

            if (id[i] == '\0')
            {
                end = true;
            }

            i++;
        }
    }

    bool ID::is(const char* id)
    {
        int i = 0;
        bool end = false;

        while (!end && i < ID_SIZE)
        {
            if (this->id[i] != id[i])
            {
                return false;
            }

            if (this->id[i] == '\0' || id[i] == '\0')
            {
                end = true;
            }

            i++;
        }

        return true;
    }

    bool ID::is(ID id)
    {
        int i = 0;
        bool end = false;

        while (!end && i < ID_SIZE)
        {
            if (this->id[i] != id.id[i])
            {
                return false;
            }

            if (this->id[i] == '\0' || id.id[i] == '\0')
            {
                end = true;
            }

            i++;
        }

        return true;
    }

    char* ID::get()
    {
        return id;
    }
}