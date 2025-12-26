#include "core/logger.h"
#include "core/utils/string.h"

#include <stdlib.h>

struct String *strCreate()
{
    struct String *str = malloc(sizeof(struct String));
    if (!str)
    {
        ERROR("Failed to allocate memory for string\n");
        return NULL;
    }

    *str = (struct String) { 0 };
    return str;
}
