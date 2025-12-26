#ifndef CORE_STRING_H
#define CORE_STRING_H

#include "core/defines.h"

/*
 * TODO:
 *  1. define a default initial string length
 *  2. various functions to append, concatinate, free, etc
 *  3. this will later be used as a foundation in the text editor's buffer structures (ropes?)
 */

struct String
{
    const char  *strData;
    unsigned int strLength;
    unsigned int strCapacity;
};

CORE_API struct String *strCreate();

#endif
