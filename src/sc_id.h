/*

    SHUFFLE COMPANY SOURCE CODE

    sc_id.h
    ------------------------------------

    This file defines the ID functionality used to assign entities, tags, and assets unique
    IDs. These IDs are unsigned integers that were created from hashing a string on compile 
    time (or, sometimes, on runtime).

*/

#ifndef SC_ID
#define SC_ID

#include <functional>
#include <string>

namespace sc
{
    typedef unsigned int ID;

    unsigned int CTID(const char* cstr);
    unsigned int RTID(std::string str);
}

#endif