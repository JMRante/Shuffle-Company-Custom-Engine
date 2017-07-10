/*

    SHUFFLE COMPANY SOURCE CODE

    sc_id.cpp
    ------------------------------------

    This file defines the ID functionality used to assign entities, tags, and assets unique
    IDs. These IDs are unsigned integers that were created from hashing a string on compile 
    time (or, sometimes, on runtime).

*/

#include "sc_id.h"

namespace sc
{
    unsigned int CTID(const char* cstr)
    {
        std::string str = std::string(cstr);
        std::hash<std::string> hasher;
        return hasher(str);
    }

    unsigned int RTID(std::string str)
    {
        std::hash<std::string> hasher;
        return hasher(str);
    }
}